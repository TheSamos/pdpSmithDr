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

#ifndef SD_QTGUI_GRAPHIC_VIEW_MESHSCENESTATE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_MESHSCENESTATE_HPP

#include <GL/glew.h>

#include "MeshResource.hpp"
#include "SceneState.hpp"
#include "SceneViewer3d.hpp"

#include <set>
#include <functional>

class QPainter;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class Scene;
      class MeshScene;
      class GLLayer;

      class MeshSceneState : public SceneState {

      public:

	enum Rendering {FACE_EDGE, FACE, FACE_SMOOTH, EDGE, DOT};
	enum Coloring {FACE_COLOR, VERTEX_COLOR, EDGE_COLOR}; 

	MeshSceneState(MeshScene& scene, MeshSceneState::Rendering r, bool picking);
	
	virtual ~MeshSceneState();

	Rendering getRendering() const;
	
	SceneViewer3d& viewer();	
	
	virtual void drawOverlay(QPainter* painter);

	void addLayer(GLLayer* layer);

	virtual void meshChanged();
	
	virtual void glDraw();

	void changeRendering(Rendering r);
	void changeColoring(Coloring c);
	
      protected:

	friend class MeshScene;

	MeshResourcePtr getResource() const;
	MeshResourcePtr getResource(MeshScene& scene) const; //< used for connected scenes

	int newUniqueDisplayListId();

	virtual bool mouseDoubleClick(QGraphicsSceneMouseEvent* event);
	virtual bool mousePress(QGraphicsSceneMouseEvent* event);
	virtual bool mouseRelease(QGraphicsSceneMouseEvent* event);
	virtual bool mouseMove(QGraphicsSceneMouseEvent* event);
	virtual bool wheelEvent(QGraphicsSceneWheelEvent* event);

	virtual int getDisplayList();

      private:

	  void drawObject();

      protected:

	int m_dspList;

      private:

	MeshScene& m_meshScene;
	Rendering m_rendering;
	Coloring m_coloring;
	static int m_nbDisplayList;
	SceneViewer3d m_viewer;
	
	std::vector<GLLayer*> m_layers;
	
      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_MESHSCENESTATE_HPP */
