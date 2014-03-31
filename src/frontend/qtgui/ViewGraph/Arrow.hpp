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

#ifndef SD_QTGUI_VIEWGRAPH_ARROW_HPP
#define SD_QTGUI_VIEWGRAPH_ARROW_HPP

#include "GraphicsItemTypes.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {

      class View;

      class Arrow
	: public QGraphicsItem
      {

      public:

	enum { Type = ArrowGraphicsItemType };

      public:

	Arrow(View* src, View* dest);

	int type() const;

	View* source() const;
	View* destination() const;

	void adjust();

      protected:

	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

      private:

	View* m_src;
	View* m_dest;

	QPointF m_srcPt, m_destPt;

      };

    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_ARROW_HPP */
