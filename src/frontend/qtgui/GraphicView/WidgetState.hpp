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

#ifndef SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATE_HPP

#include "Widget.hpp"
#include "Scene.hpp"

#include <frontend/lib/TypesInfo.hpp>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class WidgetState
      {

      public:

	virtual ~WidgetState() = 0;

	virtual Scene* getScene() = 0;
	virtual const Scene* getScene() const = 0;

	virtual void setView(frontend::ViewInfo* view) = 0;

      protected:

	WidgetState(Widget&);

      protected:

	Widget& m_widget;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATE_HPP */
