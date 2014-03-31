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

#include "MeshScene.hpp"

#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMessageBox>
#include <QPaintEngine>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QImage>
RESTORE_CONTEXT()

#include "Gui/ComboBox.hpp"
#include "../Menus.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      /*
      inline MeshSceneState*
      MeshScene::newGraphicViewState(MeshSceneState::Type stateType, bool picking)
      {
	MeshSceneState* newState = 0;
	switch (stateType) {
	case MeshSceneState::StateFacesType: {
	  newState = new mesh_states::PolygonFaces(*this, picking);
	}
	  break;
	case MeshSceneState::WireframeType: {
	  newState = new mesh_states::Wireframe(*this, picking);
	}
	  break;
	default:
	  assert(!"Should never get here!");
	}
	return newState;
      }
      */
      
      MeshScene::MeshScene(MeshSceneState::Rendering stateRendering)
	: Scene(MeshSceneType),
	  m_state(new MeshSceneState(*this, stateRendering, false)),
	  m_renderingSelector(new gui::ComboBox),
	  m_activBoxBt(new QPushButton(tr("UnitCube"))) {

	m_renderingSelector->addItem("Dots");
	m_renderingSelector->addItem("Edges");
	m_renderingSelector->addItem("Faces Smooth");
	m_renderingSelector->addItem("Faces");
	m_renderingSelector->addItem("Faces & Edges");
	// Set the index before the connection below to avoid unnecessary processing
	m_renderingSelector->setCurrentIndex(4);
	
	connect(m_renderingSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(renderingSelected(int)));
	
	connect(m_activBoxBt, SIGNAL(clicked()), this, SLOT(setUnsetUnitCube()));
	
	m_renderingItem = new gui::ProxyWidget(m_renderingSelector);
	m_renderingItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_renderingItem);

	m_activBoxBtItem = new gui::ProxyWidget(m_activBoxBt);
	m_activBoxBtItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_activBoxBtItem);
      }

      MeshScene::~MeshScene()
      {
	sceneDeleted(this);
      }

      const MeshSceneState&
      MeshScene::getState() const
      {
	return *m_state;
      }

      MeshSceneState&
      MeshScene::getState()
      {
	return *m_state;
      }

      void
      MeshScene::setStateRendering(MeshSceneState::Rendering stateRendering)
      {
	if (getState().getRendering() == stateRendering)
	  return;

	m_state->changeRendering(stateRendering);
	//m_state.reset(newGraphicViewState(stateRendering, m_state->isPicking()));

	if (m_res)
	  m_layoutItem->show();
	else
	  m_layoutItem->hide();
	//update();
      }

      void
      MeshScene::setMesh(const MeshResourcePtr mesh) {
	if (mesh) {
	  m_res = mesh;
	} else {
	  m_res.reset();
	}

	if (m_res) {
	  m_state->meshChanged();
	  m_layoutItem->show();
	}
	else {
	  m_layoutItem->hide();
	}
	
	if (mesh->getFaces().size() == 0)
	  m_renderingSelector->setCurrentIndex(0);
	else
	  m_renderingSelector->setCurrentIndex(3);
	
	update();
      }

      void
      MeshScene::drawBackground(QPainter* painter, const QRectF& rect)
      {
	Scene::drawBackground(painter, rect);

	painter->save();
	painter->beginNativePainting();

	if (m_res)
	  m_state->glDraw();

	painter->endNativePainting();
	painter->restore();

	m_state->drawOverlay(painter);
      }

      void
      MeshScene::renderingSelected(int index) {
	//auto rendering = getState().getRendering();
	if (index == 0) {
	  setStateRendering(MeshSceneState::DOT);
	} else if (index == 1) {
	  setStateRendering(MeshSceneState::EDGE);
	} else if (index == 2) {
	  setStateRendering(MeshSceneState::FACE_SMOOTH);
	} else if (index == 3) {
	  setStateRendering(MeshSceneState::FACE);
	} else if (index == 4) {
	  setStateRendering(MeshSceneState::FACE_EDGE);
	}
	/*
	if (getState().getRendering() != rendering)
	  m_state->meshChanged();
	*/
	// notify connected graphicViews for redraw
	for (auto it = m_connectedScenes.begin(), end = m_connectedScenes.end(); it != end; ++it) {
	  (*it)->update((*it)->sceneRect());
	}
      }

      void
      MeshScene::setUnsetUnitCube() {
	m_state->viewer().setUnableUnitCube(!m_state->viewer().isActivUnitCube());
      }
      
      void
      MeshScene::resized(const QRectF& rect)
      {
	setSceneRect(rect);
	m_renderingItem->setPos(0.015*rect.width(), 0.985*rect.height() - m_renderingItem->size().height());

	// 0.015 on each size of the graphicView
	m_layoutItem->setPos((rect.width() - m_layoutItem->size().width()), 0.015 * rect.height());
      }

      void
      MeshScene::setPicking(bool picking)
      {
	m_state->setPicking(picking);
      }

      void
      MeshScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseDoubleClickEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseDoubleClick(evt))
	    update();
	}
      }

      void
      MeshScene::mousePressEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mousePressEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mousePress(evt))
	    update();
	}
      }

      void
      MeshScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseReleaseEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseRelease(evt))
	    update();
	}
      }

      void
      MeshScene::mouseMoveEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseMoveEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseMove(evt))
	    update();
	}
      }

      void
      MeshScene::wheelEvent(QGraphicsSceneWheelEvent* evt)
      {
	QGraphicsScene::wheelEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->wheelEvent(evt))
	    update();
	}
      }

    }
  }
}
