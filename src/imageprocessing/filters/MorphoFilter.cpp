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

#include "StructuralElement.hpp"
#include "MorphoFilter.hpp"
#include <core/image/Image.hpp>
#include <iostream>
#include <algorithm>

namespace sd {
  
  namespace imageprocessing {

    template<typename T>
    MorphoFilter<T>::MorphoFilter(const StructuralElement& element)
      : NonLinearFilter<T>(&(element.getKernel()[0]), element.getWidth(), element.getHeight(), element.getDepth())
    {}

    template<typename T>
    MorphoFilter<T>::MorphoFilter(size_t w, size_t h, size_t d)
      : NonLinearFilter<T>(NULL, w, h, d) {
      std::fill(this->kernel().begin(), this->kernel().end(), 1);
    }

    template<typename T>
    MorphoFilter<T>::~MorphoFilter()
    {}

    template<typename T>
    bool
    morpho2d(MorphoFilter<T>* filter, T* in, T* out, int dataSizeX, int dataSizeY, 
	     double* kernel, int kernelSizeX, int kernelSizeY) {
      if (!in || !out || !kernel)
	return false;
      if (dataSizeX <= 0 || kernelSizeX <= 0)
	return false;

      // find center position of kernel (half of kernel size)
      int kCenterX = (kernelSizeX >> 1);
      int kCenterY = (kernelSizeY >> 1);

      // init working  pointers
      T* inPtr = &in[dataSizeX * kCenterY + kCenterX];  // note that it is shifted (kCenterX, kCenterY)
      T* inPtr2 = inPtr;
      T* outPtr = out;
      double* kPtr = kernel;

      std::vector<T> neigh(kernelSizeX*kernelSizeY);

      // start convolution
      for (int i = 0; i < dataSizeY; ++i) { // number of rows
	// compute the range of convolution, the current row of kernel should be between these
	int rowMax = i + kCenterY;
	int rowMin = i - dataSizeY + kCenterY;

	for (int j = 0; j < dataSizeX; ++j) { // number of columns
	  // compute the range of convolution, the current column of kernel should be between these
	  int colMax = j + kCenterX;
	  int colMin = j - dataSizeX + kCenterX;

	  *outPtr = 0; // set to 0 before accumulate

	  // flip the kernel and traverse all the kernel values
	  int cnt = 0;
	  for (int m = 0; m < kernelSizeY; ++m) { // kernel rows
	    // check if the index is out of bound of input array
	    if (m <= rowMax && m > rowMin) {
	      for (int n = 0; n < kernelSizeX; ++n) {
		// check the boundary of array
		if (n <= colMax && n > colMin) {
		  neigh[cnt++] = *(inPtr - n);
		}
		++kPtr; // next kernel
	      }
	      // determine new pixel value from neighbors
	      if (cnt)
		*outPtr = filter->processNeighborhood(neigh);
	    }
	    else
	      kPtr += kernelSizeX; // out of bound, move to next row of kernel

	    inPtr -= dataSizeX; // move input data 1 raw up
	  }

	  kPtr = kernel; // reset kernel to (0,0)
	  inPtr = ++inPtr2; // next input
	  ++outPtr; // next output
	}
      }

      return true;
    }

    template<typename T>
    bool
    MorphoFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) {
      sd::Size sz = src.size();
      size_t w = sz.width();
      size_t h = sz.height();
      size_t d = sz.depth();

      if (this->width() > w || this->height() > h || this->depth() > d) {
	std::cerr << "MorphoFilter::applyTo: filter must be smaller than image.\n";
	return NULL;
      }

      size_t wh = w * h;
      dest.init(src.information());

      auto it = src.begin();
      T* ptrOut = dest.getData();
      for (size_t k = 0; k < d; ++k) {
	morpho2d<T>(this, it.operator->(), ptrOut, int(w), int(h), &(this->kernel()[0]), int(this->width()), int(this->height()));
	it += wh;
	ptrOut += wh;
      }
      
      return true;
    }


#define MAKE_TEMPLATED(T)						\
    template MorphoFilter<T>::MorphoFilter(size_t width, size_t height, size_t depth); \
    template MorphoFilter<T>::MorphoFilter(const StructuralElement& element); \
    template MorphoFilter<T>::~MorphoFilter();				\
    template bool MorphoFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src);


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
