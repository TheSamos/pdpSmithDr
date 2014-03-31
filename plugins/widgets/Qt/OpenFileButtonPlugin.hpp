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

#ifndef SD_PLUGINS_WIDGETS_QT_OPEN_FILE_BUTTON_PLUGIN_HPP
#define SD_PLUGINS_WIDGETS_QT_OPEN_FILE_BUTTON_PLUGIN_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
#include <QPushButton>
RESTORE_CONTEXT()

#include <string>

namespace sd {
  namespace plugins {
    namespace widgets {
      namespace qt {

	class OpenFileDialog
	  : public QWidget
	{
	  Q_OBJECT

	  public:

	  OpenFileDialog(const QString& filename, QWidget* parent);

	  std::string getFilename() const;

	signals:

	  void update();

	private slots:

	  void selectFile();

	private:

	  QPushButton* m_button;
	  QWidget* m_parameterizer;

	};

      }
    }
  }
}

#endif /*! SD_PLUGINS_WIDGETS_QT_OPEN_FILE_BUTTON_PLUGIN_HPP */
