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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HPP
#define SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HPP

#include <frontend/qtgui/GraphicView/GLLayer.hpp>

#include "ColorComputer.hpp"

namespace sd {

  namespace meshprocessing {
    
    namespace colorcompute {
      
      class ColorScaleLayer : public sd::qtgui::graphicView::GLLayer {
	
      public:
	
	inline ColorScaleLayer(ColorComputer* cc);
	
	inline ColorScaleLayer(const ColorScaleLayer& csl);
	
	inline virtual ~ColorScaleLayer();

	inline virtual ColorScaleLayer* clone() const {return new ColorScaleLayer(m_cc->clone());};

	inline virtual void draw(const QRectF& rect, QPainter* painter) const;
	
      private:
	
	ColorComputer* m_cc;
	
      };
      
    }
    
  }
  
}

#include "ColorScaleLayer.hxx"

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HPP */
