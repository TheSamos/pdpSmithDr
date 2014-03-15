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

#ifndef SD_QTGUI_GRAPHIC_VIEW_GUI_COMBOBOX_HPP
#define SD_QTGUI_GRAPHIC_VIEW_GUI_COMBOBOX_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QAbstractItemView>
#include <QComboBox>
RESTORE_CONTEXT()

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace gui {

	class ComboBox
	  : public QComboBox
	{

	public:

	  virtual void
	  showPopup()
	  {
	    QComboBox::showPopup();
	    auto xPos = view()->window()->x();
	    auto yPos = view()->window()->y();
	    auto h = view()->window()->height();
	    auto yAdjust = currentIndex() * view()->height() / count();
	    view()->window()->move(xPos, yPos - h + yAdjust);
	  }

	};

      }
    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_GUI_COMBOBOX_HPP */
