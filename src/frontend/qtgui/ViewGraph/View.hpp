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

#ifndef SD_QTGUI_VIEWGRAPH_VIEW_HPP
#define SD_QTGUI_VIEWGRAPH_VIEW_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QList>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>
#include <core/ViewId.hpp>

#include "GraphicsItemTypes.hpp"

class QGraphicsSceneMouseEvent;

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      class Arrow;
      class Graph;

      class View
	: public QGraphicsItem
      {

      public:

	enum { Type = NodeGraphicsItemType };

      public:

	View(Graph* graph, frontend::ViewInfoPtr viewInfo, core::ViewId& viewId);
	virtual ~View();

	int type() const;

	const core::ViewId& getId() const;

	virtual frontend::ViewInfo* getViewInfo() const = 0;

	void setDeletable(bool deletable = true);
	bool isDeletable() const;

	void connectArrow(Arrow* arrow);
	const QList<Arrow*>& arrows() const;

	void calculateForces();
	bool advance();

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const = 0;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) = 0;

      protected:

	Graph& getGraph();
	const Graph& getGraph() const;

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

      protected:

	frontend::ViewInfo* const m_viewInfo;

      private:

	QList<Arrow*> m_arrowsList;
	QPointF m_newPos;
	Graph* m_graph;
	core::ViewId& m_viewId;

	frontend::ViewInfoPtr m_viewInfoRefKeeper;

	static float m_xGen, m_yGen;

      };

    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_VIEW_HPP */
