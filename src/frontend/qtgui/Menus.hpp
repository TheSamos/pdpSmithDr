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

#ifndef SD_QTGUI_MENUS_HPP
#define SD_QTGUI_MENUS_HPP

#include <map>
#include <set>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMenu>
#include <QActionGroup>
#include <QButtonGroup>
#include <QGridLayout>
RESTORE_CONTEXT()

#include <core/mesh/MeshView.hpp>
#include <core/image/ImageView.hpp>

#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/ViewManager.hpp>

#include "AlgorithmAction.hpp"
#include "GraphicView/BlockWidget.hpp"

namespace sd {

  namespace frontend {

    class Algorithm;

  }

  namespace qtgui {

    namespace graphicView {
      class Widget;
      class BlockWidget;
      class Scene;
      class ViewingWindow;
      class SliceWindow;
    }

    class Menus
      : public QObject
    {
      Q_OBJECT

      public:

      static inline Menus&
      instance()
      {
	static Menus inst;
	return inst;
      }

    public:

      QMenu* getFileMenu();

      QMenu* getAlgorithmSelectionMenu();

      QMenu* getViewsMenu();
      QMenu* getViewMenu(const frontend::ViewInfo* view);

      QMenu* getBlocksMenu();

      QMenu* getGraphicViewsMenu();
      QMenu* getGraphicViewMenu(const graphicView::Widget* graphicView);

      void changeLayout(graphicView::BlockWidget::LayoutType type);

      void initGraphicViewMenu(graphicView::BlockWidget*);

    public slots:

      void openFile();
      void openRawImage();
      void saveView(const frontend::ViewInfo* view);
      void saveMesh(const core::MeshView* mesh);
      void saveImage(const core::ImageView* image);
      void saveImage(const QImage& image);

      void graphicViewCreated(graphicView::Widget* graphicView);
      void graphicViewDeleted(graphicView::Widget* graphicView);
      void graphicViewChanged(graphicView::Widget* graphicView);

      void viewWindowCreated(graphicView::ViewingWindow* vw);
      void viewWindowDeleted(graphicView::ViewingWindow* vw);

      void sliceWindowCreated(graphicView::SliceWindow* sw);
      void sliceWindowDeleted(graphicView::SliceWindow* sw);

    signals:

      void enqueueViewCreationSlot(frontend::ViewInfo*);
      void enqueueViewDeletionSlot(frontend::ViewInfo*);

    private:

      Menus();
      ~Menus();
      Menus(const Menus&) = delete;
      Menus& operator=(const Menus&) = delete;

      void viewCreationCallback(frontend::ViewInfo*);
      void viewDeletionCallback(frontend::ViewInfo*);
      void viewModificationCallback(frontend::ViewInfo*);

      void emptySendToMenu(QMenu* menu);

      QMenu* getAlgorithmSelectionSubMenu(std::string path);

      void populate();

      void buildLayoutButton(const QString& name,
			     graphicView::BlockWidget::LayoutType type,
			     QGridLayout* layout, int row, int col,
			     bool pressed = false);

    private slots:

      void createAndAssignViewWindow();
      void viewWindowSelected();
      void viewWindowAssigned(graphicView::ViewingWindow*);

      void createAndAssignSliceWindow();
      void sliceWindowSelected();
      void sliceWindowAssigned(graphicView::SliceWindow*);

      void viewCreationSlot(frontend::ViewInfo*);
      void viewDeletionSlot(frontend::ViewInfo*);

      void saveView();
      void sendViewTo();
      void configureView();
      void setDeletableView(bool);
      void aboutToShowSendTo();

      void takeScreenshot();
      void takeBlockScreenshot();

      void layoutActivated();
      void layoutChanged(bool);

      void quit();

    private:

      QMenu* m_fileMenu;

      QMenu* m_algorithmSelectionMenu;
      typedef std::map<std::string, QMenu*> SubMenus;
      SubMenus m_algorithmSubMenus;

      QMenu* m_blocksMenu;

      QMenu* m_graphicViewsMenu;
      std::map<graphicView::Widget*, QMenu*> m_graphicViewToMenu;
      std::map<QAction*, graphicView::Widget*> m_actionToGraphicView;

      QMenu* m_viewsMenu;
      std::map<frontend::ViewInfo*, QMenu*> m_viewToMenu;
      std::map<frontend::ViewInfo*, QMenu*> m_viewToSendToMenu;
      std::map<QMenu*, frontend::ViewInfo*> m_sendToMenuToView;
      std::map<QAction*, frontend::ViewInfo*> m_configDeleteActionToView;
      std::map<QAction*, frontend::ViewInfo*> m_saveActionToView;
      std::map<QAction*, frontend::ViewInfo*> m_sendToActionToView;
      std::map<QAction*, bool> m_sendToActionToIsBlock;
      std::map<QAction*, graphicView::Widget*> m_sendToActionToGraphicView;
      std::map<QAction*, graphicView::BlockWidget*> m_sendToActionToGraphicViewBlock;

      std::map<graphicView::Scene*, QMenu*> m_graphicViewToViewWindowMenu;
      std::map<graphicView::Scene*, QActionGroup*> m_graphicViewToViewWindowGroup;
      std::map<QAction*, graphicView::ViewingWindow*> m_actionToViewWindow;
      std::map<graphicView::Scene*, QAction*> m_graphicViewToViewSepAction;

      std::map<graphicView::Scene*, QMenu*> m_graphicViewToSliceWindowMenu;
      std::map<graphicView::Scene*, QActionGroup*> m_graphicViewToSliceWindowGroup;
      std::map<QAction*, graphicView::SliceWindow*> m_actionToSliceWindow;
      std::map<graphicView::Scene*, QAction*> m_graphicViewToSliceSepAction;

      std::set<graphicView::ViewingWindow*> m_viewWindows;
      std::set<graphicView::SliceWindow*> m_sliceWindows;

      frontend::ViewManager::CallbackHandle viewCreationCallbackHandle;
      frontend::ViewManager::CallbackHandle viewDeletionCallbackHandle;
      frontend::ViewManager::CallbackHandle viewModificationCallbackHandle;

      QButtonGroup* m_layoutButtonGroup;

    };

  }
}

#endif /*! SD_QTGUI_MENUS_HPP */
