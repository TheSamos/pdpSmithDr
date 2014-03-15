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

#ifndef SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOW_HPP
#define SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOW_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QString>
#include <QObject>
RESTORE_CONTEXT()

#include <SmartPointers.hpp>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class ViewingWindow;
      typedef sd::intrusive_ptr<ViewingWindow> ViewingWindowPtr;

      class ViewingWindow
	: public QObject
      {
	Q_OBJECT

	public:

	void setWidth(float width);
	float getWidth() const;

	void setLevel(float level);
	float getLevel() const;

	void reset();

	QString getName() const;

      signals:

	void changed();

      private:

	friend class ViewingWindowManager;
	ViewingWindow(QString name);
	~ViewingWindow();

	friend inline void
	intrusive_ptr_add_ref(ViewingWindow* self)
	{
	  ++self->m_refCounter;
	}

	friend inline void
	intrusive_ptr_release(ViewingWindow* self)
	{
	  if (!--self->m_refCounter)
	    delete self;
	}

      private:

	float m_width;
	float m_level;

	QString m_name;
	size_t m_refCounter;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_VIEWINGWINDOW_HPP */
