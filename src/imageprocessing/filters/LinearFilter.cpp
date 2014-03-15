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

#include "LinearFilter.hpp"
#include <core/image/Image.hpp>
#include <iostream>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    LinearFilter<T>::LinearFilter(const double* kern, size_t w, size_t h, size_t d)
      : LocalFilter<T>(kern, w, h, d)
    {}

    template<typename T>
    LinearFilter<T>::~LinearFilter()
    {}

    template<typename T, typename U>
    bool
    convolve2d(T* in, U* out, int dataSizeX, int dataSizeY, 
	       const double* kernel, int kernelSizeX, int kernelSizeY) {
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
      U* outPtr = out;
      const double* kPtr = kernel;

      // start convolution
      for (int i = 0; i < dataSizeY; ++i) { // number of rows
	// compute the range of convolution, the current row of kernel should be between these
	int rowMax = i + kCenterY;
	int rowMin = i - dataSizeY + kCenterY;

	for (int j = 0; j < dataSizeX; ++j) { // number of columns
	  // compute the range of convolution, the current column of kernel should be between these
	  int colMax = j + kCenterX;
	  int colMin = j - dataSizeX + kCenterX;

	  double outVal = 0; // set to 0 before accumulate

	  // flip the kernel and traverse all the kernel values
	  // multiply each kernel value with underlying input data
	  for (int m = 0; m < kernelSizeY; ++m) { // kernel rows
	    // check if the index is out of bound of input array
	    if (m <= rowMax && m > rowMin) {
	      for (int n = 0; n < kernelSizeX; ++n) {
		// check the boundary of array
		if (n <= colMax && n > colMin)
		  outVal += double(*(inPtr - n)) * *kPtr;
		++kPtr; // next kernel
	      }
	    }
	    else
	      kPtr += kernelSizeX; // out of bound, move to next row of kernel

	    inPtr -= dataSizeX; // move input data 1 raw up
	  }

	  *outPtr = U(outVal); // :GLITCH: possible precision loss, or even truncation of negative values!

	  kPtr = kernel; // reset kernel to (0,0)
	  inPtr = ++inPtr2; // next input
	  ++outPtr; // next output
	}
      }

      return true;
    }

    template<typename T, typename U>
    void
    processLinearFilter(sd::core::Image<T>& dest, const sd::core::ImageView_<U>& src,
			const std::vector<double>& kernel, size_t width, size_t height) {
      sd::Size sz = src.size();
      int w = int(sz.width());
      int h = int(sz.height());
      size_t d = sz.depth();

      size_t wh = w * h;

      dest.init(src.information());
      T* ptrOut = dest.getData();
      
      auto it = src.begin();

      for (size_t k = 0; k < d; ++k) {
	convolve2d(it.operator->(), ptrOut, w, h, &(kernel[0]), int(width), int(height));
	it += wh;
	ptrOut += wh;
      }
    }

    template<typename T>
    bool
    LinearFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) {
      sd::Size sz = src.size();
      size_t w = sz.width();
      size_t h = sz.height();
      size_t d = sz.depth();

      if (this->width() > w || this->height() > h || this->depth() > d) {
	std::cerr << "LinearFilter::applyTo: filter must be smaller than image.\n";
	return NULL;
      }

      // decide if using double in order to avoid possible data truncation
      if (this->hasNegativeValues() && isUnsignedType<T>()) {
	// data copy
	sd::core::Image<double> tmp(src.information());
	auto it = tmp.begin();
	for (auto it2 = src.begin(); it2 != src.end(); ++it, ++it2)
	  *it = *it2;
	
	processLinearFilter<T, double>(dest, tmp, this->kernel(), this->width(), this->height());
	return true;
      } else {
	processLinearFilter<T, T>(dest, src, this->kernel(), this->width(), this->height());
	return true;
      }
    }


#define MAKE_TEMPLATED(T)						\
    template LinearFilter<T>::LinearFilter(const double* kernel, size_t width, size_t height, size_t depth); \
    template LinearFilter<T>::~LinearFilter();				\
    template bool LinearFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src);


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
