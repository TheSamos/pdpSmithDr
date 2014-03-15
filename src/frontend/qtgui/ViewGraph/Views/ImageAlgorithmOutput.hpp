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

#ifndef SD_QTGUI_VIEWGRAPH_VIEWS_IMAGE_ALGORITHM_OUTPUT_HPP
#define SD_QTGUI_VIEWGRAPH_VIEWS_IMAGE_ALGORITHM_OUTPUT_HPP

#include <frontend/lib/TypesInfo.hpp>

#include "Rectangle.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {
      namespace views {

	class ImageAlgorithmOutput
	  : public Rectangle
	{

	public:

	  ImageAlgorithmOutput(Graph* graph, const frontend::AlgorithmOutputImageInfoPtr& info);

	  virtual frontend::ViewInfo* getViewInfo() const;

	private:

	  frontend::AlgorithmOutputImageInfo* const m_imageInfo;

	};

      }
    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_VIEWS_IMAGE_ALGORITHM_OUTPUT_HPP */
