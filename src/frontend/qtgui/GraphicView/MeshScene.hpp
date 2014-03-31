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

#ifndef SD_QTGUI_GRAPHIC_VIEW_MESHSCENE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_MESHSCENE_HPP

#include <memory>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QGraphicsSceneMouseEvent>
RESTORE_CONTEXT()

#include <Types.hpp>

#include "Gui/ProxyWidget.hpp"
#include "Scene.hpp"
#include "MeshResource.hpp"

#include "MeshSceneState.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class MeshScene : public Scene {
	Q_OBJECT

	public:

	MeshScene(MeshSceneState::Rendering stateRendering = MeshSceneState::FACE_EDGE);
	~MeshScene();

	const MeshSceneState& getState() const;
	MeshSceneState& getState();

	virtual void drawBackground(QPainter* painter, const QRectF& rect);

      public slots:

	void setStateRendering(MeshSceneState::Rendering stateRendering);

	void setMesh(const MeshResourcePtr mesh);

	void setPicking(bool picking);

	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent* event);

      private:

	//MeshSceneState* newGraphicViewState(MeshSceneState::Rendering state, bool picking = false);
	virtual void resized(const QRectF& rect);

      private slots:

	void renderingSelected(int index);
	void setUnsetUnitCube();

      private:

	friend class MeshSceneState;

	MeshResourcePtr m_res;

	std::unique_ptr<MeshSceneState> m_state;

	QComboBox* m_renderingSelector;
	QPushButton* m_activBoxBt;

	gui::ProxyWidget* m_renderingItem;
	gui::ProxyWidget* m_activBoxBtItem;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_MESHSCENE_HPP */
