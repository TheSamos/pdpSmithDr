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

#ifndef SD_IMAGE_PROCESSING_MEDIAN_FILTER_HPP
#define SD_IMAGE_PROCESSING_MEDIAN_FILTER_HPP

#include "MorphoFilter.hpp"

namespace sd {
  namespace imageprocessing {

    template<typename T>
    class MedianFilter
      : public MorphoFilter<T>
    {

    public:

      MedianFilter(size_t width, size_t height, size_t depth);
      virtual ~MedianFilter();

      virtual T processNeighborhood(std::vector<T>& neigh);

    private:

      MedianFilter(const MedianFilter& f) = delete;
      MedianFilter& operator=(const MedianFilter& f) = delete;

    };

  }
}

#include "MedianFilter.hxx"

#endif /* #! SD_IMAGE_PROCESSING_MEDIAN_FILTER_HPP */
