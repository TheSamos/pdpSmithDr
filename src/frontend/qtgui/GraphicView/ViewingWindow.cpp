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

#include "ViewingWindow.hpp"
#include "ViewingWindowManager.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      ViewingWindow::~ViewingWindow()
      {
	ViewingWindowManager::instance().removeWindow(this);
      }

      ViewingWindow::ViewingWindow(QString name)
	: m_width(1.0f),
	  m_level(0.5f),
	  m_name(name),
	  m_refCounter(0)
      {
      }

      void
      ViewingWindow::setWidth(float width)
      {
	if (width > 1.0f)
	  width = 1.0f;
	if (width < 0.0f)
	  width = 0.0f;
	m_width = width;

	changed();
      }

      float
      ViewingWindow::getWidth() const
      {
	return m_width;
      }

      void
      ViewingWindow::setLevel(float level)
      {
	if (level < 0.0f)
	  level = 0.0f;
	if (level > 1.0f)
	  level = 1.0f;
	m_level = level;

	changed();
      }

      float
      ViewingWindow::getLevel() const
      {
	return m_level;
      }

      void
      ViewingWindow::reset()
      {
	m_width = 1.0f;
	m_level = 0.5f;
	changed();
      }

      QString
      ViewingWindow::getName() const
      {
	return m_name;
      }

    }
  }
}
