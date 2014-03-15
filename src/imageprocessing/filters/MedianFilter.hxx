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

#ifndef SD_IMAGE_PROCESSING_MEDIAN_FILTER_HXX
#define SD_IMAGE_PROCESSING_MEDIAN_FILTER_HXX

#include <algorithm>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    MedianFilter<T>::MedianFilter(size_t w, size_t h, size_t d)
      : MorphoFilter<T>(w, h, d)
    {
    }

    template<typename T>
    MedianFilter<T>::~MedianFilter()
    {
    }

    template<typename T>
    T
    MedianFilter<T>::processNeighborhood(std::vector<T>& neigh)
    {
      size_t cnt = neigh.size();
      auto itr = neigh.begin();
      std::sort(itr, itr+cnt);
      return neigh[cnt/2];
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_MEDIAN_FILTER_HXX */
