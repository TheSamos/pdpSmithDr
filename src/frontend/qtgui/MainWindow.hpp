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

#ifndef SD_QTGUI_MAINWINDOW_HPP
#define SD_QTGUI_MAINWINDOW_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMainWindow>
#include <QTabWidget>
RESTORE_CONTEXT()

#include "ViewGraph/Graph.hpp"
#include "GraphicView/BlockWidget.hpp"

namespace sd {
  namespace qtgui {

    class GraphDockWidget;

    class MainWindow : public QMainWindow {
      
      Q_OBJECT

      public:

      static void start();
      static void setVisible();
      static void stop();

      static inline MainWindow&
      instance() {
	return *m_inst;
      }
      
    public:

      viewgraph::Graph* getGraph();

      void addBlock(graphicView::BlockWidget* block);
      void removeBlock(graphicView::BlockWidget* block);

      graphicView::BlockWidget* currentBlock();

    private slots:

      void tabChanged(int);

    private:

      MainWindow();
      ~MainWindow();
      MainWindow(const MainWindow&) = delete;
      MainWindow& operator=(const MainWindow&) = delete;

    private:

      static MainWindow* m_inst;

    private:

      GraphDockWidget* m_graphWidget;
      QTabWidget* m_tabWidget;

    };

  }
}

#endif /*! SD_QTGUI_MAINWINDOW_HPP */
