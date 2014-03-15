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

#include "ProxyWidget.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace gui {

	ProxyWidget::ProxyWidget(QWidget* w)
	  : m_anim(NULL)
	{
	  m_anim = new QPropertyAnimation(w, "windowOpacity");
	  m_anim->setDuration(250);
	  m_anim->setStartValue(0);
	  m_anim->setEndValue(0.8);
	  setWidget(w);
	  w->setWindowOpacity(0);
	}

	ProxyWidget::~ProxyWidget()
	{
	  if (m_anim)
	    delete m_anim;
	}

	void
	ProxyWidget::hoverEnterEvent(QGraphicsSceneHoverEvent*)
	{
	  m_anim->start();
	}

	void
	ProxyWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
	{
	  widget()->setWindowOpacity(0);
	  m_anim->stop();
	}

      }
    }
  }
}
