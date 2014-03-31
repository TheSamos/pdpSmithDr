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

#ifndef SD_QTGUI_VIEWGRAPH_GRAPHSCENE_HPP
#define SD_QTGUI_VIEWGRAPH_GRAPHSCENE_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsScene>
RESTORE_CONTEXT()

#include <core/ViewNode.hpp>
#include <frontend/lib/TypesInfo.hpp>
#include "View.hpp"

#include <vector>

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      class Graph;

      class GraphScene
	: public QGraphicsScene
      {

      public:

	GraphScene(Graph* widget);

	View* createMeshFileView(const frontend::FileMeshInfoPtr& mesh);
	View* createImageFileView(const frontend::FileImageInfoPtr& image);

	View* createMeshAlgorithmView(const frontend::AlgorithmOutputMeshInfoPtr& mesh);
	View* createImageAlgorithmView(const frontend::AlgorithmOutputImageInfoPtr& image);

	void removeViewItem(View* view);

      protected:

	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

      private:

	void connectViewtoParents(View* view, const std::vector<core::ViewNode*>& parents);
      private:

	Graph* m_graph;

      };

    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_GRAPHSCENE_HPP */
