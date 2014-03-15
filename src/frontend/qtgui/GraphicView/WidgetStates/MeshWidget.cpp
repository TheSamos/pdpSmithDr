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

#include "MeshWidget.hpp"
#include "../GraphicsResourceManager.hpp"
#include "../MeshResource.hpp"

#include <meshprocessing/colorcompute/ColorComputer.hpp>
#include <meshprocessing/colorcompute/ColorScaleLayer.hpp>

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace widget_states {

	MeshWidget::MeshWidget(Widget& w) : WidgetState(w) {
	  m_meshScene.reset(new MeshScene);
	}

	MeshWidget::~MeshWidget()
	{}

	Scene*
	MeshWidget::getScene() {
	  return m_meshScene.get();
	}

	const Scene*
	MeshWidget::getScene() const {
	  return m_meshScene.get();
	}

	void
	MeshWidget::setView(frontend::ViewInfo* view) {
	  graphicView::GraphicsResourceManager& grm = graphicView::GraphicsResourceManager::instance();
	  MeshResourcePtr mesh = grm.getResourceFor((frontend::MeshInfo*) view);
	  m_meshScene->setMesh(mesh);
	  m_widget.setScene(m_meshScene.get());
	}

      }
    }
  }
}
