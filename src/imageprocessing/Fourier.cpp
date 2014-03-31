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

#include "Fourier.hpp"
#include <Types.hpp>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    Fourier<T>::Fourier(size_t length)
      : m_spatialRepr(NULL), m_frequencyRepr(NULL), m_length(length)
    {
      m_spatialRepr = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*length);
      m_frequencyRepr = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*length);
    }

    template<typename T>
    Fourier<T>::~Fourier()
    {
      fftw_free(m_frequencyRepr);
      fftw_free(m_spatialRepr);

      // avoid memory leaks!! (see http://www.fftw.org/faq/section3.html#leaks)
      fftw_cleanup();
    }

    template<typename T>
    void
    Fourier<T>::forward(const T* in, double* reOut, double* imOut, bool center) const
    {
      for (size_t i = 0; i < m_length; ++i) {
	m_spatialRepr[i][0] = in[i];
	m_spatialRepr[i][1] =  0.0f;
      }

      fftw_plan plan = fftw_plan_dft_1d(int(m_length), m_spatialRepr, m_frequencyRepr, FFTW_FORWARD, FFTW_ESTIMATE);
      fftw_execute(plan);

      size_t l_2 = m_length / 2;
      for (size_t i = 0; i < m_length; ++i) {
	// center data
	size_t x = i;
	if (center) {
	  if (i < l_2)
	    x = i + (m_length-l_2);
	  if (i >= l_2)
	    x = i - l_2;
	}
	reOut[x] = m_frequencyRepr[i][0];
	imOut[x] = m_frequencyRepr[i][1];
      }

      fftw_destroy_plan(plan);
    }

    template<typename T>
    void
    Fourier<T>::forward2d(size_t width, size_t height, const T* in, double* reOut, double* imOut, bool center) const
    {
      assert(m_length == width*height);

      for (size_t i = 0; i < m_length; ++i) {
	m_spatialRepr[i][0] = in[i];
	m_spatialRepr[i][1] =  0.0f;
      }

      fftw_plan plan = fftw_plan_dft_2d(int(width), int(height), m_spatialRepr, m_frequencyRepr, FFTW_FORWARD, FFTW_ESTIMATE);
      fftw_execute(plan);

      size_t w_2 = width / 2;
      size_t h_2 = height / 2;
      for (size_t j = 0; j < height; ++j) {
	size_t y = j;
	if (center) {
	  if (j < h_2)
	    y = j + (height-h_2);
	  if (j >= h_2)
	    y = j - h_2;
	}
	for (size_t i = 0; i < width; ++i) {
	  // center data
	  size_t x = i;
	  if (center) {
	    if (i < w_2)
	      x = i + (width-w_2);
	    if (i >= w_2)
	      x = i - w_2;
	  }
	  reOut[x+y*width] = m_frequencyRepr[i+j*width][0];
	  imOut[x+y*width] = m_frequencyRepr[i+j*width][1];
	}
      }

      fftw_destroy_plan(plan);
    }

    template<typename T>
    void
    Fourier<T>::backward(const double* reIn, const double* imIn, T* out, bool centered) const
    {
      size_t l_2 = m_length / 2;
      for (size_t i = 0; i < m_length; ++i) {
	// decenter
	size_t x = i;
	if (centered) {
	  if (i < l_2)
	    x = i + (m_length-l_2);
	  if (i >= l_2)
	    x = i - l_2;
	}
	m_frequencyRepr[i][0] = reIn[x];
	m_frequencyRepr[i][1] = imIn[x];
      }

      fftw_plan plan = fftw_plan_dft_1d(int(m_length), m_frequencyRepr, m_spatialRepr, FFTW_BACKWARD, FFTW_ESTIMATE);
      fftw_execute(plan);

      for (size_t i = 0; i < m_length; ++i)
	out[i] = T(m_spatialRepr[i][0] / double(m_length)); // :GLITCH: possible precision loss, or even truncation of negative values!

      fftw_destroy_plan(plan);
    }

    template<typename T>
    void
    Fourier<T>::backward2d(size_t width, size_t height, const double* reIn, const double* imIn, T* out, bool centered) const
    {
      assert(m_length == width*height);

      size_t w_2 = width / 2;
      size_t h_2 = height / 2;
      for (size_t j = 0; j < height; ++j) {
	size_t y = j;
	if (centered) {
	  if (j < h_2)
	    y = j + (height-h_2);
	  if (j >= h_2)
	    y = j - h_2;
	}
	for (size_t i = 0; i < width; ++i) {
	  // decenter
	  size_t x = i;
	  if (centered) {
	    if (i < w_2)
	      x = i + (width-w_2);
	    if (i >= w_2)
	      x = i - w_2;
	  }
	  m_frequencyRepr[i+j*width][0] = reIn[x+y*width];
	  m_frequencyRepr[i+j*width][1] = imIn[x+y*width];
	}
      }

      fftw_plan plan = fftw_plan_dft_2d(int(width), int(height), m_frequencyRepr, m_spatialRepr, FFTW_BACKWARD, FFTW_ESTIMATE);
      fftw_execute(plan);

      for (size_t i = 0; i < m_length; ++i)
	out[i] = T(m_spatialRepr[i][0] / double(m_length)); // :GLITCH: possible precision loss, or even truncation of negative values!

      fftw_destroy_plan(plan);
    }

    template<typename T>
    void
    Fourier<T>::I2MP(const double* reIn, const double* imIn, double* magOut, double* phaseOut) const
    {
      for (size_t i = 0; i < m_length; ++i) {
	double re = reIn[i];
	double im = imIn[i];
	magOut[i] = sqrt(re*re + im*im);
	if (re != 0) phaseOut[i] = atan2(im, re);
      }
    }

    template<typename T>
    void
    Fourier<T>::MP2I(const double* magIn, const double* phaseIn, double* reOut, double* imOut) const
    {
      for (size_t i = 0; i < m_length; ++i) {
	double m = magIn[i];
	double p = phaseIn[i];
	reOut[i] = m * cos(p);
	imOut[i] = m * sin(p);
      }
    }

    template<typename T>
    void
    Fourier<T>::multiply(const double* reIn, const double* imIn, double* reOut, double* imOut) const
    {
      for (size_t i = 0; i < m_length; ++i) {
	double a = reOut[i];
	double b = imOut[i];
	double c = reIn[i];
	double d = imIn[i];

	reOut[i] = a*c - b*d;
	imOut[i] = b*c + a*d;
      }
    }

#define MAKE_TEMPLATED(T)						\
    template Fourier<T>::Fourier(size_t length);			\
    template Fourier<T>::~Fourier();					\
									\
    template void Fourier<T>::forward(const T* in, double* reOut, double* imOut, bool center = false) const; \
    template void Fourier<T>::forward2d(size_t width, size_t length, const T* in, double* reOut, double* imOut, bool center = false) const; \
									\
    template void Fourier<T>::backward(const double* reIn, const double* imIn, T* out, bool centered = false) const; \
    template void Fourier<T>::backward2d(size_t width, size_t length, const double* reIn, const double* imIn, T* out, bool centered = false) const; \
									\
    template void Fourier<T>::I2MP(const double* reIn, const double* imIn, double* magOut, double* phaseOut) const;	\
    template void Fourier<T>::MP2I(const double* magIn, const double* phaseIn, double* reOut, double* imOut) const;	\
									\
    template void Fourier<T>::multiply(const double* reIn, const double* imIn, double* reOut, double* imOut) const;

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
