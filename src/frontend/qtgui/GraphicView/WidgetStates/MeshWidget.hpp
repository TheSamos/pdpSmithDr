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

#ifndef SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATES_MESHWIDGET_HPP
#define SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATES_MESHWIDGET_HPP

#include "../WidgetState.hpp"
#include "../MeshScene.hpp"
#include <frontend/lib/TypesInfo.hpp>

#include <SmithDRDefs.hpp>
#include <memory>

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace widget_states {

	class MeshWidget
	  : public WidgetState
	{

	public:

	  MeshWidget(Widget&);

	  virtual ~MeshWidget();

	  virtual Scene* getScene();
	  virtual const Scene* getScene() const;

	  virtual void setView(frontend::ViewInfo* view);

	private:

	  std::unique_ptr<MeshScene> m_meshScene;

	};

      }
    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_WIDGETSTATES_MESHWIDGET_HPP */
