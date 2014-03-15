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

#include "SliceWindow.hpp"
#include "SliceWindowManager.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      namespace {
	static const float epsilon = 0.0001f;
      }

      SliceWindow::~SliceWindow()
      {
	SliceWindowManager::instance().removeWindow(this);
      }

      SliceWindow::SliceWindow(QString name)
	: m_level(0.5f),
	  m_name(name),
	  m_refCounter(0)
      {
      }

      void
      SliceWindow::setLevel(float level)
      {
	if (level < 0)
	  level = 0;
	if (level > 1.0f-epsilon)
	  level = 1.0f-epsilon;
	m_level = level;

	changed();
      }

      float
      SliceWindow::getLevel() const
      {
	return m_level;
      }

      void
      SliceWindow::reset()
      {
	m_level = 0.5f;
	changed();
      }

      QString
      SliceWindow::getName() const
      {
	return m_name;
      }

    }
  }
}
