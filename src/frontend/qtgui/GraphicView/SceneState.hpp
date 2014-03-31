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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SCENESTATE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_SCENESTATE_HPP

#include <set>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class Scene;

      class SceneState {

      public:

	virtual ~SceneState();

	virtual void glDraw() = 0;

	float getWidth() const;
	float getHeight() const;

	const std::set<Scene*>& getConnectedScenes() const;

      protected:

	SceneState(Scene& scene, bool picking);

	bool isPicking() const;
	void setPicking(bool picking);
	void picked(float x, float y, float z);

	virtual bool mouseDoubleClick(QGraphicsSceneMouseEvent* event);
	virtual bool mousePress(QGraphicsSceneMouseEvent* event);
	virtual bool mouseRelease(QGraphicsSceneMouseEvent* event);
	virtual bool mouseMove(QGraphicsSceneMouseEvent* event);
	virtual bool wheelEvent(QGraphicsSceneWheelEvent* event);

      private:

	Scene& m_scene;
	bool m_picking;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SCENESTATE_HPP */
