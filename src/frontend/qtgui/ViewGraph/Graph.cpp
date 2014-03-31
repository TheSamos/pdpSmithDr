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

#include "Graph.hpp"

#include <cmath>
#include <functional>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QKeyEvent>
#include <QWheelEvent>
RESTORE_CONTEXT()

#include "Arrow.hpp"
#include "Views/ImageFile.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      Graph::Graph(QWidget* p)
	: QGraphicsView(p),
	  m_timerId(0)
      {
	GraphScene* scene_ = new GraphScene(this);
	scene_->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene_);

	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	setContextMenuPolicy(Qt::DefaultContextMenu);

	setMinimumSize(155, 155);

	m_viewCreationCallbackHandle = frontend::ViewManager::instance().registerViewCreationCallback(std::bind(&Graph::viewCreated, this, std::placeholders::_1));
	m_viewDeletionCallbackHandle = frontend::ViewManager::instance().registerViewDeletionCallback(std::bind(&Graph::viewDeleted, this, std::placeholders::_1));
	m_viewModificationCallbackHandle = frontend::ViewManager::instance().registerViewModificationCallback(std::bind(&Graph::viewModified, this, std::placeholders::_1));
      }

      Graph::~Graph()
      {
	// Avoid unwanted behavior where ViewManager being a static object
	frontend::ViewManager::instance().unregisterViewCreationCallback(m_viewCreationCallbackHandle);
	frontend::ViewManager::instance().unregisterViewDeletionCallback(m_viewDeletionCallbackHandle);
	frontend::ViewManager::instance().unregisterViewModificationCallback(m_viewModificationCallbackHandle);
      }

      void
      Graph::setDeletable(frontend::ViewInfo* view, bool deletable)
      {
	m_viewInfoToItem[view]->setDeletable(deletable);
	getScene().update();
      }

      void
      Graph::itemMoved()
      {
	if (!m_timerId) {
	  m_timerId = startTimer(1000/25);
	}
      }

      void
      Graph::keyPressEvent(QKeyEvent* evt)
      {
	switch (evt->key()) {
	case Qt::Key_Plus:
	  zoomIn();
	  break;
	case Qt::Key_Minus:
	  zoomOut();
	  break;
	default:
	  QGraphicsView::keyPressEvent(evt);
	  break;
	}
      }

      void
      Graph::timerEvent(QTimerEvent*)
      {
	QList<View*> views;
	foreach (QGraphicsItem* item, scene()->items()) {
	  if (View* view = qgraphicsitem_cast<View*>(item)) {
	    views << view;
	  }
	}

	foreach (View* view, views) {
	  view->calculateForces();
	}

	bool itemsMoved = false;
	foreach (View* view, views) {
	  if (view->advance())
	    itemsMoved = true;
	}

	if (!itemsMoved) {
	  killTimer(m_timerId);
	  m_timerId = 0;
	}
      }

      void
      Graph::wheelEvent(QWheelEvent* evt)
      {
	scaleView(pow(2.0, evt->delta()/240.0));
      }

      void
      Graph::drawBackground(QPainter* painter, const QRectF& r)
      {
	QLinearGradient gradient(r.topLeft(), r.bottomRight());
	gradient.setColorAt(0, Qt::white);
	gradient.setColorAt(1, Qt::darkGray);
	painter->fillRect(r, gradient);
      }

      void
      Graph::scaleView(qreal scaleFactor)
      {
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
	  return;

	scale(scaleFactor, scaleFactor);
      }

      void
      Graph::zoomIn()
      {
	scaleView(qreal(1.2));
      }

      void
      Graph::zoomOut()
      {
	scaleView(1 / qreal(1.2));
      }

      GraphScene&
      Graph::getScene()
      {
	return *((GraphScene*) scene());
      }

      const GraphScene&
      Graph::getScene() const
      {
	return *((const GraphScene*)scene());
      }

      const std::vector<View*>
      Graph::getViews() const
      {
	std::vector<View*> v;
	for (auto it = m_viewInfoToItem.begin(); it != m_viewInfoToItem.end(); ++it)
	  v.push_back((*it).second);
	return v;
      }

      void
      Graph::removeViewItem(View* view)
      {
	getScene().removeViewItem(view);
	delete view;
      }

      void
      Graph::viewCreated(frontend::ViewInfo* view)
      {
	frontend::ViewInfoPtr viewInfo(view);
	switch (view->getViewType()) {
	case frontend::ViewInfo::MeshType: {
	  const frontend::MeshInfoPtr& meshInfo = static_pointer_cast<frontend::MeshInfo>(viewInfo);
	  switch (meshInfo->getOriginType()) {
	  case frontend::MeshInfo::File: {
	    const frontend::FileMeshInfoPtr& fileMeshInfo = static_pointer_cast<frontend::FileMeshInfo>(meshInfo);
	    m_viewInfoToItem[view] = getScene().createMeshFileView(fileMeshInfo);
	  }
	    break;
	  case frontend::MeshInfo::AlgorithmOutput: {
	    const frontend::AlgorithmOutputMeshInfoPtr& algorithmMeshInfo = static_pointer_cast<frontend::AlgorithmOutputMeshInfo>(meshInfo);
	    m_viewInfoToItem[view] = getScene().createMeshAlgorithmView(algorithmMeshInfo);
	  }
	    break;
	  default:
	    assert(!"Should never get here!");
	  }
	}
	  break;
	case frontend::ViewInfo::ImageType: {
	  const frontend::ImageInfoPtr& imageInfo = static_pointer_cast<frontend::ImageInfo>(viewInfo);
	  switch (imageInfo->getOriginType()) {
	  case frontend::ImageInfo::File: {
	    const frontend::FileImageInfoPtr& fileImageInfo = static_pointer_cast<frontend::FileImageInfo>(imageInfo);
	    m_viewInfoToItem[view] = getScene().createImageFileView(fileImageInfo);
	  }
	    break;
	  case frontend::ImageInfo::AlgorithmOutput: {
	    const frontend::AlgorithmOutputImageInfoPtr& algorithmImageInfo = static_pointer_cast<frontend::AlgorithmOutputImageInfo>(imageInfo);
	    m_viewInfoToItem[view] = getScene().createImageAlgorithmView(algorithmImageInfo);
	  }
	    break;
	  default:
	    assert(!"Should never get here!");
	  }
	}
	  break;
	case frontend::ViewInfo::UndefinedType:
	default:
	  assert(!"Should never get here!");
	}
      }

      void
      Graph::viewDeleted(frontend::ViewInfo* viewInfo)
      {
	removeViewItem(m_viewInfoToItem[viewInfo]);
	m_viewInfoToItem.erase(viewInfo);
      }

      void
      Graph::viewModified(frontend::ViewInfo*)
      {
	update();
      }

    }
  }
}
