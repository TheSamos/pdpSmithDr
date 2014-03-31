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

#ifndef SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOWMANAGER_HPP
#define SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOWMANAGER_HPP

#include <set>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QObject>
RESTORE_CONTEXT()

#include "ViewingWindow.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class ViewingWindowManager
	: public QObject
      {
	Q_OBJECT

	public:

	static inline ViewingWindowManager&
	instance()
	{
	  static ViewingWindowManager inst;
	  return inst;
	}

	ViewingWindowPtr getNewWindow();

      signals:

	void windowCreated(graphicView::ViewingWindow*);
	void windowDeleted(graphicView::ViewingWindow*);

      private:

	friend class ViewingWindow;

	ViewingWindowManager();
	~ViewingWindowManager();
	ViewingWindowManager(const ViewingWindowManager&) = delete;
	ViewingWindowManager& operator=(const ViewingWindowManager&) = delete;

	void removeWindow(ViewingWindow* self);

      private:

	unsigned int m_nameGenerator;
	std::set<const ViewingWindow*> m_viewWindows;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOWMANAGER_HPP */
