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

#include "ImageSceneState.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
RESTORE_CONTEXT()

#include "ImageScene.hpp"
#include "Widget.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      namespace {
	static const float nameBorderInPercent = 0.015f;
      }

      ImageSceneState::ImageSceneState(ImageScene& scene, ImageSceneState::Type t, bool picking)
	: SceneState(scene, picking),
	  m_imageScene(scene),
	  m_type(t)
      {
	m_mousePos[0] = m_mousePos[1] = -1;
      }

      ImageSceneState::~ImageSceneState()
      {
      }

      ImageSceneState::Type
      ImageSceneState::getType() const
      {
	return m_type;
      }

      ImageResourcePtr
      ImageSceneState::getResource() const
      {
	return m_imageScene.m_res;
      }

      ImageResourcePtr
      ImageSceneState::getResource(ImageScene& scene) const
      {
	return scene.m_res;
      }

      ViewingWindowPtr
      ImageSceneState::getViewWindow() const
      {
	return m_imageScene.m_viewWindow;
      }

      SliceWindowPtr
      ImageSceneState::getSliceWindow() const
      {
	return m_imageScene.m_sliceWindow;
      }

      void
      ImageSceneState::imageChanged()
      {
      }

      void
      ImageSceneState::drawOverlay(QPainter* painter)
      {
	QRectF rect = m_imageScene.sceneRect();
	QString name;

	if (isPicking()) {
	  if (getResource())
	    name = QString::fromStdString(getResource()->getName());
	}
	else {
	  name = m_imageScene.getWidget().getName();
	  if (getResource() && !getResource()->getName().empty())
	    name += " (" + QString::fromStdString(getResource()->getName()) + ")";
	}

	int textHeight = painter->fontMetrics().height();
	painter->setPen(Qt::black);
	// scale the textHeight since it seems (visual check) that the font metrics
	// return 1.5 the actual printed height
	painter->drawText(QPointF(nameBorderInPercent*rect.width(),
				  nameBorderInPercent*rect.height() + 2/3.0 * textHeight),
			  name);

	auto res = getResource();
	if (res) {
	  float width  = getWidth();
	  float height = getHeight();

	  painter->drawLine(QPointF((1 - 0.015) * width, 0.12 * height),
			    QPointF((1 - 0.045) * width, 0.12 * height));
	  painter->drawLine(QPointF((1 - 0.015) * width, 0.88 * height),
			    QPointF((1 - 0.045) * width, 0.88 * height));
	  painter->drawLine(QPointF((1 - 0.03) * width, 0.12 * height),
			    QPointF((1 - 0.03) * width, 0.88 * height));

	  static const float epsilon = 0.0025f;

	  painter->setPen(Qt::white);
	  float ww = getViewWindow()->getWidth();
	  float wl = getViewWindow()->getLevel();

	  float minHeight = (0.12f + (0.76f-epsilon) * (wl + ww/2.0f)) * height;
	  float maxHeight = (0.12f + (0.76f-epsilon) * (wl - ww/2.0f)) * height;
	  QLinearGradient gradient(QPointF(0, minHeight), QPointF(0, maxHeight));
	  static const QColor gradColTop(0, 0, 0);
	  static const QColor gradColBott(255, 255, 255);
	  gradient.setColorAt(0, gradColTop);
	  gradient.setColorAt(1, gradColBott);
	  QRectF windowRect((1 - 0.04) * width, minHeight, 0.02 * width, maxHeight-minHeight);
	  painter->fillRect(windowRect, gradient);
	}
      }

      bool
      ImageSceneState::mouseDoubleClick(QGraphicsSceneMouseEvent* event)
      {
	if (event->button() & Qt::RightButton) {
	  getViewWindow()->reset();
	  return true;
	}
	return false;
      }

      bool
      ImageSceneState::mouseMove(QGraphicsSceneMouseEvent* event)
      {
	m_mousePos[0] = event->scenePos().x();
	m_mousePos[1] = event->scenePos().y();

	if (isPicking())
	  m_imageScene.update();

	if (event->buttons() & Qt::RightButton) {
	  QPointF newPos  = event->scenePos();
	  QPointF lastPos = event->lastScenePos();
	  float distX = newPos.x() - lastPos.x();
	  float distY = newPos.y() - lastPos.y();

	  static const float wChangeFactor = 1000.0f;

	  if (!(event->modifiers() & Qt::ShiftModifier))
	    getViewWindow()->setWidth(getViewWindow()->getWidth() + distX/wChangeFactor);

	  if (!(event->modifiers() & Qt::ControlModifier))
	    getViewWindow()->setLevel(getViewWindow()->getLevel() + distY/wChangeFactor);

	  return true;
	}
	return false;
      }

      void
      ImageSceneState::adjustBrightnessContrast(std::function<void(void)> displayImage)
      {
	// apply brightness & contrast
	GLfloat scale = getViewWindow()->getWidth();
	GLfloat bias = getViewWindow()->getLevel() - 0.5;

	if (bias != 0.0 || scale != 1.0) {
	  glDisable(GL_TEXTURE_2D);
	  glDisable(GL_DEPTH_TEST);

	  // apply brightness & contrast
	  assert(scale >= 0);
	  // :GLITCH: as currently defined, scale is always <= 1
	  assert(scale <= 1);

	  glEnable(GL_BLEND);
	  if (bias > 0) {
	    glBlendEquation(GL_FUNC_ADD);
	    glColor4f(bias, bias, bias, scale);
	  }
	  else {
	    glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	    glColor4f(-bias, -bias, -bias, scale);
	  }
	  glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	  displayImage();
	  glBlendEquation(GL_FUNC_ADD);
	} // (bias != 0.0 || scale != 1.0)

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }

    }
  }
}
