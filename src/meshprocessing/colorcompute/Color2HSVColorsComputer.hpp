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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HPP
#define SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HPP

#include "ColorComputer.hpp"

#include <core/mesh/MeshView_.hpp>

namespace sd {

  namespace meshprocessing {

    namespace colorcompute {
      
      class Color2HSVColorsComputer : public ColorComputer {
  
      public:  
  
	inline Color2HSVColorsComputer(double min=0., double max=0.,
				       bool centredOnZero = true);
  
	virtual inline std::string name();
  
	virtual inline std::string getDescription();
  
	virtual inline void computeColor(double value,
					 sd::core::RGBCoordinates<unsigned char>& rgb) const;

	virtual inline ColorComputer* clone() const;
    
      };

    }
    
  }
  
}
#include "Color2HSVColorsComputer.hxx"

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HPP */
