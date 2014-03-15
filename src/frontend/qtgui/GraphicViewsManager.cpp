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

#include "GraphicViewsManager.hpp"

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGLContext>
RESTORE_CONTEXT()

#include "GraphicView/Scene.hpp"
#include "GraphicView/ImageScene.hpp"
#include "GraphicView/ImageSceneState.hpp"
#include "GraphicView/MeshScene.hpp"
#include "GraphicView/MeshSceneState.hpp"

#include "GraphicView/ViewingWindowManager.hpp"
#include "GraphicView/SliceWindowManager.hpp"
#include "Menus.hpp"

namespace sd {
  namespace qtgui {

    GraphicViewsManager&
    GraphicViewsManager::instance()
    {
      static GraphicViewsManager inst;
      return inst;
    }

    int GraphicViewsManager::m_graphicViewCount = 0;
    static const QString graphicViewBaseName("GraphicView ");

    GraphicViewsManager::GraphicViewsManager()
      : m_graphicViews(),
	m_blocks()
    {
      QObject::connect(this, SIGNAL(graphicViewCreated(graphicView::Widget*)),
		       &Menus::instance(), SLOT(graphicViewCreated(graphicView::Widget*)));
      QObject::connect(this, SIGNAL(graphicViewDeleted(graphicView::Widget*)),
		       &Menus::instance(), SLOT(graphicViewDeleted(graphicView::Widget*)));
    }

    GraphicViewsManager::~GraphicViewsManager()
    {
    }

    graphicView::Widget*
    GraphicViewsManager::createImageGraphicView(graphicView::ImageSceneState::Type type,
				     graphicView::ViewingWindowPtr* vw,
				     graphicView::SliceWindowPtr* sw,
				     bool picking)
    {

      QString graphicViewName = graphicViewBaseName + QString::number(m_graphicViewCount++);

      graphicView::ViewingWindowPtr viewingWindow = vw ? *vw
	: graphicView::ViewingWindowManager::instance().getNewWindow();

      graphicView::SliceWindowPtr sliceWindow = sw ? *sw
	: graphicView::SliceWindowManager::instance().getNewWindow();

      graphicView::Widget* graphicView = m_graphicViews.empty()
	? new graphicView::Widget(graphicViewName, graphicView::Scene::ImageSceneType)
	: new graphicView::Widget(graphicViewName, graphicView::Scene::ImageSceneType, m_graphicViews.front());

      ((graphicView::ImageScene*) graphicView->getScene())->setStateType(type);
      graphicViewCreated(graphicView);

      ((graphicView::ImageScene*) graphicView->getScene())->setPicking(picking);

      ((graphicView::ImageScene*) graphicView->getScene())->setSliceWindow(sliceWindow);
      ((graphicView::ImageScene*) graphicView->getScene())->setViewWindow(viewingWindow);

      m_graphicViews.push_back(graphicView);
      return graphicView;
    }

    graphicView::Widget*
    GraphicViewsManager::createMeshGraphicView(graphicView::MeshSceneState::Rendering rendering,
				    bool picking)
    {

      QString graphicViewName = graphicViewBaseName + QString::number(m_graphicViewCount++);

      graphicView::Widget* graphicView = m_graphicViews.empty()
	? new graphicView::Widget(graphicViewName, graphicView::Scene::MeshSceneType)
	: new graphicView::Widget(graphicViewName, graphicView::Scene::MeshSceneType, m_graphicViews.front());

      ((graphicView::MeshScene*) graphicView->getScene())->setStateRendering(rendering);
      graphicViewCreated(graphicView);

      ((graphicView::MeshScene*) graphicView->getScene())->setPicking(picking);

      m_graphicViews.push_back(graphicView);
      return graphicView;
    }

    graphicView::BlockWidget*
    GraphicViewsManager::createBlockWidget(frontend::ViewInfo* view)
    {
      static unsigned int nameGenerator = 0;
      static const QString baseName("GraphicViewBlock ");
      QString blockName = baseName + QString::number(nameGenerator++);

      graphicView::BlockWidget* block = NULL;
      auto viewType = view->getViewType();
      if (viewType == frontend::ViewInfo::ImageType) {
	block = createDefaultImageBlockWidget(blockName);
      }
      else if (viewType == frontend::ViewInfo::MeshType) {
	block = createDefaultMeshBlockWidget(blockName);
      }
      else {
	assert(!"Should never get here!");
	return NULL;
      }

      block->setView(view);

      return block;
    }

    graphicView::BlockWidget*
    GraphicViewsManager::createDefaultImageBlockWidget(const QString& name)
    {
      graphicView::ViewingWindowPtr viewWindow(graphicView::ViewingWindowManager::instance().getNewWindow());

      auto w0 = createImageGraphicView(graphicView::ImageSceneState::State2dType, &viewWindow);
      auto w1 = createImageGraphicView(graphicView::ImageSceneState::State2dType, &viewWindow);
      auto w2 = createImageGraphicView(graphicView::ImageSceneState::State2dType, &viewWindow);
      auto w3 = createImageGraphicView(graphicView::ImageSceneState::State3dType, &viewWindow);

      ((graphicView::ImageScene*) w0->getScene())->setOrientation(Frontal);
      ((graphicView::ImageScene*) w1->getScene())->setOrientation(Sagittal);
      ((graphicView::ImageScene*) w2->getScene())->setOrientation(Axial);
      w3->m_glWidget->makeCurrent();

      graphicView::BlockWidget* blockWidget = new graphicView::BlockWidget(name);
      blockWidget->configure({w0, w1, w2, w3}, graphicView::BlockWidget::BlockLayout4);
      blockWidget->connectGraphicViews({w0, w1, w2, w3});

      m_blocks.push_back(blockWidget);

      return blockWidget;
    }

    graphicView::BlockWidget*
    GraphicViewsManager::createDefaultMeshBlockWidget(const QString& name)
    {
      auto w0 = createMeshGraphicView(graphicView::MeshSceneState::FACE);

      w0->m_glWidget->makeCurrent();

      graphicView::BlockWidget* blockWidget = new graphicView::BlockWidget(name);
      blockWidget->configure({w0}, graphicView::BlockWidget::BlockLayout1);

      m_blocks.push_back(blockWidget);

      return blockWidget;
    }

    void
    GraphicViewsManager::remove(graphicView::Widget* graphicView)
    {
      if (!graphicView)
	return;

      graphicViewDeleted(graphicView);
      m_graphicViews.remove(graphicView);
    }

    void
    GraphicViewsManager::remove(graphicView::BlockWidget* block)
    {
      if (!block)
	return;

      m_blocks.remove(block);
    }

  }
}
