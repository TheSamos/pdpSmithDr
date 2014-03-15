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

#include "GLWidget.hpp"

#include <cmath>

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMessageBox>
RESTORE_CONTEXT()

namespace sd {
  namespace qtgui {
    namespace graphicView {

      GLWidget::GLWidget(QWidget* p, const QGLWidget* shareWidget)
	: QGLWidget(p, shareWidget)
      {
      }

      GLWidget::~GLWidget()
      {
      }

      void
      GLWidget::resize(int w, int h)
      {
	if (h == 0)
	  h = 1;
	glViewport(0, 0, w, h);
      }

      void
      GLWidget::initializeGL()
      {
	static bool initialized = false;
	if (initialized == false) {
	  GLenum glewInitOk = glewInit();
	  if (glewInitOk != GLEW_OK) {
	    QMessageBox::critical(this, tr("Initialization error"),
				  tr("GLEW initialization failed"));
	    exit(-1);
	  }
	  initialized = true;
	}
      }

    }
  }
}
