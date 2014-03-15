/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Types.hpp>
#include "ImageRestoration.hpp"
#include "Hartley.hpp"
#include <core/image/Image.hpp>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    ImageRestoration<T>::ImageRestoration()
    {}

    template<typename T>
    ImageRestoration<T>::~ImageRestoration()
    {}

    Hartley*
    buildPSF(size_t width, size_t height, size_t depth, double* spatial_psf, size_t psf_size) {
      size_t n = width * height * depth;

      // construction du filtre, centré, normalisé
      double* psf = new double[n];
      memset(psf, 0, n*sizeof(double));

      int offx = ((int(width) >> 1) + 1);
      int offy = ((int(height) >> 1) + 1);
      int f0 = (int(psf_size) >> 1);
      int f1 = (int(psf_size)-f0-1);
      for (int j = -f0, index = 0; j <= f1; ++j)
	for (int i = -f0; i <= f1; ++i, ++index)
	  psf[i+offx + (j+offy)*width] = spatial_psf[index];
      for (size_t i = 1; i < depth; ++i)
	memcpy(psf+i*width*height, psf, width*height*sizeof(double));

      Hartley* fht_psf = new Hartley(psf, width, height, depth, Hartley::FHT_SPATIAL, 1.5, Hartley::FHT_CENTERED_MASK);
      delete [] psf;

      return fht_psf;
    }

    template<typename T>
    sd::core::ImageView_<double>*
    ImageRestoration<T>::pseudoInverseFilter(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, double coeff) {
      if (!spatial_psf)
	return NULL;

      sd::Size sz = iv.size();
      size_t width = sz.width();
      size_t height = sz.height();
      size_t depth = sz.depth();
      size_t length = width * height * depth;

      Hartley* fht_psf = buildPSF(width, height, depth, spatial_psf, psf_size);
      fht_psf->transform(Hartley::FHT_FORWARD);

      // dimensions étendues
      size_t we = fht_psf->getWidth();
      size_t he = fht_psf->getHeight();
      size_t de = fht_psf->getDepth();

      // Les données
      Hartley fht_reco(iv.begin().operator->(), width, height, depth, Hartley::FHT_SPATIAL, 1.5, Hartley::FHT_MIRROR);
      fht_reco.transform(Hartley::FHT_FORWARD);

      Hartley* fht_deconv = NULL;
      if (coeff == 0)
	fht_deconv = fht_reco.divide(*fht_psf);
      else
	fht_deconv = fht_reco.divideGamma(*fht_psf, 1, coeff);

      delete fht_psf;
      fht_deconv->transform(Hartley::FHT_BACKWARD);

      // découpe l'image résultante aux dimensions de l'image initiale
      int kOff = ((int(de) - int(depth) + 1) >> 1);
      int jOff = ((int(he) - int(height) + 1) >> 1);
      int iOff = ((int(we) - int(width) + 1) >> 1);
      double* data_deconv = new double[length];
      for (size_t k = 0, index = 0; k < depth; ++k) {
	double* slice = fht_deconv->getSlice(k+kOff);
	for (size_t j = 0; j < height; ++j)
	  for (size_t i = 0; i < width; ++i, ++index) {
	    double val = slice[i+iOff + we*(j+jOff)];
	    data_deconv[index] = val;
	  }
      }

      delete fht_deconv;

      sd::core::Image<double>* img = new sd::core::Image<double>(iv.information());
      img->setData(data_deconv);
      return img;
    }

    /*
      Returns the image resulting from the point by point Hartley division
      of this image by the specified image. Both images are assumed to be in
      the frequency domain. Division in the frequency domain is equivalent
      to deconvolution in the space domain.

      So far, this is Wanye Rasband's FHD divide.  The generalized part is that
      zero is returned for frequencies where the magnitude of the spectral
      component of h2 is less than alpha times the largest magnitude.  This is
      analogous to application of the generalized inverse in linear algebra.
    */
    void
    generalizedDivide(Hartley& ht, Hartley& filt, double alpha, int maxN) {
      double* out = new double[maxN*maxN];
      size_t depth = ht.getDepth();

      for (size_t slice = 0; slice < depth; ++slice) {
	double* h1 = ht.getSlice(slice);
	double* h2 = filt.getSlice(slice);

	double maxMag = 0;
	for (int r = 0; r < maxN; ++r) {
	  int rowMod = (maxN - r) % maxN;
	  for (int c = 0; c < maxN; ++c) {
	    int colMod = (maxN - c) % maxN;
	    double mag = h2[r*maxN+c] * h2[r*maxN+c] + h2[rowMod*maxN+colMod] * h2[rowMod*maxN+colMod];
	    if (mag > maxMag)
	      maxMag = mag;
	  }
	}

	double limit = alpha*maxMag;
	for (int r = 0; r < maxN; ++r) {
	  int rowMod = (maxN - r) % maxN;
	  for (int c = 0; c < maxN; ++c) {
	    int colMod = (maxN - c) % maxN;
	    double mag = h2[r*maxN+c] * h2[r*maxN+c] + h2[rowMod*maxN+colMod] * h2[rowMod*maxN+colMod];

	    double h2e = (h2[r*maxN+c] + h2[rowMod*maxN+colMod]);
	    double h2o = (h2[r*maxN+c] - h2[rowMod*maxN+colMod]);
	    double tmp = (h1[r*maxN+c] * h2e - h1[rowMod*maxN+colMod] * h2o);
	    out[r*maxN+c] = tmp/mag;

	    if (limit > 20.7*mag)
	      out[r*maxN+c] = 0;
	    else {
	      mag *= exp(limit/mag);
	      out[r*maxN+c] = tmp/mag;
	    }
	  }
	}

	size_t w = ht.getWidth() * ht.getHeight();
	for (size_t i = 0; i < w; ++i)
	  ht.getSlice(slice)[i] = out[i];
      }

      delete [] out;
    }

    /*
      filter out spectral components of arg for which the
      corresponding component of fht is less than alpha times the
      max component of fht.
    */
    void
    filter(Hartley& ht, Hartley& filt, double alpha, int maxN) {
      size_t depth = ht.getDepth();
      for (size_t slice = 0; slice < depth; ++slice) {
	double* h1 = ht.getSlice(slice);
	double* h2 = filt.getSlice(slice);
	double maxMag = 0;
	for (int r = 0; r < maxN; ++r) {
	  int rowMod = (maxN - r) % maxN;
	  for (int c = 0; c < maxN; ++c) {
	    int colMod = (maxN - c) % maxN;
	    double mag = h2[r*maxN+c] * h2[r*maxN+c] + h2[rowMod*maxN+colMod] * h2[rowMod*maxN+colMod];
	    if (mag > maxMag)
	      maxMag = mag;
	  }
	}

	double limit = alpha*maxMag;
	for (int r = 0; r < maxN; ++r) {
	  int rowMod = (maxN - r) % maxN;
	  for (int c = 0; c < maxN; ++c) {
	    int colMod = (maxN - c) % maxN;
	    double mag = h2[r*maxN+c] * h2[r*maxN+c] + h2[rowMod*maxN+colMod] * h2[rowMod*maxN+colMod];
	    if (limit > 30*mag)
	      h1[r*maxN+c] = 0;
	    else
	      h1[r*maxN+c] *= exp(-limit/mag);
	  }
	}
      }
    }

    template<typename T>
    sd::core::ImageView_<double>*
    ImageRestoration<T>::iterativeDeconvolution(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, int nbIter) {
      if (!spatial_psf)
	return NULL;

      sd::Size sz = iv.size();
      size_t width = sz.width();
      size_t height = sz.height();
      size_t depth = sz.depth();
      Hartley* fht_psf = buildPSF(width, height, depth, spatial_psf, psf_size);
      // dimensions étendues
      size_t we = fht_psf->getWidth();
      size_t he = fht_psf->getHeight();
      size_t de = fht_psf->getDepth();
      int kOff = ((int(de) - int(depth) + 1) >> 1);
      int jOff = ((int(he) - int(height) + 1) >> 1);
      int iOff = ((int(we) - int(width) + 1) >> 1);

      double psfSum = 0;
      for (size_t j = 0; j < de; ++j)
	for (size_t i = 0; i < we*he; ++i)
	  psfSum += fht_psf->getSlice(j)[i];

      fht_psf->transform(Hartley::FHT_FORWARD);

      // Les données
      Hartley fht_init(iv.begin().operator->(), width, height, depth, Hartley::FHT_SPATIAL, 1.5, Hartley::FHT_MIRROR);
      assert(we == he);
      assert(we == fht_init.getWidth());
      assert(he == fht_init.getHeight());
      assert(de == fht_init.getDepth());

      Hartley fht_deconv(fht_init);
      double alpha = 1000;
      size_t sliceLength = we*he;

      // PROCÉSSUS ITÉRATIF
      for (int iter = 0; iter < nbIter; ++iter) {
	Hartley copy(fht_deconv);

	// convolve copy and psf
	copy.transform(Hartley::FHT_FORWARD);
	Hartley* fht_conv = copy.multiply(*fht_psf, 0);
	fht_conv->transform(Hartley::FHT_BACKWARD);
	//result->swapQuadrants();
	copy.transform(Hartley::FHT_BACKWARD);

	// copy = fht_init - copy*psf
	for (size_t j = 0; j < de; ++j)
	  for (size_t i = 0; i < sliceLength; ++i)
	    copy.getSlice(j)[i] = fht_init.getSlice(j)[i] - fht_conv->getSlice(j)[i] / psfSum;

	delete fht_conv;
	copy.transform(Hartley::FHT_FORWARD);
	generalizedDivide(copy, *fht_psf, 1./alpha, int(we)); // assume we == he
	copy.transform(Hartley::FHT_BACKWARD);
	//copy.swapQuadrants();
	double meanDelta = 0;
	for (size_t j = 0; j < de; ++j)
	  for (size_t i = 0; i < sliceLength; ++i) {
	    copy.getSlice(j)[i] *= psfSum;
	    double delta = copy.getSlice(j)[i];
	    fht_deconv.getSlice(j)[i] += delta;
	    meanDelta += fabs(delta);
	  }
	fht_deconv.transform(Hartley::FHT_FORWARD);
	filter(fht_deconv, *fht_psf, 1/(alpha*alpha), int(we)); // assume we == he
	fht_deconv.transform(Hartley::FHT_BACKWARD);

	// energy sum to track convergence
	double sum_pixels = 0;
	for (size_t j = 0; j < de; ++j)
	  for (size_t i = 0; i < sliceLength; ++i)
	    sum_pixels += fht_deconv.getSlice(j)[i];

	//double percent_change = 100.*meanDelta/sum_pixels;
	//std::cout << "percent_change: " << percent_change << std::endl;
      }
      // FIN PROCÉSSUS ITÉRATIF

      delete fht_psf;
      double* data_deconv = new double[width*height*depth];
      for (size_t k = 0; k < depth; ++k) {
	double* slice = fht_deconv.getSlice(k+kOff);
	for (size_t j = 0, index = 0; j < height; ++j)
	  for (size_t i = 0; i < width; ++i, ++index) {
	    double val = slice[i+iOff + we*(j+jOff)];
	    data_deconv[index] = val / psfSum;
	  }
      }

      sd::core::Image<double>* img = new sd::core::Image<double>(iv.information());
      img->setData(data_deconv);
      return img;
    }

#define MAKE_TEMPLATED(T)						\
    template ImageRestoration<T>::ImageRestoration();			\
    template ImageRestoration<T>::~ImageRestoration();			\
									\
    template sd::core::ImageView_<double>* ImageRestoration<T>::pseudoInverseFilter(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, double coeff); \
									\
    template sd::core::ImageView_<double>* ImageRestoration<T>::inverseFilter(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size); \
									\
    template sd::core::ImageView_<double>* ImageRestoration<T>::iterativeDeconvolution(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, int nbIter = 5);


    MAKE_TEMPLATED(float)
    MAKE_TEMPLATED(double)
    MAKE_TEMPLATED(sd::INT8)
    MAKE_TEMPLATED(sd::UINT8)
    MAKE_TEMPLATED(sd::INT16)
    MAKE_TEMPLATED(sd::UINT16)
    MAKE_TEMPLATED(sd::INT32)
    MAKE_TEMPLATED(sd::UINT32)

#undef MAKE_TEMPLATED

  }
}
