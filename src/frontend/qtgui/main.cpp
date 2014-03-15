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

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QPixmap>
RESTORE_CONTEXT()

#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/libqt/ThreadSleeper.hpp>
#include "MainWindow.hpp"

#ifndef WIN32
#include <sys/types.h>
#endif

#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/ViewManager.hpp>

int
main(int argc, char* argv[]) {
  QApplication app(argc, argv);
#ifndef DEBUG
  QSplashScreen startSplashScreen(QPixmap(QString(SD_ICON_PATH)+"/SmithDR/SD_splash_screen.png"));
#else
  QSplashScreen startSplashScreen(QPixmap(QString(SD_ICON_PATH)+"/SmithDR/SD_splash_screen_debug.png"));
#endif
  startSplashScreen.show();
  sd::frontend::plugins::PluginManager::loadPlugins(PLUGIN_DIR);
  sd::qtgui::MainWindow::start();
  if (argc > 1) {
    for (int i=1; i<argc; ++i) {
      std::string error;
      startSplashScreen.showMessage(QString::fromUtf8("Loading : ")+argv[i], Qt::AlignRight | Qt::AlignBottom, Qt::white);
      sd::frontend::ViewManager::instance().createView(argv[i], error);
    }
  } else {
    sd::libqt::ThreadSleeper::msleep(1000);  
  }
  startSplashScreen.finish(&sd::qtgui::MainWindow::instance());
  sd::qtgui::MainWindow::setVisible();
  int ret = app.exec();
  sd::qtgui::MainWindow::stop();
  return ret;
}
