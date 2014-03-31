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

#include <GL/glew.h>

#include "SceneState.hpp"
#include "Scene.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      SceneState::SceneState(Scene& scene, bool picking)
	: m_scene(scene),
	  m_picking(picking)
      {
      }

      SceneState::~SceneState()
      {
      }

      const std::set<Scene*>&
      SceneState::getConnectedScenes() const
      {
	return m_scene.getConnectedScenes();
      }

      float
      SceneState::getWidth() const
      {
	return m_scene.getWidth();
      }

      float
      SceneState::getHeight() const
      {
	return m_scene.getHeight();
      }

      bool
      SceneState::isPicking() const
      {
	return m_picking;
      }

      void
      SceneState::setPicking(bool picking)
      {
	m_picking = picking;
      }

      void
      SceneState::picked(float x, float y, float z)
      {
	m_scene.emitPicked(x, y, z);
      }

      bool
      SceneState::mouseDoubleClick(QGraphicsSceneMouseEvent*)
      {
	return false;
      }

      bool
      SceneState::mousePress(QGraphicsSceneMouseEvent*)
      {
	return false;
      }

      bool
      SceneState::mouseRelease(QGraphicsSceneMouseEvent*)
      {
	return false;
      }

      bool
      SceneState::mouseMove(QGraphicsSceneMouseEvent*)
      {
	return false;
      }

      bool
      SceneState::wheelEvent(QGraphicsSceneWheelEvent*)
      {
	return false;
      }

    }
  }
}
