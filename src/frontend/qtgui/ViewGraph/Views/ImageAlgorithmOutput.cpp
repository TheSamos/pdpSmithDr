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

#include "ImageAlgorithmOutput.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {
      namespace views {

	ImageAlgorithmOutput::ImageAlgorithmOutput(Graph* g, const frontend::AlgorithmOutputImageInfoPtr& info)
	  : Rectangle(g, info, info->imagePtr->getId(), true,
		      QColor(192, 210, 210),
		      QColor(128, 142, 142),
		      3, 4),
	    m_imageInfo(info.get())
	{
	}

	frontend::ViewInfo*
	ImageAlgorithmOutput::getViewInfo() const
	{
	  return m_imageInfo;
	}

      }
    }
  }
}
