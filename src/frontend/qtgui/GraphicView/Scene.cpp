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

#include "Scene.hpp"
#include "Widget.hpp"
#include "../MainWindow.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      Scene::Scene(Scene::Type sceneType)
	: m_type(sceneType), m_maximized(false)
      {
	QSize sz(20, 20);
	static QPixmap maxiPix = QPixmap(QString(SD_ICON_PATH)+"/layouts/maximize.svg").scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_maximizeIcon = QIcon(maxiPix);
	static QPixmap miniPix = QPixmap(QString(SD_ICON_PATH)+"/layouts/minimize.svg").scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_minimizeIcon = QIcon(miniPix);

	m_layoutButton = new QToolButton;
	m_layoutButton->setFixedSize(sz);
	m_layoutButton->setIcon(m_maximizeIcon);
	connect(m_layoutButton, SIGNAL(clicked()), this, SLOT(changeLayout()));
	
	m_layoutItem = new gui::ProxyWidget(m_layoutButton);
	m_layoutItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	addItem(m_layoutItem);
	m_layoutItem->hide();
      }

      Scene::~Scene()
      {
      }

      Scene::Type
      Scene::getType() const
      {
	return m_type;
      }

      float
      Scene::getWidth() const
      {
	return sceneRect().width();
      }

      float
      Scene::getHeight() const
      {
	return sceneRect().height();
      }

      void
      Scene::resize(const QRectF& rect)
      {
	resized(rect);
      }

      Widget&
      Scene::getWidget()
      {
	return *((Widget*) views().front());
      }

      const Widget&
      Scene::getWidget() const
      {
	return *((Widget*) views().front());
      }

      const std::set<Scene*>&
      Scene::getConnectedScenes() const
      {
	return m_connectedScenes;
      }

      void
      Scene::updateScene()
      {
	update();
      }

      void
      Scene::connectScene(Scene* other)
      {
	if (m_connectedScenes.insert(other).second) { // if new element inserted
	  connect(other, SIGNAL(sceneDeleted(Scene*)), this, SLOT(disconnectScene(Scene*)));
	}
      }

      void
      Scene::disconnectScene(Scene* other)
      {
	m_connectedScenes.erase(other);
	disconnect(other, SIGNAL(sceneDeleted(Scene*)));
      }

      void
      Scene::changeLayout()
      {
	auto block = MainWindow::instance().currentBlock();
	if (!block)
	  return;

	static BlockWidget::LayoutType type = block->getType();
	m_maximized = !m_maximized;

	if (m_maximized) {
	  type = block->getType();

	  m_layoutButton->setIcon(m_minimizeIcon);
	  block->maximizeWidget(&getWidget());
	}
	else {
	  m_layoutButton->setIcon(m_maximizeIcon);
	  block->changeLayout(type);
	}
      }

      void
      Scene::drawBackground(QPainter* painter, const QRectF& rect)
      {
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QLinearGradient gradient(QPointF(0, rect.topLeft().y()), QPointF(0, rect.bottomRight().y()));
	static const QColor gradColTopLeft(192, 192, 192);
	static const QColor gradColBotRight(64, 64, 64);
	gradient.setColorAt(0, gradColTopLeft);
	gradient.setColorAt(1, gradColBotRight);
	painter->fillRect(rect, gradient);
      }

      void
      Scene::drawForeground(QPainter*, const QRectF&)
      {
	// Do the overlay in the Backgroud pass so that the GraphicsWidgets are
	// drawn in the overlay
      }

      void
      Scene::emitPicked(float x, float y, float z) const
      {
	picked(x, y, z);
      }

    }
  }
}
