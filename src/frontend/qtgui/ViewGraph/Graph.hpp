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

#ifndef SD_QTGUI_VIEWGRAPH_GRAPH_HPP
#define SD_QTGUI_VIEWGRAPH_GRAPH_HPP

#include <map>
#include <vector>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsView>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/ViewManager.hpp>

#include "GraphScene.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      class View;

      class Graph
	: public QGraphicsView
      {
	Q_OBJECT

	public:

	Graph(QWidget* parent);
	~Graph();

	void itemMoved();

	GraphScene& getScene();
	const GraphScene& getScene() const;

	const std::vector<View*> getViews() const;

      public slots:

	void zoomIn();
	void zoomOut();

	void setDeletable(frontend::ViewInfo* view, bool deletable = true);

      protected:

	void keyPressEvent(QKeyEvent* event);
	void timerEvent(QTimerEvent* event);
	void wheelEvent(QWheelEvent* event);

	void drawBackground(QPainter* painter, const QRectF& rect);

	void scaleView(qreal scaleFactor);

      private:

	void viewCreated(frontend::ViewInfo* viewInfo);
	void viewDeleted(frontend::ViewInfo* viewInfo);
	void viewModified(frontend::ViewInfo* viewInfo);

      private slots:

	void removeViewItem(View* view);

      private:

	int m_timerId;

	std::map<frontend::ViewInfo*, View*> m_viewInfoToItem;

	frontend::ViewManager::CallbackHandle m_viewCreationCallbackHandle;
	frontend::ViewManager::CallbackHandle m_viewDeletionCallbackHandle;
	frontend::ViewManager::CallbackHandle m_viewModificationCallbackHandle;

      };

    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_GRAPH_HPP */
