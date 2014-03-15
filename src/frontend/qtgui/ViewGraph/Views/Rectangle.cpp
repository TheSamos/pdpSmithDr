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

#include "Rectangle.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
RESTORE_CONTEXT()

#ifdef DEBUG
#include <sstream>
#endif

namespace sd {
  namespace qtgui {
    namespace viewgraph {
      namespace views {

	Rectangle::Rectangle(Graph* graph, frontend::ViewInfoPtr viewInfo,
			     core::ViewId& viewId,
			     bool rounded, const QColor& color1, const QColor& color2,
			     qreal margin, qreal radius)
	  : View(graph, viewInfo, viewId),
	    m_rounded(rounded), m_color1(color1), m_color2(color2),
	    m_margin(margin), m_radius(radius)
	{
	}

	QPainterPath
	Rectangle::shape() const
	{
	  QPainterPath p;
#ifdef DEBUG
	  p.addText(0, 0, QFont(), QString::fromStdString(m_viewInfo->getName())
		    + " (" + QString::fromStdString(getId().toString()) + ")");
#else
	  p.addText(0, 0, QFont(), QString::fromStdString(m_viewInfo->getName()));
#endif
	  QRectF rect = p.boundingRect();
	  QPainterPath path;
	  if (m_rounded) {
	    path.addRoundedRect(rect.x()-(m_radius+m_margin)/2,
				rect.y()-(m_radius+m_margin)/2,
				rect.width() +m_radius+m_margin+1,
				rect.height()+m_radius+m_margin+1,
				m_radius, m_radius);
	  }
	  else {
	    path.addRect(rect.x()-(m_radius+m_margin)/2,
			 rect.y()-(m_radius+m_margin)/2,
			 rect.width() +m_radius+m_margin+1,
			 rect.height()+m_radius+m_margin+1);
	  }
	  return path;
	}

	void
	Rectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
	{
	  QRadialGradient gradient(-3, -3, 10);
	  if (option->state & QStyle::State_Sunken) {
	    gradient.setCenter(3, 3);
	    gradient.setFocalPoint(3, 3);
	    gradient.setColorAt(1, m_color1.light(120));
	    gradient.setColorAt(0, m_color2.light(120));
	  }
	  else {
	    gradient.setColorAt(0, m_color1.light(120));
	    gradient.setColorAt(1, m_color2.light(120));
	  }
	  painter->setBrush(gradient);
	  painter->setPen(QPen(Qt::black, 0));

#ifdef DEBUG
	  std::stringstream st;
	  st << getId();
	  QString label = QString::fromStdString(m_viewInfo->getName())
	    + " (" + QString::fromStdString(st.str()) + ")";
#else
	  QString label = QString::fromStdString(m_viewInfo->getName());
#endif
	  QPainterPath p;
	  p.addText(0, 0, QFont(), label);
	  QRectF rect = p.boundingRect();
	  if (m_rounded) {
	    painter->drawRoundedRect(rect.x()-(m_radius+m_margin)/2,
				     rect.y()-(m_radius+m_margin)/2,
				     rect.width() +m_radius+m_margin+1,
				     rect.height()+m_radius+m_margin+1,
				     m_radius, m_radius);
	  }
	  else {
	    painter->drawRect(rect.x()-(m_radius+m_margin)/2,
			      rect.y()-(m_radius+m_margin)/2,
			      rect.width() +m_radius+m_margin+1,
			      rect.height()+m_radius+m_margin+1);
	  }
	  painter->drawText(0, 0, label);
	}

      }
    }
  }
}
