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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SCENE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_SCENE_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsScene>
#include <QToolButton>
RESTORE_CONTEXT()

#include <Types.hpp>
#include "Gui/ProxyWidget.hpp"

#include <set>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class Widget;

      class Scene
	: public QGraphicsScene
      {
	Q_OBJECT

      public:

	enum Type { ImageSceneType, MeshSceneType };

      public:

	Type getType() const;

	Widget& getWidget();
	const Widget& getWidget() const;

	const std::set<Scene*>& getConnectedScenes() const;

	float getWidth() const;
	float getHeight() const;

	void resize(const QRectF& rect);
	void updateScene();

	virtual void drawBackground(QPainter* painter, const QRectF& rect);
	virtual void drawForeground(QPainter* painter, const QRectF& rect);

	void emitPicked(float x, float y, float z) const;

      public slots:

	void connectScene(Scene* other);
	void disconnectScene(Scene* other);

      signals:

	void sceneDeleted(Scene* self);
	void picked(float x, float y, float z) const;

      protected slots:

	void changeLayout();

      protected:

	Scene(Type t);
	virtual ~Scene() = 0;

	virtual void resized(const QRectF& rect) = 0;

      protected:

	std::set<Scene*> m_connectedScenes;
	gui::ProxyWidget* m_layoutItem;

      private:

	const Type m_type;
	bool m_maximized;
	QToolButton* m_layoutButton;
	QIcon m_maximizeIcon;
	QIcon m_minimizeIcon;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SCENE_HPP */
