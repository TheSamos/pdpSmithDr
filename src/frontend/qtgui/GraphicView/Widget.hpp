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

#ifndef SD_QTGUI_GRAPHIC_VIEW_WIDGET_HPP
#define SD_QTGUI_GRAPHIC_VIEW_WIDGET_HPP

#include <memory>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsView>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>

#include "GLWidget.hpp"
#include "ViewingWindow.hpp"

#include "Scene.hpp"

namespace sd {
  namespace qtgui {

    class GraphicViewsManager;

    namespace graphicView {

      class WidgetState;

      class Widget
	: public QGraphicsView
      {
	Q_OBJECT

	public:

	virtual ~Widget();

	const QString& getName() const;

	Scene* getScene();
	const Scene* getScene() const;

	void takeScreenshot();
	
	void takeScreenshot(const QString& fileURI);
	
	void setView(frontend::ViewInfo* view);

      public slots:

	void setName(const QString& name);

      protected:

	virtual void resizeEvent(QResizeEvent* event);

      private:

	friend class sd::qtgui::GraphicViewsManager;
	Widget(const QString& name, Scene::Type type, const Widget* shareWidget = 0);

      private:

	QString m_name;
	GLWidget* m_glWidget;

	WidgetState* m_state;
	frontend::ViewInfo* m_view;
      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_WIDGET_HPP */
