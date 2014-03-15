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

#include "GraphScene.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
RESTORE_CONTEXT()

#include "Graph.hpp"
#include "Views/MeshFile.hpp"
#include "Views/MeshAlgorithmOutput.hpp"
#include "Views/ImageFile.hpp"
#include "Views/ImageAlgorithmOutput.hpp"
#include "../Menus.hpp"
#include "Arrow.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      GraphScene::GraphScene(Graph* widget)
	: QGraphicsScene(widget),
	  m_graph(widget)
      {
      }

      void
      GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* evt)
      {
	QGraphicsItem* item = itemAt(evt->scenePos(), QTransform());

	if (item) {
	  // dispatch to item
	  // (won't do it manually since contextMenuEvent is protected in View)
	  QGraphicsScene::contextMenuEvent(evt);
	}
	else {
	  Menus::instance().getViewsMenu()->popup(evt->screenPos());
	}
      }

      View*
      GraphScene::createMeshFileView(const frontend::FileMeshInfoPtr& mesh)
      {
	views::MeshFile* view = new views::MeshFile(m_graph, mesh);
	addItem(view);
	connectViewtoParents(view, mesh->meshPtr->getPreviousViewNodes());
	return view;
      }

      View*
      GraphScene::createImageFileView(const frontend::FileImageInfoPtr& image)
      {
	views::ImageFile* view = new views::ImageFile(m_graph, image);
	addItem(view);
	connectViewtoParents(view, image->imagePtr->getPreviousViewNodes());
	return view;
      }

      View*
      GraphScene::createMeshAlgorithmView(const frontend::AlgorithmOutputMeshInfoPtr& mesh)
      {
	views::MeshAlgorithmOutput* view = new views::MeshAlgorithmOutput(m_graph, mesh);
	addItem(view);
	connectViewtoParents(view, mesh->meshPtr->getPreviousViewNodes());
	return view;
      }

      View*
      GraphScene::createImageAlgorithmView(const frontend::AlgorithmOutputImageInfoPtr& image)
      {
	views::ImageAlgorithmOutput* view = new views::ImageAlgorithmOutput(m_graph, image);
	addItem(view);
	connectViewtoParents(view, image->imagePtr->getPreviousViewNodes());
	return view;
      }

      void
      GraphScene::removeViewItem(View* view)
      {
	removeItem(view);
	// :TODO: remove arrows
      }

      void
      GraphScene::connectViewtoParents(View* view,
				       const std::vector<core::ViewNode*>& parents)
      {
	auto v = m_graph->getViews();

	for (size_t i = 0; i < parents.size(); ++i) {
	  for (size_t j = 0; j < v.size(); ++j) {
	    if (v[j] != NULL) {
	      if (parents[i]->getId() == v[j]->getId())
		addItem(new Arrow(v[j], view));
	    }
	  }
	}
      }

    }
  }
}
