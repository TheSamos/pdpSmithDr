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

#include "MainWindow.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
RESTORE_CONTEXT()

#include "Menus.hpp"
#include "GraphicViewsManager.hpp"
#include "GraphDockWidget.hpp"

// :TODO: fix memory leaks related to Qt objects
// Qt objects automatically delete their children,
// BUT if an object has no parent, it must be deleted by hand!
// --> check all classes...

namespace sd {
  namespace qtgui {

    MainWindow* MainWindow::m_inst = 0;

    MainWindow::MainWindow()
      : QMainWindow()
    {
#ifdef DEBUG
      setWindowTitle("SmithDR-Debug");
#else
      setWindowTitle("SmithDR");
#endif
      setWindowIcon(QIcon(QString(SD_ICON_PATH)+"/SmithDR/SD_icon_64.png"));

      m_graphWidget = new GraphDockWidget(this);
      addDockWidget(Qt::LeftDockWidgetArea, m_graphWidget, Qt::Vertical);

      m_tabWidget = new QTabWidget(this);
      setCentralWidget(m_tabWidget);

      m_tabWidget->setMinimumSize(600, 600);
      connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

      QMenuBar* mainMenu = menuBar();
      mainMenu->addMenu(Menus::instance().getFileMenu());
      mainMenu->addMenu(Menus::instance().getViewsMenu());
      mainMenu->addMenu(Menus::instance().getAlgorithmSelectionMenu());
      mainMenu->addMenu(Menus::instance().getBlocksMenu());
      mainMenu->addMenu(Menus::instance().getGraphicViewsMenu());
    }

    MainWindow::~MainWindow()
    {
    }

    viewgraph::Graph*
    MainWindow::getGraph()
    {
      return m_graphWidget->getGraph();
    }

    void
    MainWindow::addBlock(graphicView::BlockWidget* block)
    {
      m_tabWidget->addTab(block, block->getName());
    }

    void
    MainWindow::removeBlock(graphicView::BlockWidget* block)
    {
      for (int i = 0; i < m_tabWidget->count(); ++i) {
	auto widget = dynamic_cast<graphicView::BlockWidget*>(m_tabWidget->widget(i));
	if (widget == block) {
	  GraphicViewsManager::instance().remove(block);
	  m_tabWidget->removeTab(i);
	  break;
	}
      }
    }

    graphicView::BlockWidget*
    MainWindow::currentBlock()
    {
      if (m_tabWidget->currentIndex() == -1)
	return NULL;

      return (graphicView::BlockWidget*) m_tabWidget->currentWidget();
    }

    void
    MainWindow::tabChanged(int index)
    {
      if (index == -1)
	return;

      auto block = (graphicView::BlockWidget*) m_tabWidget->currentWidget();
      Menus::instance().changeLayout(block->getType());
      Menus::instance().initGraphicViewMenu(block);
    }

    void
    MainWindow::start()
    {
      m_inst = new MainWindow;
    }

    void
    MainWindow::setVisible()
    {
      m_inst->show();
    }
    
    void
    MainWindow::stop()
    {
      m_inst->hide();
      delete m_inst;
    }

  }
}
