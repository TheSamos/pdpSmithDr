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

#include "Widget.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QResizeEvent>
#include <QGraphicsTextItem>
#include <QGLWidget>
RESTORE_CONTEXT()

#include "Scene.hpp"

#include "WidgetState.hpp"
#include "../GraphicViewsManager.hpp"
#include "../Menus.hpp"

#include "WidgetStates/ImageWidget.hpp"
#include "WidgetStates/MeshWidget.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      Widget::Widget(const QString& name, Scene::Type type, const Widget* shareWidget)
	: QGraphicsView(),
	  m_name(name),
	  m_state(0),
	  m_view()
      {
	QGLWidget* glShared = 0;
	if (shareWidget)
	  glShared = (QGLWidget*) shareWidget->viewport();

	m_glWidget = new GLWidget(0, glShared);
	setViewport(m_glWidget);
	setViewportUpdateMode(FullViewportUpdate);

	m_glWidget->makeCurrent();
	m_glWidget->initializeGL();

	if (type == Scene::ImageSceneType)
	  m_state = new widget_states::ImageWidget(*this);
	else if (type == Scene::MeshSceneType)
	  m_state = new widget_states::MeshWidget(*this);

	setScene(getScene());

	setMinimumSize(250, 250);
	setContentsMargins(1, 1, 1, 1);

#ifdef DEBUG
	std::cout << "create graphicView: " << name.toStdString() << std::endl;
#endif
      }

      Widget::~Widget()
      {
#ifdef DEBUG
	std::cout << "~Widget: " << m_name.toStdString() << std::endl;
#endif
	GraphicViewsManager::instance().remove(this);
      }

      const QString&
      Widget::getName() const
      {
	return m_name;
      }

      void
      Widget::setName(const QString& name)
      {
	m_name = name;
      }

      Scene*
      Widget::getScene()
      {
	return m_state->getScene();
      }

      const Scene*
      Widget::getScene() const
      {
	return m_state->getScene();
      }

      void
      Widget::takeScreenshot()
      {
	m_glWidget->makeCurrent();
	m_glWidget->updateGL();
	QImage image = m_glWidget->grabFrameBuffer();
	Menus::instance().saveImage(image);
      }

      void
      Widget::takeScreenshot(const QString& fileURI)
      {
	m_glWidget->makeCurrent();
	m_glWidget->updateGL();
	QImage image = m_glWidget->grabFrameBuffer();
	image.save(fileURI);
      }
      
      void
      Widget::setView(frontend::ViewInfo* view)
      {
	m_view = view;
	m_glWidget->makeCurrent();
	graphicView::Scene::Type type = getScene()->getType();
	switch (view->getViewType()) {
	case frontend::ViewInfo::MeshType: {
	  if (type == graphicView::Scene::MeshSceneType)
	    m_state->setView((frontend::MeshInfo*) view);
	  else {
	    m_state = new widget_states::MeshWidget(*this);
	    m_state->setView((frontend::MeshInfo*) view);
	  }
	}
	  break;
	case frontend::ViewInfo::ImageType: {
	  if (type == graphicView::Scene::ImageSceneType)
	    m_state->setView((frontend::ImageInfo*) view);
	  else {
	    m_state = new widget_states::ImageWidget(*this);
	    m_state->setView((frontend::ImageInfo*) view);
	  }
	}
	  break;
	case frontend::ViewInfo::UndefinedType:
	default:
	  assert(!"Should never get here!");
	}
      }

      void
      Widget::resizeEvent(QResizeEvent* evt)
      {
	QSize sz = evt->size();
	getScene()->resize(QRectF(0, 0, sz.width(), sz.height()));
	((GLWidget*) viewport())->resize(sz.width(), sz.height());
	QGraphicsView::resizeEvent(evt);
      }

    }
  }
}
