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

#ifndef SD_QTGUI_GRAPHIC_VIEW_BLOCKWIDGET_HPP
#define SD_QTGUI_GRAPHIC_VIEW_BLOCKWIDGET_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
RESTORE_CONTEXT()

#include "Widget.hpp"
#include <frontend/lib/TypesInfo.hpp>

#include <cstdio>
#include <vector>

namespace sd {
  namespace qtgui {

    class GraphicViewsManager;

    namespace graphicView {

      class BlockWidget
	: public QWidget
      {
	Q_OBJECT

	public:

	enum LayoutType { BlockLayout1, BlockLayout4,
			  BlockLayout2h, BlockLayout2v,
			  BlockLayout1_2h, BlockLayout2_1h,
			  BlockLayout1_2v, BlockLayout2_1v};

	public:

	~BlockWidget();

	void configure(std::initializer_list<Widget*> widgets, LayoutType type);
	void connectGraphicViews(std::initializer_list<Widget*> widgets);

	bool containsWidget(Widget* w);

	const QString& getName() const;
	LayoutType getType() const;

	void maximizeWidget(Widget*);

	std::vector<graphicView::Widget*>& getGraphicViews();
	const std::vector<graphicView::Widget*>& getGraphicViews() const;

	void addWidget(Widget* w);
	void removeWidget(Widget* w);

	Widget* getWidget(size_t index);
	const Widget* getWidget(size_t index) const;

	void setView(frontend::ViewInfo* view);

	void changeLayout(LayoutType type);

	void takeScreenshot();

      private:

	friend class sd::qtgui::GraphicViewsManager;
	BlockWidget(const QString& name);

	void connectScenes(std::initializer_list<int> sceneIds);
	void disconnectScenes();

	void clearLayout();

	void stretchRowsAndColums(int nbRows, int nbCols);

      private:

	std::vector<Widget*> m_graphicViews;
	LayoutType m_type;
	QString m_name;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_BLOCKWIDGET_HPP */
