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

#ifndef SD_QTGUI_VIEWGRAPH_VIEWS_RECTANGLE_HPP
#define SD_QTGUI_VIEWGRAPH_VIEWS_RECTANGLE_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QColor>
#include <QPainter>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>
#include <core/ViewId.hpp>

#include "../View.hpp"

namespace sd {
  namespace qtgui {
    namespace viewgraph {
      namespace views {

	class Rectangle
	  : public View
	{

	public:

	public:

	  Rectangle(Graph* graph, frontend::ViewInfoPtr viewInfo, core::ViewId& viewId,
		    bool rounded, const QColor& color1, const QColor& color2,
		    qreal margin, qreal radius);

	  QPainterPath shape() const;
	  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

	private:

	  bool m_rounded;
	  QColor m_color1;
	  QColor m_color2;
	  qreal m_margin;
	  qreal m_radius;

	};

      }
    }
  }
}

#endif /*! SD_QTGUI_VIEWGRAPH_VIEWS_RECTANGLE_HPP */
