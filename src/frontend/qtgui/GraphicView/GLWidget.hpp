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

#ifndef SD_QTGUI_GRAPHIC_VIEW_GLWIDGET_HPP
#define SD_QTGUI_GRAPHIC_VIEW_GLWIDGET_HPP

#include <GL/glew.h> // must be included before any QtOpenGL header

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGLWidget>
RESTORE_CONTEXT()

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class GLWidget
	: public QGLWidget
      {
	Q_OBJECT

	public:

	GLWidget(QWidget* parent = 0, const QGLWidget* shareWidget = 0);
	virtual ~GLWidget();

	void resize(int width, int height);

	void initializeGL();

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_GLWIDGET_HPP */
