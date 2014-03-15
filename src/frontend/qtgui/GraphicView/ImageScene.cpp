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

#include "ImageScene.hpp"

#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMessageBox>
#include <QPaintEngine>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QImage>
RESTORE_CONTEXT()

#include "Gui/ComboBox.hpp"

#include "ImageStates/SingleSlice.hpp"
#include "ImageStates/OrientedSlices.hpp"

#include "../Menus.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      inline ImageSceneState*
      ImageScene::newGraphicViewState(ImageSceneState::Type stateType, bool picking)
      {
	ImageSceneState* newState = 0;
	switch (stateType) {
	case ImageSceneState::State2dType: {
	  newState = new image_states::SingleSlice(*this, picking);
	}
	  break;
	case ImageSceneState::State3dType: {
	  newState = new image_states::OrientedSlices(*this, picking);
	}
	  break;
	default:
	  assert(!"Should never get here!");
	}
	return newState;
      }

      ImageScene::ImageScene(ImageSceneState::Type stateType)
	: Scene(ImageSceneType),
	  m_state(newGraphicViewState(stateType))
      {
	m_sliceSlider = new QSlider(Qt::Horizontal);
	m_sliceNumberEdit = new QLineEdit;
	m_sliceSelector = new QWidget;
	m_maxSliceNumberLabel = new QLabel;
	QHBoxLayout* layout = new QHBoxLayout(m_sliceSelector);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_sliceSlider, 1);
	layout->addWidget(m_sliceNumberEdit);
	layout->addWidget(m_maxSliceNumberLabel);

	m_sliceSlider->setMinimum(1);
	connect(m_sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(sliceNumberSelected(int)));
	m_sliceNumberEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	connect(m_sliceNumberEdit, SIGNAL(editingFinished()), this, SLOT(sliceNumberEdited()));

	QWidget* rotateWidget = new QWidget;
	QHBoxLayout* rotateLayout = new QHBoxLayout(rotateWidget);
	rotateLayout->setMargin(0);
	rotateLayout->setContentsMargins(2, 0, 2, 0);
	m_rotatePos = new QToolButton;
	m_rotatePos->setText("+");
	rotateLayout->addWidget(m_rotatePos);
	QToolButton* rotateNeg = new QToolButton;
	rotateNeg->setText("-");
	rotateLayout->addWidget(rotateNeg);
	connect(m_rotatePos, SIGNAL(clicked()), this, SLOT(rotate90()));
	connect(rotateNeg, SIGNAL(clicked()), this, SLOT(rotate90()));

	m_typeSelector = new gui::ComboBox;
	m_typeSelector->addItem("XZ");
	m_typeSelector->addItem("YZ");
	m_typeSelector->addItem("XY");
	m_typeSelector->addItem("3D slices");
	// Set the index before the connection below to avoid unnecessary processing
	m_typeSelector->setCurrentIndex(3);
	connect(m_typeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(typeSelected(int)));

	m_typeItem = new gui::ProxyWidget(m_typeSelector);
	m_typeItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_typeItem);

	m_sliceSelector->setContentsMargins(1, 0, 0, 0);
	m_sliceItem = new gui::ProxyWidget(m_sliceSelector);
	m_sliceItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_sliceItem);
	m_sliceItem->hide();

	m_rotateItem = new gui::ProxyWidget(rotateWidget);
	m_rotateItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_rotateItem);
	m_rotateItem->hide();
      }

      ImageScene::~ImageScene()
      {
	sceneDeleted(this);
      }

      const ImageSceneState&
      ImageScene::getState() const
      {
	return *m_state;
      }

      const ViewingWindow*
      ImageScene::getViewWindow() const
      {
	return m_viewWindow.get();
      }

      const SliceWindow*
      ImageScene::getSliceWindow() const
      {
	return m_sliceWindow.get();
      }

      void
      ImageScene::setStateType(ImageSceneState::Type stateType)
      {
	if (getState().getType() == stateType)
	  return;

	m_state.reset(newGraphicViewState(stateType, m_state->isPicking()));

	if (!m_res || stateType == ImageSceneState::State3dType) {
	  m_sliceItem->hide();
	  m_rotateItem->hide();
	}
	else {
	  m_sliceItem->show();
	  m_rotateItem->show();
	}

	if (m_res)
	  m_layoutItem->show();
	else
	  m_layoutItem->hide();
      }

      void
      ImageScene::setImage(const ImageResourcePtr img)
      {
	if (img) {
	  m_res = img;
	}
	else {
	  m_res.reset();
	}

	if (m_res) {
	  m_state->imageChanged();
	  m_viewWindow->reset();
	  if (getState().getType() == ImageSceneState::State2dType) {
	    configSliceSelector();
	    m_sliceItem->show();
	    m_rotateItem->show();
	  }
	  m_layoutItem->show();
	}
	else {
	  m_sliceItem->hide();
	  m_rotateItem->hide();
	  m_layoutItem->hide();
	}

	update();
      }

      void
      ImageScene::setViewWindow(const ViewingWindowPtr window)
      {
	if (m_viewWindow) {
	  disconnect(m_viewWindow.get(), SIGNAL(changed()));
	}

	m_viewWindow = window;
	connect(m_viewWindow.get(), SIGNAL(changed()), this, SLOT(viewWindowUpdated()));
	viewWindowSet(window.get());

	update();
      }

      void
      ImageScene::viewWindowUpdated()
      {
	updateScene();
      }

      void
      ImageScene::setSliceWindow(const SliceWindowPtr window)
      {
	if (m_sliceWindow) {
	  disconnect(m_sliceWindow.get(), SIGNAL(changed()));
	}

	m_sliceWindow = window;
	connect(m_sliceWindow.get(), SIGNAL(changed()), this, SLOT(sliceWindowUpdated()));
	sliceWindowSet(window.get());

	update();
      }

      Orientation
      ImageScene::getOrientation() const
      {
	assert(getState().getType() == ImageSceneState::State2dType);
	return ((image_states::SingleSlice*) m_state.get())->getOrientation();
      }

      UINT16
      ImageScene::getSliceNumber() const
      {
	assert(getState().getType() == ImageSceneState::State2dType);
	return ((image_states::SingleSlice*) m_state.get())->getSliceNumber();
      }

      const ImageSlicePtr
      ImageScene::getImageSlice() const
      {
	assert(getState().getType() == ImageSceneState::State2dType);
	return ((image_states::SingleSlice*) m_state.get())->getSlice();
      }

      void
      ImageScene::drawBackground(QPainter* painter, const QRectF& rect)
      {
	Scene::drawBackground(painter, rect);

	painter->save();
	painter->beginNativePainting();

	if (m_res)
	  m_state->glDraw();

	painter->endNativePainting();
	painter->restore();

	m_state->drawOverlay(painter);
      }

      void
      ImageScene::setSliceNumber(UINT16 number)
      {
	if (m_res && m_state->getType() == ImageSceneState::State2dType) {
	  UINT16 size = 0;
	  switch (((image_states::SingleSlice*) m_state.get())->getOrientation()) {
	  case Frontal:
	    size = m_res->getSizeY();
	    break;
	  case Sagittal:
	    size = m_res->getSizeX();
	    break;
	  case Axial:
	    size = m_res->getSizeZ();
	    break;
	  case NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  }
	  m_sliceWindow->setLevel((number - 0.5f)/size);
	}
      }

      void
      ImageScene::setOrientation(Orientation ori)
      {
	if (m_state->getType() == ImageSceneState::State2dType) {
	  switch (ori) {
	  case Frontal:
	    m_typeSelector->setCurrentIndex(0);
	    break;
	  case Sagittal:
	    m_typeSelector->setCurrentIndex(1);
	    break;
	  case Axial:
	    m_typeSelector->setCurrentIndex(2);
	    break;
	  case NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  }
	  // notify connected graphicViews for redraw
	  for (auto it = m_connectedScenes.begin(), end = m_connectedScenes.end(); it != end; ++it) {
	    (*it)->update((*it)->sceneRect());
	  }
	}
      }

      void
      ImageScene::typeSelected(int index)
      {
	auto type = getState().getType();
	if (index < 3) {
	  setStateType(ImageSceneState::State2dType);
	  Orientation ori = (index == 0 ? Frontal : (index == 1 ? Sagittal : Axial));
	  ((image_states::SingleSlice*) m_state.get())->setOrientation(ori);

	  if (m_res)
	    configSliceSelector();
	}
	else {
	  setStateType(ImageSceneState::State3dType);
	}

	if (getState().getType() != type)
	  m_state->imageChanged();
	// notify connected graphicViews for redraw
	for (auto it = m_connectedScenes.begin(), end = m_connectedScenes.end(); it != end; ++it) {
	  (*it)->update((*it)->sceneRect());
	}
      }

      void
      ImageScene::sliceNumberSelected(int number)
      {
	// The slider should only appear in 2D mode, m_state must be in 2d mode
	assert(getState().getType() == ImageSceneState::State2dType);

	setSliceNumber(number);
	m_sliceNumberEdit->setText(QString::number(number));
      }

      void
      ImageScene::sliceNumberEdited()
      {
	UINT16 number = m_sliceNumberEdit->text().toInt();
	m_sliceSlider->setValue(number);
      }

      void
      ImageScene::configSliceSelector()
      {
	assert(getState().getType() == ImageSceneState::State2dType);
	assert(m_res);

	UINT16 max = 0;
	switch (((image_states::SingleSlice*) m_state.get())->getOrientation()) {
	case Frontal:
	  max = m_res->getSizeY();
	  break;
	case Sagittal:
	  max = m_res->getSizeX();
	  break;
	case Axial:
	  max = m_res->getSizeZ();
	  break;
	case NO_AXIS:
	default:
	  assert(!"Should never get here!");
	}
	m_sliceSlider->setValue(1);
	m_sliceSlider->setMaximum(max);
	m_maxSliceNumberLabel->setText("/ "+QString::number(max)+" ");

	m_sliceNumberEdit->setValidator(new QIntValidator(1, max, m_sliceNumberEdit));
	m_sliceNumberEdit->setText(QString::number(1));
	m_sliceNumberEdit->setAlignment(Qt::AlignRight);
	m_sliceNumberEdit->setMaximumWidth(m_sliceNumberEdit->fontMetrics().width(QString::number(max)+"   "));
      }

      void
      ImageScene::resized(const QRectF& rect)
      {
	setSceneRect(rect);
	m_typeItem->setPos(0.015*rect.width(), 0.985*rect.height() - m_typeItem->size().height());

	// 0.015 on each size of the graphicView and 0.015 between type and slice widgets
	m_sliceSelector->setMinimumWidth(0.955*rect.width() - m_typeItem->size().width());
	m_sliceSelector->setMaximumWidth(0.955*rect.width() - m_typeItem->size().width());
	m_sliceItem->setPos(0.985*rect.width()  - m_sliceItem->size().width(), 0.985*rect.height() - m_sliceItem->size().height());
	m_rotateItem->setPos((rect.width() - m_rotateItem->size().width())/2, 0.015 * rect.height());
	m_layoutItem->setPos((rect.width() - m_layoutItem->size().width()), 0.015 * rect.height());
      }

      void
      ImageScene::setPicking(bool picking)
      {
	if (picking) {
	  m_typeSelector->removeItem(3);
	}
	else if (m_state->isPicking()) {
	  m_typeSelector->addItem("3D slices");
	}
	m_state->setPicking(picking);
      }

      void
      ImageScene::sliceWindowUpdated()
      {
	if (m_res && m_state->getType() == ImageSceneState::State2dType) {
	  UINT16 size = 0;
	  switch (((image_states::SingleSlice*) m_state.get())->getOrientation()) {
	  case Frontal:
	    size = m_res->getSizeY();
	    break;
	  case Sagittal:
	    size = m_res->getSizeX();
	    break;
	  case Axial:
	    size = m_res->getSizeZ();
	    break;
	  case NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  }
	  int number = (int) floor(m_sliceWindow->getLevel() * size) + 1;
	  assert(number > 0 && number <= size);

	  ((image_states::SingleSlice*) m_state.get())->setSliceNumber(number);

	  // notify connected graphicViews for redraw
	  for (auto it = m_connectedScenes.begin(), end = m_connectedScenes.end(); it != end; ++it) {
	    (*it)->updateScene();
	  }
	}
      }

      void
      ImageScene::rotate90()
      {
	assert(getState().getType() == ImageSceneState::State2dType);

	QToolButton* s = (QToolButton*) sender();
	if (s == m_rotatePos) {
	  ((image_states::SingleSlice*) m_state.get())->rotate90(-1);
	}
	else {
	  ((image_states::SingleSlice*) m_state.get())->rotate90(1);
	}
      }

      void
      ImageScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseDoubleClickEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseDoubleClick(evt))
	    update();
	}
      }

      void
      ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mousePressEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mousePress(evt))
	    update();
	}
      }

      void
      ImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseReleaseEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseRelease(evt))
	    update();
	}
      }

      void
      ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent* evt)
      {
	QGraphicsScene::mouseMoveEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->mouseMove(evt))
	    update();
	}
      }

      void
      ImageScene::wheelEvent(QGraphicsSceneWheelEvent* evt)
      {
	QGraphicsScene::wheelEvent(evt);
	if (!evt->isAccepted()) {
	  if (m_state->wheelEvent(evt))
	    update();
	}
      }

    }
  }
}
