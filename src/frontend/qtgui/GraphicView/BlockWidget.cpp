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

#include "BlockWidget.hpp"

#include <cassert>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGridLayout>
RESTORE_CONTEXT()

#include "../GraphicViewsManager.hpp"
#include "../Menus.hpp"

#include "Widget.hpp"
#include "ViewingWindowManager.hpp"
#include "ImageScene.hpp"
#include "MeshScene.hpp"

#include <map>
#include <iostream>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      BlockWidget::BlockWidget(const QString& name)
	: QWidget(),
	  m_graphicViews(),
	  m_type(BlockLayout4),
	  m_name(name)
      {
	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->setMargin(0);
	gridLayout->setSpacing(0);
	setLayout(gridLayout);
      }

      BlockWidget::~BlockWidget()
      {
	GraphicViewsManager::instance().remove(this);
      }

      const QString&
      BlockWidget::getName() const
      {
	return m_name;
      }

      BlockWidget::LayoutType
      BlockWidget::getType() const
      {
	return m_type;
      }

      void
      BlockWidget::configure(std::initializer_list<Widget*> widgets, LayoutType type)
      {
	m_graphicViews = widgets;
	changeLayout(type);
      }

      void
      BlockWidget::connectGraphicViews(std::initializer_list<Widget*> widgets)
      {
	for (auto itW = widgets.begin(); itW != widgets.end(); ++itW) {
	  if (containsWidget(*itW)) {
	    for (auto it = itW+1; it != widgets.end(); ++it) {
	      if (containsWidget(*it)) {
		(*itW)->getScene()->connectScene((*it)->getScene());
		(*it)->getScene()->connectScene((*itW)->getScene());
	      }
	    }
	  }
	}
      }

      bool
      BlockWidget::containsWidget(Widget* w)
      {
	for (auto it = m_graphicViews.begin(); it != m_graphicViews.end(); ++it) {
	  if (*it == w)
	    return true;
	}

	return false;
      }

      void
      BlockWidget::clearLayout()
      {
	if (layout() == NULL)
	  return;

	while (layout()->count() > 0) {
	  QWidget* w = layout()->itemAt(0)->widget();
	  layout()->removeWidget(w);
	  w->setVisible(false);
	}
      }

      void
      BlockWidget::stretchRowsAndColums(int nbRows, int nbCols)
      {
	QGridLayout* gridLayout = (QGridLayout*) layout();

	// stretch rows and cols to have cells of same size
	for (int i = 0; i < nbCols; ++i)
	  gridLayout->setColumnStretch(i, 1);
	for (int i = nbCols; i < gridLayout->columnCount(); ++i)
	  gridLayout->setColumnStretch(i, 0);

	for (int i = 0; i < nbRows; ++i)
	  gridLayout->setRowStretch(i, 1);
	for (int i = nbRows; i < gridLayout->rowCount(); ++i)
	  gridLayout->setRowStretch(i, 0);
      }

      void
      BlockWidget::changeLayout(LayoutType type)
      {
	if (m_graphicViews.empty())
	  return;

	static std::map<LayoutType, std::vector<std::initializer_list<int> > >
	  layouts = { { BlockLayout1, { {0,0} } },
		      { BlockLayout4, { {0,0}, {0,1}, {1,0}, {1,1}} },
		      { BlockLayout2h, { {0,0}, {1,0} } },
		      { BlockLayout2v, { {0,0}, {0,1} } },
		      { BlockLayout1_2h, { {0,0,1,2}, {1,0}, {1,1} } },
		      { BlockLayout2_1h, { {0,0}, {0,1}, {1,0,1,2} } },
		      { BlockLayout1_2v, { {0,0,2,1}, {0,1}, {1,1} } },
		      { BlockLayout2_1v, { {0,0}, {1,0}, {0,1,2,1} } }
	};

	m_type = type;

	QGridLayout* gridLayout = (QGridLayout*) layout();
	//disconnectScenes();
	clearLayout();

	int maxCol = 0;
	int maxRow = 0;
	for (size_t i = 0, graphicViewNo = 0; i < layouts[m_type].size(); ++i, ++graphicViewNo) {
	  size_t n = layouts[m_type][i].size();
	  auto it = layouts[m_type][i].begin();

	  if (i >= m_graphicViews.size()) {
	    // create empty graphicView if needed
	    Widget* w = GraphicViewsManager::instance().createImageGraphicView(ImageSceneState::State2dType);
	    m_graphicViews.push_back(w);
	  }

	  if (n == 2) {
	    maxRow = std::max(maxRow, *it);
	    maxCol = std::max(maxCol, *(it+1));
	    gridLayout->addWidget(m_graphicViews[i], *it, *(it+1));
	  }
	  else if (n == 4) {
	    maxRow = std::max(maxRow, *it + *(it+2)-1);
	    maxCol = std::max(maxCol, *(it+1) + *(it+3)-1);
	    gridLayout->addWidget(m_graphicViews[i], *it, *(it+1), *(it+2), *(it+3));
	  }

	  m_graphicViews[i]->setVisible(true);
	}

	stretchRowsAndColums(maxRow+1, maxCol+1);
	Menus::instance().changeLayout(m_type);
      }

      void
      BlockWidget::connectScenes(std::initializer_list<int> sceneIds)
      {
	for (auto it1 = sceneIds.begin(); it1 != sceneIds.end(); ++it1) {
	  for (auto it2 = it1+1; it2 != sceneIds.end(); ++it2) {
	    m_graphicViews[*it1]->getScene()->connectScene(m_graphicViews[*it2]->getScene());
	    m_graphicViews[*it2]->getScene()->connectScene(m_graphicViews[*it1]->getScene());
	  }
	}
      }

      void
      BlockWidget::disconnectScenes()
      {
	size_t n = m_graphicViews.size();
	for (size_t i = 0; i < n; ++i) {
	  for (size_t j = i+1; j < n; ++j) {
	    m_graphicViews[i]->getScene()->disconnectScene(m_graphicViews[j]->getScene());
	    m_graphicViews[j]->getScene()->disconnectScene(m_graphicViews[i]->getScene());
	  }
	}
      }

      std::vector<graphicView::Widget*>&
      BlockWidget::getGraphicViews()
      {
	return m_graphicViews;
      }

      const std::vector<graphicView::Widget*>&
      BlockWidget::getGraphicViews() const
      {
	return m_graphicViews;
      }

      void
      BlockWidget::setView(frontend::ViewInfo* view)
      {
	for (size_t i = 0; i < m_graphicViews.size(); ++i)
	  m_graphicViews[i]->setView(view);
      }

      void
      BlockWidget::addWidget(Widget* w)
      {
	for (auto it = m_graphicViews.begin(); it != m_graphicViews.end(); ++it) {
	  if (*it == w)
	    return;
	}
	m_graphicViews.push_back(w);
      }

      void
      BlockWidget::removeWidget(Widget* w)
      {
	auto it = m_graphicViews.begin();
	auto itEnd = m_graphicViews.end();
	for ( ; it != itEnd; ++it) {
	  if (*it == w)
	    break;
	}
	if (it != itEnd)
	  m_graphicViews.erase(it);
      }

      Widget*
      BlockWidget::getWidget(size_t index)
      {
	assert(index < m_graphicViews.size());
	return m_graphicViews[index];
      }

      const Widget*
      BlockWidget::getWidget(size_t index) const
      {
	assert(index < m_graphicViews.size());
	return m_graphicViews[index];
      }

      void
      BlockWidget::maximizeWidget(Widget* w)
      {
	bool existingGraphicView = false;
	for (auto it = m_graphicViews.begin(); it != m_graphicViews.end(); ++it) {
	  if (*it == w)
	    existingGraphicView = true;
	}
	if (!existingGraphicView)
	  return;

	clearLayout();
	m_type = BlockLayout1;
	QGridLayout* gridLayout = (QGridLayout*) layout();
	gridLayout->addWidget(w, 0, 0);
	stretchRowsAndColums(1, 1);
	w->setVisible(true);
	Menus::instance().changeLayout(m_type);
      }

      void
      BlockWidget::takeScreenshot()
      {
	QImage image = QPixmap::grabWindow(winId()).toImage();
	Menus::instance().saveImage(image);
      }

    }
  }
}
