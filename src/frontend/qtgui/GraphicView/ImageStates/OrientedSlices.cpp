/*
  Copyright (C) 2[0][1]1 The SmithDR team, smithdr@labri.fr

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

#include "OrientedSlices.hpp"

#include <cmath>

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGLWidget>
RESTORE_CONTEXT()

#include "../Scene.hpp"
#include "../ImageScene.hpp"
#include "SingleSlice.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace image_states {

	OrientedSlices::OrientedSlices(ImageScene& scene, bool picking)
	  : ImageSceneState(scene, State3dType, picking),
	    m_viewer(scene)
	{
	}

	void
	drawSlice(ImageSlicePtr slice, float coords[4][3])
	{
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  glBegin(GL_QUADS);

	  slice->bindTexCoord(0.0f, 0.0f);
	  glVertex3fv(coords[0]);

	  slice->bindTexCoord(0.0f, 1.0f);
	  glVertex3fv(coords[1]);

	  slice->bindTexCoord(1.0f, 1.0f);
	  glVertex3fv(coords[2]);

	  slice->bindTexCoord(1.0f, 0.0f);
	  glVertex3fv(coords[3]);
	  glEnd();
	}

	void
	OrientedSlices::glDraw()
	{
	  m_viewer.draw(std::bind(&OrientedSlices::drawObject, this));
	}

	void
	OrientedSlices::drawObject()
	{
	  glDisable(GL_LIGHTING);
	  glDisable(GL_COLOR_MATERIAL);
	  float resolX = getResource()->getResolX();
	  float resolY = getResource()->getResolY();
	  float resolZ = getResource()->getResolZ();
	  float maxResol = std::max(resolX, std::max(resolY, resolZ));
	  float sizeX = (float) getResource()->getSizeX();
	  float sizeY = (float) getResource()->getSizeY();
	  float sizeZ = (float) getResource()->getSizeZ();
	  float maxSize = std::max(sizeX, std::max(sizeY, sizeZ));

	  float scaleX = resolX/maxResol * sizeX/maxSize;
	  float scaleY = resolY/maxResol * sizeY/maxSize;
	  float scaleZ = resolZ/maxResol * sizeZ/maxSize;
	  float scaleMax = std::max(scaleX, std::max(scaleY, scaleZ));

	  glScaled(scaleX/scaleMax, scaleY/scaleMax, scaleZ/scaleMax);

	  glEnable(GL_LINE_SMOOTH);
	  glHint(GL_LINE_SMOOTH, GL_NICEST);
	  glEnable(GL_BLEND);
	  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glEnable(GL_DEPTH_TEST);

	  const auto& connections = getConnectedScenes();
	  for (auto it = connections.begin(), end = connections.end(); it != end; ++it) {
	    ImageScene* scene = (ImageScene*) *it;
	    if (getResource(*scene) && scene->getState().getType() == State2dType) {
	      Orientation ori = scene->getOrientation();
	      UINT16 sliceNumber = scene->getSliceNumber();   // put in [0, max-1]
	      UINT16 max = 0;
	      switch (ori) {
	      case X_AXIS:
		max = getResource(*scene)->getSizeX();
		break;
	      case Y_AXIS:
		max = getResource(*scene)->getSizeY();
		break;
	      case Z_AXIS:
		max = getResource(*scene)->getSizeZ();
	      break;
	      case NO_AXIS:
	      default:
		assert(!"Should never get here!");
	      }

	      float coords[4][3] = {{0}};
	      float coord = 2*(sliceNumber-1.0f)/max - 1;
	      switch (ori) {
	      case X_AXIS: {
		coords[1][0] = coords[0][0] = coords[3][0] = coords[2][0] = coord;
		coords[3][1] = coords[2][1] = coords[1][2] = coords[2][2] =  1.0f;
		coords[1][1] = coords[0][1] = coords[0][2] = coords[3][2] = -1.0f;
	      }
		break;
	      case Y_AXIS: {
		coords[3][0] = coords[2][0] = coords[1][2] = coords[2][2] =  1.0f;
		coords[1][1] = coords[0][1] = coords[3][1] = coords[2][1] = coord;
		coords[1][0] = coords[0][0] = coords[0][2] = coords[3][2] = -1.0f;
	      }
		break;
	      case Z_AXIS: {
		coords[3][0] = coords[2][0] = coords[2][1] = coords[1][1] =  1.0f;
		coords[1][0] = coords[0][0] = coords[3][1] = coords[0][1] = -1.0f;
		coords[3][2] = coords[2][2] = coords[1][2] = coords[0][2] = coord;
	      }
		break;
	      case NO_AXIS:
	      default:
		assert(!"Should never get here!");
	      }

	      const ImageSlicePtr& slice = static_cast<const SingleSlice*>(&(scene)->getState())->getSlice();
	      if (slice && slice->use()) {
		glColor3f(1.0, 1.0, 1.0);  // Modulate texture with white.
		glEnable(GL_TEXTURE_2D);
		drawSlice(slice, coords);

		adjustBrightnessContrast(std::bind(drawSlice, slice, coords));

		glEnable(GL_DEPTH_TEST);
	      }

	      // draw borders
	      glDisable(GL_TEXTURE_2D);
	      glDisable(GL_TEXTURE_3D);
	      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	      glColor4f(ori == X_AXIS ? 1.0f : 0.0f,
			ori == Y_AXIS ? 1.0f : 0.0f,
			ori == Z_AXIS ? 1.0f : 0.0f,
			0.5f);
	      glBegin(GL_QUADS);
	      glVertex3fv(coords[0]);
	      glVertex3fv(coords[1]);
	      glVertex3fv(coords[2]);
	      glVertex3fv(coords[3]);
	      glEnd();
	    }
	  }
	  glEnable(GL_LIGHTING);
	  glEnable(GL_COLOR_MATERIAL);
	}

	void
	OrientedSlices::imageChanged()
	{
	  m_viewer.initView();
	}

	bool
	OrientedSlices::mouseDoubleClick(QGraphicsSceneMouseEvent* event)
	{
	  if (!ImageSceneState::mouseDoubleClick(event)) {
	    if (event->button() & Qt::LeftButton) {
	      m_viewer.initView();
	      return true;
	    }
	  }
	  return false;
	}

	bool
	OrientedSlices::mousePress(QGraphicsSceneMouseEvent* event)
	{
	  if (!ImageSceneState::mousePress(event)) {
	  if ((event->button() & Qt::LeftButton) ||
	      (event->button() & Qt::RightButton)) {
	      m_viewer.pushContext(event);
	      return true;
	    }
	  }
	  return false;
	}

	bool
	OrientedSlices::mouseRelease(QGraphicsSceneMouseEvent* event)
	{
	  if (!ImageSceneState::mouseRelease(event)) {
	  if ((event->button() & Qt::LeftButton) ||
	      (event->button() & Qt::RightButton)) {
	      m_viewer.popContext();
	      return true;
	    }
	  }
	  return false;
	}

	bool
	OrientedSlices::mouseMove(QGraphicsSceneMouseEvent* event)
	{
	  if (ImageSceneState::mouseMove(event))
	    return true;

	  if (event->buttons() & Qt::LeftButton)
	    return m_viewer.rotate(event);

	  if (event->buttons() & Qt::RightButton)
	    return m_viewer.translate(event);

	  return false;
	}

	bool
	OrientedSlices::wheelEvent(QGraphicsSceneWheelEvent* event)
	{
	  return m_viewer.scale(event);
	}

      }
    }
  }
}
