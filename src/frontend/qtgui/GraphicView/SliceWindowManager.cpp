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

#include "SliceWindowManager.hpp"

#include "../Menus.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      SliceWindowManager::SliceWindowManager()
	: m_nameGenerator(0)
      {
	QObject::connect(this, SIGNAL(windowCreated(graphicView::SliceWindow *)),
			 &Menus::instance(), SLOT(sliceWindowCreated(graphicView::SliceWindow *)));
	QObject::connect(this, SIGNAL(windowDeleted(graphicView::SliceWindow *)),
			 &Menus::instance(), SLOT(sliceWindowDeleted(graphicView::SliceWindow *)));
      }

      SliceWindowManager::~SliceWindowManager()
      {
      }

      SliceWindowPtr
      SliceWindowManager::getNewWindow()
      {
	SliceWindow* window = new SliceWindow("Slice Window " + QString::number(m_nameGenerator++));
	m_sliceWindows.insert(window);
	windowCreated(window);
	return SliceWindowPtr(window);
      }

      void
      SliceWindowManager::removeWindow(SliceWindow* self)
      {
	m_sliceWindows.erase(self);
	windowDeleted(self);
      }

    }
  }
}
