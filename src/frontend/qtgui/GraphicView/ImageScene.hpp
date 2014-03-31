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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGESCENE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGESCENE_HPP

#include <memory>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>
#include <QGraphicsSceneMouseEvent>
RESTORE_CONTEXT()

#include <Types.hpp>

#include "Gui/ProxyWidget.hpp"
#include "Scene.hpp"
#include "ImageResource.hpp"
#include "SliceWindow.hpp"
#include "ViewingWindow.hpp"

#include "ImageSceneState.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class ImageScene
	: public Scene
      {
	Q_OBJECT

	public:

	ImageScene(ImageSceneState::Type stateType = ImageSceneState::State3dType);
	~ImageScene();

	const ImageSceneState& getState() const;

	Orientation getOrientation() const;
	UINT16 getSliceNumber() const;

	const ViewingWindow* getViewWindow() const;
	const SliceWindow* getSliceWindow() const;

	const ImageSlicePtr getImageSlice() const;

	virtual void drawBackground(QPainter* painter, const QRectF& rect);

      public slots:

	void setStateType(ImageSceneState::Type stateType);

	void setImage(const ImageResourcePtr img);

	void setViewWindow(const ViewingWindowPtr window);
	void setSliceWindow(const SliceWindowPtr window);

	void setSliceNumber(UINT16 number);
	void setOrientation(Orientation ori);

	void setPicking(bool picking);

	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent* event);

      signals:

	void viewWindowSet(graphicView::ViewingWindow* window);
	void sliceWindowSet(graphicView::SliceWindow* window);

      private:

	void configSliceSelector();

	ImageSceneState* newGraphicViewState(ImageSceneState::Type state, bool picking = false);

	virtual void resized(const QRectF& rect);

      private slots:

	void typeSelected(int index);
	void viewWindowUpdated();
	void sliceNumberSelected(int number);
	void sliceNumberEdited();
	void sliceWindowUpdated();
	void rotate90();

      private:

	friend class ImageSceneState;

	ImageResourcePtr m_res;
	ViewingWindowPtr m_viewWindow;
	SliceWindowPtr m_sliceWindow;

	std::unique_ptr<ImageSceneState> m_state;

	QComboBox* m_typeSelector;
	QWidget* m_sliceSelector;
	QSlider* m_sliceSlider;
	QLineEdit* m_sliceNumberEdit;
	QLabel* m_maxSliceNumberLabel;

	QToolButton* m_rotatePos;

	gui::ProxyWidget* m_sliceItem;
	gui::ProxyWidget* m_typeItem;
	gui::ProxyWidget* m_rotateItem;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGESCENE_HPP */
