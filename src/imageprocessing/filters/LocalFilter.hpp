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

#ifndef SD_IMAGE_PROCESSING_LOCAL_FILTER_HPP
#define SD_IMAGE_PROCESSING_LOCAL_FILTER_HPP

#include "Filter.hpp"
#include <core/image/ImageView_.hpp>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    class LocalFilter
      : public Filter<T>
    {

    public:

      LocalFilter(const double* kernel, size_t width, size_t height, size_t depth);
      virtual ~LocalFilter();

      virtual bool applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) = 0;

    private:

      LocalFilter(const LocalFilter& f) = delete;
      LocalFilter& operator=(const LocalFilter& f) = delete;

    };

  }
}

#include "LocalFilter.hxx"

#endif /* #! SD_IMAGE_PROCESSING_LOCAL_FILTER_HPP */
