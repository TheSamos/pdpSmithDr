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

#include "View.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
RESTORE_CONTEXT()

#include "Arrow.hpp"
#include "Graph.hpp"

#include "../Menus.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      float View::m_xGen, View::m_yGen;

      View::View(Graph* graphWidget, frontend::ViewInfoPtr viewInfoPtr, core::ViewId& viewId)
	: m_viewInfo(viewInfoPtr.get()),
	  m_graph(graphWidget),
	  m_viewId(viewId),
	  m_viewInfoRefKeeper(viewInfoPtr)
      {
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setAcceptedMouseButtons(Qt::LeftButton);
	setZValue(-1);

	setPos(m_xGen, m_yGen);
	m_yGen += 25;
      }

      View::~View() {
      }

      int
      View::type() const
      {
	return Type;
      }

      const core::ViewId&
      View::getId() const
      {
	return m_viewId;
      }

      void
      View::setDeletable(bool deletable)
      {
	if (deletable) {
	  update();
	  m_viewInfoRefKeeper.reset();
	}
	else {
	  update();
	  m_viewInfoRefKeeper = frontend::ViewInfoPtr(m_viewInfo);
	}
      }

      bool
      View::isDeletable() const
      {
	return !m_viewInfoRefKeeper;
      }

      QRectF
      View::boundingRect() const
      {
	return shape().boundingRect();
      }

      void
      View::connectArrow(Arrow* arrow)
      {
	m_arrowsList << arrow;
	arrow->adjust();
      }

      const QList<Arrow*>&
      View::arrows() const
      {
	return m_arrowsList;
      }

      void
      View::calculateForces()
      {
	if (!scene() || scene()->mouseGrabberItem() == this) {
	  m_newPos = pos();
	  return;
	}

	qreal xvel = 0, yvel = 0;
	/*
	  foreach (QGraphicsItem* item, scene()->items()) {
	  View* node = qgraphicsitem_cast<View*>(item);
	  if (!node) {   continue;   }

	  QPointF vec = mapToItem(node, 0, 0);
	  qreal dx = vec.x();
	  qreal dy = vec.y();
	  double len = 2.0 * (dx * dx + dy * dy);
	  if (len > 0) {
	  xvel += (dx * 150.0) / len;
	  yvel += (dy * 150.0) / len;
	  }
	  }

	  double weight = (m_arrowsList.size() + 1) * 10;
	  foreach (Arrow * arrow, m_arrowsList) {
	  QPointF vec;
	  if (arrow->source() == this) {
	  vec = mapToItem(arrow->destination(), 0, 0);
	  } else {
	  vec = mapToItem(arrow->source(), 0, 0);
	  }
	  xvel -= vec.x() / weight;
	  yvel -= vec.y() / weight;
	  }
	*/
	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1) {
	  xvel = yvel = 0;
	}

	QRectF sceneRect = scene()->sceneRect();
	m_newPos = pos() + QPointF(xvel, yvel);
	m_newPos.setX(qMin(qMax(m_newPos.x(), sceneRect.left() + 10),
			   sceneRect.right() - 10));
	m_newPos.setY(qMin(qMax(m_newPos.y(), sceneRect.top() + 10),
			   sceneRect.bottom() - 10));
      }

      bool
      View::advance()
      {
	if (m_newPos == pos()) {
	  return false;
	}
	else {
	  setPos(m_newPos);
	  return true;
	}
      }

      Graph&
      View::getGraph()
      {
	return *m_graph;
      }

      const Graph&
      View::getGraph() const
      {
	return *m_graph;
      }

      QVariant
      View::itemChange(GraphicsItemChange change, const QVariant& value)
      {
	if (change == ItemPositionHasChanged) {
	  foreach (Arrow * arrow, m_arrowsList)
	    arrow->adjust();
	  m_graph->itemMoved();
	}

	return QGraphicsItem::itemChange(change, value);
      }

      void
      View::mousePressEvent(QGraphicsSceneMouseEvent* event)
      {
	update();
	QGraphicsItem::mousePressEvent(event);
      }

      void
      View::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
      {
	update();
	QGraphicsItem::mouseReleaseEvent(event);
      }

      void
      View::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
      {
	Menus::instance().getViewMenu(getViewInfo())->popup(event->screenPos());
      }

    }
  }
}
