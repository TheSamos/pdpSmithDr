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

#ifndef SD_IMAGE_PROCESSING_DILATION_FILTER_HXX
#define SD_IMAGE_PROCESSING_DILATION_FILTER_HXX

namespace sd {
  namespace imageprocessing {

    template<typename T>
    DilationFilter<T>::DilationFilter(const StructuralElement& element)
      : MorphoFilter<T>(element)
    {
    }

    template<typename T>
    DilationFilter<T>::~DilationFilter()
    {
    }

    template<typename T>
    T
    DilationFilter<T>::processNeighborhood(std::vector<T>& neigh)
    {
      return *std::max_element(neigh.begin(), neigh.end());
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_DILATION_FILTER_HXX */
