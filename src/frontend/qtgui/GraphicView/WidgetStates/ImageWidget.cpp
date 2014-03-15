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

#include "ImageWidget.hpp"
#include "../GraphicsResourceManager.hpp"
#include "../ImageResource.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace widget_states {

	ImageWidget::ImageWidget(Widget& w)
	  : WidgetState(w)
	{
	  m_imageScene.reset(new ImageScene);
	}

	ImageWidget::~ImageWidget()
	{
	}

	Scene*
	ImageWidget::getScene()
	{
	  return m_imageScene.get();
	}

	const Scene*
	ImageWidget::getScene() const
	{
	  return m_imageScene.get();
	}

	void
	ImageWidget::setView(frontend::ViewInfo* view)
	{
	  auto& grm = graphicView::GraphicsResourceManager::instance();
	  ImageResourcePtr image = grm.getResourceFor((frontend::ImageInfo*) view);
	  m_imageScene->setImage(image);
	  m_widget.setScene(m_imageScene.get());
	}

      }
    }
  }
}
