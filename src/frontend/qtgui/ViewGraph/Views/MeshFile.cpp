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

#include "MeshFile.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {
      namespace views {

	MeshFile::MeshFile(Graph* graph, const frontend::FileMeshInfoPtr& info)
	  : Rectangle(graph, info, info->meshPtr->getId(), false,
		      QColor(210, 192, 192),
		      QColor(142, 128, 128),
		      3, 4),
	    m_meshInfo(info.get())
	{
	}

	frontend::ViewInfo*
	MeshFile::getViewInfo() const
	{
	  return m_meshInfo;
	}

      }
    }
  }
}
