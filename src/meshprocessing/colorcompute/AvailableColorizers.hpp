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

#ifndef SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_COLORIZERS_HPP
#define SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_COLORIZERS_HPP

#include <meshprocessing/AvailableToolsManager.hpp>

// ColorComputers
#include <meshprocessing/colorcompute/Color2HSVColorsComputer.hpp>
#include <meshprocessing/colorcompute/ColorBlueRedColorsComputer.hpp>
#include <meshprocessing/colorcompute/ColorBlueWhiteRedColorsComputer.hpp>
#include <meshprocessing/colorcompute/ColorDoubleThresholdColorsComputer.hpp>
#include <meshprocessing/colorcompute/ColorHueVariationColorsComputer.hpp>
#include <meshprocessing/colorcompute/ColorGrayLevelColorsComputer.hpp>

namespace sd {

  namespace meshprocessing {

    namespace colorcompute {
      
      class AvailableColorizers : public sd::meshprocessing::AvailableToolsManager<sd::meshprocessing::colorcompute::ColorComputer> {
  
      public:
  
	AvailableColorizers() : sd::meshprocessing::AvailableToolsManager<sd::meshprocessing::colorcompute::ColorComputer>() {
	  this->load();
	  this->loadNames();
	}
  
	virtual ~AvailableColorizers() {}
 
      protected:
	  
	virtual void load() {
	  this->data() = {new sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer(),
			  // new sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer(),
			  new sd::meshprocessing::colorcompute::ColorBlueWhiteRedColorsComputer(),
			  // new sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer(),
			  new sd::meshprocessing::colorcompute::ColorGrayLevelColorsComputer(),
			  // new sd::meshprocessing::colorcompute::Color2HSVColorsComputer(),
	  };
	}
	
      };

    }

  }

}

#endif /*! SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_COLORIZERS_HPP */
