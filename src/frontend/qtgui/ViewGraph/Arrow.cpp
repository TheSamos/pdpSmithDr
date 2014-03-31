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

#include "Arrow.hpp"

#include <cmath>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QPainter>
RESTORE_CONTEXT()

#include "View.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      static const qreal CONST_PI = 3.14159265358979323846264338327950288419717;

      static const qreal arrowSize = 5;

      Arrow::Arrow(View* src, View* dest)
	: m_src(src), m_dest(dest)
      {
	setAcceptedMouseButtons(0);

	m_src->connectArrow(this);
	m_dest->connectArrow(this);

	adjust();
      }

      int
      Arrow::type() const
      {
	return Type;
      }

      View*
      Arrow::source() const
      {
	return m_src;
      }

      View*
      Arrow::destination() const
      {
	return m_dest;
      }

      void
      Arrow::adjust()
      {
	if (!m_src || !m_dest)
	  return;

	QLineF line(mapFromItem(m_src, 0, 0), mapFromItem(m_dest, 0, 0));
	qreal length = line.length();

	prepareGeometryChange();

	if (length > qreal(20)) {
	  QPointF edgeOffset((line.dx()*10)/length, (line.dy()*10)/length);
	  m_srcPt  = line.p1() + edgeOffset;
	  m_destPt = line.p2() - edgeOffset;
	}
	else {
	  m_srcPt = m_destPt = line.p1();
	}
      }

      QRectF
      Arrow::boundingRect() const
      {
	if (!m_src || !m_dest)
	  return QRectF();

	qreal penWidth = 1;
	qreal extra = (penWidth + arrowSize) / 2.0;

	return QRectF(m_srcPt, QSizeF(m_destPt.x() - m_srcPt.x(),
				      m_destPt.y() - m_srcPt.y())
		      ).normalized().adjusted(-extra, -extra, extra, extra);
      }

      void
      Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
      {
	if (!m_src || !m_dest)
	  return;

	QLineF line(m_srcPt, m_destPt);
	if (qFuzzyCompare(line.length(), qreal(0)))
	  return;

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);

	double angle = acos(line.dx() / line.length());
	if (line.dy() >= 0)
	  angle = 2*CONST_PI - angle;

	QPointF destArrowP1 = m_destPt + QPointF(sin(angle - CONST_PI/3) * arrowSize,
						 cos(angle - CONST_PI/3) * arrowSize);
	QPointF destArrowP2 = m_destPt + QPointF(sin(angle - CONST_PI + CONST_PI/3) * arrowSize,
						 cos(angle - CONST_PI + CONST_PI/3) * arrowSize);

	painter->setBrush(Qt::black);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
      }

    }
  }
}
