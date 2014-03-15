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
#include "GradientFilter.hpp"
#include <core/image/Image.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    GradientFilter<T>::GradientFilter(const double* kernel1, const double* kernel2, size_t size)
      : NonLinearFilter<T>(NULL, size, size, 2*1) { // 2 kernels with depth=1
      if (kernel1 && kernel2) {
	std::copy(kernel1, kernel1+size*size, this->kernel().begin());
	std::copy(kernel2, kernel2+size*size, this->kernel().begin()+size*size);
      }
    }

    template<typename T>
    GradientFilter<T>::~GradientFilter()
    {}
    
    template<typename T, typename U>
    void
    processGradientFilter(sd::core::Image<T>& dest, const sd::core::ImageView_<U>& src,
			  const std::vector<double>& kernel, size_t width, size_t height) {
      sd::Size sz = src.size();
      size_t w = sz.width();
      size_t h = sz.height();
      size_t d = sz.depth();
      
      LinearFilter<U> ph(&(kernel[0]), width, height, 1);
      LinearFilter<U> pv(&(kernel[(kernel.size() >= width*height*2) ? width*height : 0]), width, height, 1);
      // ???
      
      sd::core::Image<U> ivh;
      sd::core::Image<U> ivv;

      ph.applyTo(ivh, src); // slice-by-slice 2d filtering
      pv.applyTo(ivv, src); // slice-by-slice 2d filtering

      auto ith = ivh.begin();
      auto itv = ivv.begin();

      size_t length = w * h * d;
      dest.init(src.information());
      T* dataConv = dest.getData();

      for (size_t i = 0 ; i < length && ith != ivh.end() && itv != ivv.end(); ++i, ++ith, ++itv) {
	double hVal = double(*ith);
	double vVal = double(*itv);
	dataConv[i] = T(sqrt(hVal*hVal + vVal*vVal));
      }
      
    }

    template<typename T>
    bool
    GradientFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) {
      sd::Size sz = src.size();
      size_t w = sz.width();
      size_t h = sz.height();
      size_t d = sz.depth();

      if (this->width() > w || this->height() > h || this->depth()/2 > d) {
	std::cerr << "GradientFilter::applyTo: filter must be smaller than image.\n";
	return false;
      }

      // decide if using double in order to avoid possible data truncation
      if (this->hasNegativeValues() && isUnsignedType<T>()) {
	// data copy
	sd::core::Image<double> tmp(src.information());
	double* tmpData = tmp.getData();
	std::fill(tmpData, tmpData+src.size().dataSize(), 0);
	auto it = tmp.begin();
	for (auto it2 = src.begin(); it2 != src.end(); ++it, ++it2)
	  *it = *it2;

	processGradientFilter<T,double>(dest, tmp, this->kernel(), this->width(), this->height());
	return true;
      } else {
	processGradientFilter<T,T>(dest, src, this->kernel(), this->width(), this->height());
	return true;
      }
    }


#define MAKE_TEMPLATED(T)						\
    template GradientFilter<T>::GradientFilter(const double* kernel1, const double* kernel2, size_t size); \
    template GradientFilter<T>::~GradientFilter();			\
    template bool GradientFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src);


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
