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

#ifndef SD_IMAGE_PROCESSING_REGION_GROWING_HPP
#define SD_IMAGE_PROCESSING_REGION_GROWING_HPP

#include <Types.hpp>
#include <core/image/ImageView_.hpp>

#include <tuple>
#include <set>

namespace sd {
  
  namespace imageprocessing {

    typedef std::tuple<UINT16, UINT16>         FloodFill2dCoord;
    typedef std::tuple<UINT16, UINT16, UINT16> FloodFill3dCoord;

    typedef std::set<FloodFill2dCoord> FloodFill2dSet;
    typedef std::set<FloodFill3dCoord> FloodFill3dSet;

    // available types:
    //   floating points: float, double
    //   integrals: INT8, UINT8, INT16, UINT16, INT32, UINT32
    template<typename T>
    class RegionGrowing {

    public :

      RegionGrowing();
      ~RegionGrowing();

      void floodFill2d(const sd::core::ImageView_<T>& iv,
		       sd::Point seed, float inf, float sup,
		       FloodFill2dSet& ffset,
		       bool connexity8 = false);

      void floodFill3d(const sd::core::ImageView_<T>& iv,
		       sd::Point seed, float inf, float sup,
		       FloodFill3dSet& ffset,
		       bool connexity26 = false);

    private:

      RegionGrowing(const RegionGrowing&) = delete;
      RegionGrowing& operator=(const RegionGrowing&) = delete;

    };

  }
  
}

#endif /* #! SD_IMAGE_PROCESSING_REGION_GROWING_HPP */
