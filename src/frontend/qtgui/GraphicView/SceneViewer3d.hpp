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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_HPP
#define SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_HPP

#include <functional>

#include "ArcBall.hpp"
#include "SceneViewer3d_Maths.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
RESTORE_CONTEXT()

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {
      
      class Scene;
      
      class SceneViewer3d : public QObject {

      public:
	
	SceneViewer3d(Scene& scene, bool activUnitCube = true);
	
	virtual ~SceneViewer3d();
	
	void initView();
	
	void draw(std::function<void ()> drawObject);
	
	bool translate(QGraphicsSceneMouseEvent*);
	bool rotate(QGraphicsSceneMouseEvent*);
	bool scale(QGraphicsSceneWheelEvent*);
	bool rotate(double alpha, double beta, double gamma, double scale = 1.);

	void setUnableUnitCube(bool cond);
	bool isActivUnitCube() const;
	
	void pushContext(QGraphicsSceneMouseEvent*);
	void popContext();
	
      private:

	void drawUnitCube();
	void drawLandmark();

      private:

	Scene& m_scene;
	ArcBall m_arcBall;

	float m_offset[2];
	float m_scaling;
	Matrix3fT m_lastRot;
	Matrix3fT m_thisRot;
	Matrix4fT m_transform;

	bool m_drawUnitCube;
      };
      
    }
    
  }
  
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_HPP */
