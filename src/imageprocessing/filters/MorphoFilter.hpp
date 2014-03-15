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

#ifndef SD_IMAGE_PROCESSING_MORPHO_FILTER_HPP
#define SD_IMAGE_PROCESSING_MORPHO_FILTER_HPP

#include "NonLinearFilter.hpp"
#include <core/image/ImageView_.hpp>
#include <vector>

namespace sd {
  namespace imageprocessing {

    class StructuralElement;

    // available types:
    //   floating points: float, double
    //   integrals: INT8, UINT8, INT16, UINT16, INT32, UINT32
    template<typename T>
    class MorphoFilter
      : public NonLinearFilter<T>
    {

    public:

      MorphoFilter(const StructuralElement& element);
      virtual ~MorphoFilter();

      // NOTE: SLICE BY SLICE 2D FILTERING ONLY
      virtual bool applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src);

      virtual T processNeighborhood(std::vector<T>& neigh) = 0;

    protected:

      // use a rectangular cuboid structural element (each kernel point is set to 1)
      MorphoFilter(size_t width, size_t height, size_t depth);

    private:

      MorphoFilter(const MorphoFilter& f) = delete;
      MorphoFilter& operator=(const MorphoFilter& f) = delete;

    };

  }
}

#endif /* #! SD_IMAGE_PROCESSING_MORPHO_FILTER_HPP */
