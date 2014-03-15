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

#include "FrequencyFilter.hpp"
#include <imageprocessing/Fourier.hpp>
#include <core/image/Image.hpp>
#include <iostream>
#include <cmath>

namespace sd {
  
  namespace imageprocessing {

    template<typename T>
    FrequencyFilter<T>::FrequencyFilter(const double* kern, size_t w, size_t h, size_t d)
      : GlobalFilter<T>(kern, w, h, d)
    {}

    template<typename T>
    FrequencyFilter<T>::~FrequencyFilter()
    {}

    template<typename T>
    bool
    FrequencyFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) {
      sd::Size sz = src.size();
      size_t w = sz.width();
      size_t h = sz.height();
      size_t d = sz.depth();

      if (this->width() != w || this->height() != h || this->depth() != d) {
	std::cerr << "FrequencyFilter::applyTo: image and filter must have the same size.\n";
	return false;
      }

      size_t wh = w * h;
      dest.init(src.information());
      
      std::vector<double> reFilt(this->kernel());
      std::vector<double> imFilt(wh, 0);
      
      std::vector<double> re(wh);
      std::vector<double> im(wh);
      auto it = src.begin();
      T* ptrOut = dest.getData();
      Fourier<T> fourier(wh);
      
      for (sd::UINT16 k = 0; k < d; ++k) {
	fourier.forward2d(w, h, it.operator->(), &(re[0]), &(im[0]), true);
	fourier.multiply(&(reFilt[0]), &(imFilt[0]), &(re[0]), &(im[0]));
	fourier.backward2d(w, h, &(re[0]), &(im[0]), ptrOut, true);
	it += wh;
	ptrOut += wh;
      }
      return true;
    }


#define MAKE_TEMPLATED(T)						\
    template FrequencyFilter<T>::FrequencyFilter(const double* kernel, size_t width, size_t height, size_t depth); \
    template FrequencyFilter<T>::~FrequencyFilter();			\
									\
    template bool FrequencyFilter<T>::applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src);


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
