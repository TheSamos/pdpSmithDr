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

#ifndef SD_QTGUI_GRAPHIC_VIEWESMANAGER_HPP
#define SD_QTGUI_GRAPHIC_VIEWESMANAGER_HPP

#include <list>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QObject>
RESTORE_CONTEXT()

#include "GraphicView/ViewingWindow.hpp"
#include "GraphicView/SliceWindow.hpp"
#include "GraphicView/BlockWidget.hpp"
#include "GraphicView/ImageSceneState.hpp"
#include "GraphicView/MeshSceneState.hpp"

namespace sd {
  namespace qtgui {

    namespace graphicView {
      class Widget;
    }

    class GraphicViewsManager
      : public QObject
    {
      Q_OBJECT

      public:

      static GraphicViewsManager& instance();

      graphicView::Widget* createImageGraphicView(graphicView::ImageSceneState::Type t,
				      graphicView::ViewingWindowPtr* vw = 0,
				      graphicView::SliceWindowPtr* sw = 0,
				      bool picking = false);
      
      graphicView::Widget* createMeshGraphicView(graphicView::MeshSceneState::Rendering r,
				     bool picking = false);
      
      graphicView::BlockWidget* createBlockWidget(frontend::ViewInfo* view = NULL);
      
      void remove(graphicView::Widget*);
      void remove(graphicView::BlockWidget*);

    signals:

      void graphicViewCreated(graphicView::Widget*);
      void graphicViewDeleted(graphicView::Widget*);

    private:

      GraphicViewsManager();
      ~GraphicViewsManager();
      GraphicViewsManager(const GraphicViewsManager&) = delete;
      GraphicViewsManager& operator=(const GraphicViewsManager&) = delete;

      graphicView::BlockWidget* createDefaultImageBlockWidget(const QString&);
      graphicView::BlockWidget* createDefaultMeshBlockWidget(const QString&);

    private:

      std::list<graphicView::Widget*> m_graphicViews;
      std::list<graphicView::BlockWidget*> m_blocks;

      static int m_graphicViewCount;

    };

  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEWESMANAGER_HPP */
