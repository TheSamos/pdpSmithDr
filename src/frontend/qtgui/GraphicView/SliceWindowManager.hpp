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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SLICEWINDOWMANAGER_HPP
#define SD_QTGUI_GRAPHIC_VIEW_SLICEWINDOWMANAGER_HPP

#include <set>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QObject>
RESTORE_CONTEXT()

#include "SliceWindow.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class SliceWindowManager
	: public QObject
      {
	Q_OBJECT

	public:

	static inline SliceWindowManager&
	instance()
	{
	  static SliceWindowManager inst;
	  return inst;
	}

	SliceWindowPtr getNewWindow();

      signals:

	void windowCreated(graphicView::SliceWindow*);
	void windowDeleted(graphicView::SliceWindow*);

      private:

	SliceWindowManager();
	~SliceWindowManager();
	SliceWindowManager(const SliceWindowManager&) = delete;
	SliceWindowManager& operator=(const SliceWindowManager&) = delete;

	friend class SliceWindow;
	void removeWindow(SliceWindow* self);

      private:

	unsigned int m_nameGenerator;
	std::set<const SliceWindow*> m_sliceWindows;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SLICEWINDOWMANAGER_HPP */
