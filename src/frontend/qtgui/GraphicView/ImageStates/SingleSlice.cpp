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

#include "SingleSlice.hpp"

#include <limits>

#include <GL/glew.h>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGLWidget>
RESTORE_CONTEXT()

#include "../Scene.hpp"
#include "../ImageScene.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace image_states {

	namespace {
	  static const float lineIndicatorsAlpha = 0.42f;
	}

	SingleSlice::SingleSlice(ImageScene& scene, bool picking)
	  : ImageSceneState(scene, State2dType, picking),
	    m_scaling(1.0f),
	    m_rotation(0),
	    m_ori(NO_AXIS),
	    m_sliceNb(1)
	{
	  memset(&m_position[0], 0, sizeof(m_position));
	  // at graphicView construction, first state is constructed and there
	  //  still is no resource nor view window attached
	  if (getResource() && getViewWindow() && m_ori != NO_AXIS) {
	    SliceDesc sliceDesc(Z_AXIS, 1);
	    ViewingWindowDesc windowDesc(getViewWindow()->getWidth(), getViewWindow()->getLevel());
	    m_slice = getResource()->getSlice(TextureResourceDesc(sliceDesc, windowDesc));
	  }
	}

	void
	SingleSlice::setOrientation(Orientation dir)
	{
	  if (m_ori != dir) {
	    m_ori = dir;
	    if (getResource()) {
	      SliceDesc sliceDesc(m_ori, m_sliceNb);
	      ViewingWindowDesc windowDesc(getViewWindow()->getWidth(), getViewWindow()->getLevel());
	      m_slice = getResource()->getSlice(TextureResourceDesc(sliceDesc, windowDesc));
	    }
	  }
	}

	Orientation
	SingleSlice::getOrientation() const
	{
	  return m_ori;
	}

	void
	SingleSlice::setSliceNumber(UINT16 sNb)
	{
	  if (m_sliceNb != sNb) {
	    m_sliceNb = sNb;
	    if (getResource()) {
	      SliceDesc sliceDesc(m_ori, m_sliceNb);
	      ViewingWindowDesc windowDesc(getViewWindow()->getWidth(), getViewWindow()->getLevel());
	      m_slice = getResource()->getSlice(TextureResourceDesc(sliceDesc, windowDesc));
	    }
	  }
	}

	UINT16
	SingleSlice::getSliceNumber() const
	{
	  return m_sliceNb;
	}

	const ImageSlicePtr
	SingleSlice::getSlice() const
	{
	  return m_slice;
	}

	void
	SingleSlice::imageChanged()
	{
	  memset(&m_position[0], 0, sizeof(m_position));
	  m_scaling = 1.0f;
	  m_rotation = 0;

	  // do not use setSliceNumber, we want to reset m_slice even if the
	  //  preceding image was aready sliced at 1
	  m_sliceNb = 1;
	  if (getResource()) {
	    SliceDesc sliceDesc(m_ori, m_sliceNb);
	    ViewingWindowDesc windowDesc(getViewWindow()->getWidth(), getViewWindow()->getLevel());
	    m_slice = getResource()->getSlice(TextureResourceDesc(sliceDesc, windowDesc));
	  }
	}

	void
	drawSlice(ImageSlicePtr slice, int rotation)
	{
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  glBegin(GL_QUADS);
	  float x1 = 0.0f, x2 = 0.0f;
	  float y1 = 0.0f, y2 = 0.0f;
	  float z1 = 0.0f, z2 = 0.0f;
	  float w1 = 0.0f, w2 = 0.0f;
	  switch (rotation) {
	  case 0: {
	    x1 = -1.0f;   x2 = -1.0f;
	    y1 =  1.0f;   y2 = -1.0f;
	    z1 =  1.0f;   z2 =  1.0f;
	    w1 = -1.0f;   w2 =  1.0f;
	  }
	    break;
	  case 1: {
	    x1 = -1.0f;   x2 =  1.0f;
	    y1 = -1.0f;   y2 = -1.0f;
	    z1 =  1.0f;   z2 = -1.0f;
	    w1 =  1.0f;   w2 =  1.0f;
	  }
	    break;
	  case 2: {
	    x1 =  1.0f;   x2 =  1.0f;
	    y1 = -1.0f;   y2 =  1.0f;
	    z1 = -1.0f;   z2 = -1.0f;
	    w1 =  1.0f;   w2 = -1.0f;
	  }
	    break;
	  case 3: {
	    x1 =  1.0f;   x2 = -1.0f;
	    y1 =  1.0f;   y2 =  1.0f;
	    z1 = -1.0f;   z2 =  1.0f;
	    w1 = -1.0f;   w2 = -1.0f;
	  }
	    break;
	  default:
	    assert(!"Should never get here!");
	  }
	  slice->bindTexCoord(0.0f, 1.0f);   glVertex2f(x1, x2);
	  slice->bindTexCoord(1.0f, 1.0f);   glVertex2f(y1, y2);
	  slice->bindTexCoord(1.0f, 0.0f);   glVertex2f(z1, z2);
	  slice->bindTexCoord(0.0f, 0.0f);   glVertex2f(w1, w2);
	  glEnd();
	}

	void
	SingleSlice::glDraw()
	{
	  glDisable(GL_LIGHTING);
	  glMatrixMode(GL_PROJECTION);
	  glPushMatrix();
	  glLoadIdentity();

	  float ratio = getWidth() / getHeight();
	  if (ratio > 1) {
	    glScaled(1 / ratio, 1.0f, 1.0f);
	  }
	  else {
	    glScaled(1.0f, ratio, 1.0f);
	  }

	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glLoadIdentity();
	  glTranslatef(m_position[0], m_position[1], 0.0f);
	  glScaled(0.85 * m_scaling, 0.85 * m_scaling, 1.0);
	  // Keep some space between the images and the widget borders

	  float resolX = getResource()->getResolX();
	  float resolY = getResource()->getResolY();
	  float resolZ = getResource()->getResolZ();
	  float maxResol = std::max(resolX, std::max(resolY, resolZ));
	  float sizeX = (float)getResource()->getSizeX();
	  float sizeY = (float)getResource()->getSizeY();
	  float sizeZ = (float)getResource()->getSizeZ();
	  float maxSize = std::max(sizeX, std::max(sizeY, sizeZ));
	  float newResX = 1.0f, newResY = 1.0f;
	  switch (m_ori) {
	  case X_AXIS: {
	    newResX = resolY/maxResol * sizeY/maxSize;
	    newResY = resolZ/maxResol * sizeZ/maxSize;
	    if (m_rotation % 2 != 0)
	      std::swap(newResX, newResY);
	  }
	    break;
	  case Y_AXIS: {
	    newResX = resolX/maxResol * sizeX/maxSize;
	    newResY = resolZ/maxResol * sizeZ/maxSize;
	    if (m_rotation % 2 != 0)
	      std::swap(newResX, newResY);
	  }
	    break;
	  case Z_AXIS: {
	    newResX = resolX/maxResol * sizeX/maxSize;
	    newResY = resolY/maxResol * sizeY/maxSize;
	    if (m_rotation % 2 != 0)
	      std::swap(newResX, newResY);
	  }
	    break;
	  case NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  }
	  // ensure a minimum size, to allow some visibility for very small object
	  // (eg. one slice in a very big image)
	  if (newResX < 0.025f)
	    newResX = 0.025f;
	  if (newResY < 0.025f)
	    newResY = 0.025f;

	  float scaleMax = std::max(newResX, newResY);
	  glScaled(newResX/scaleMax, newResY/scaleMax, 1.0f);

	  glEnable(GL_LINE_SMOOTH);
	  glHint(GL_LINE_SMOOTH, GL_NICEST);
	  glEnable(GL_BLEND);

	  if (m_slice->use()) {
	    glColor3f(1.0, 1.0, 1.0);  // Modulate texture with white.
	    glEnable(GL_TEXTURE_2D);
	    drawSlice(m_slice, m_rotation);

	    adjustBrightnessContrast(std::bind(drawSlice, m_slice, m_rotation));

	    // draw borders
	    glDisable(GL_TEXTURE_2D);
	    glDisable(GL_TEXTURE_3D);
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    glColor4f(m_ori == X_AXIS ? 1.0f : 0.0f,
		      m_ori == Y_AXIS ? 1.0f : 0.0f,
		      m_ori == Z_AXIS ? 1.0f : 0.0f,
		      lineIndicatorsAlpha);
	    glBegin(GL_LINE_LOOP);
	    glVertex2f(-1.0f, -1.0f);
	    glVertex2f( 1.0f, -1.0f);
	    glVertex2f( 1.0f,  1.0f);
	    glVertex2f(-1.0f,  1.0f);
	    glEnd();
	  } // m_slice->use()

	  // draw borders on connected scenes
	  const auto& connections = getConnectedScenes();
	  for (auto it = connections.begin(), end = connections.end(); it != end; ++it) {
	    ImageScene* scene = (ImageScene*) *it;
	    if (!(scene->getState().getType() == State2dType && getResource(*scene))) {
	      continue;
	    }
	    Orientation ori = scene->getOrientation();
	    if (ori == m_ori)
	      continue;

	    glColor4f(ori == X_AXIS ? 1.0f : 0.0f,
		      ori == Y_AXIS ? 1.0f : 0.0f,
		      ori == Z_AXIS ? 1.0f : 0.0f,
		      lineIndicatorsAlpha);
	    UINT16 sliceNumber = scene->getSliceNumber() - 1;   // put in [0, max-1]
	    glBegin(GL_LINES);
	    switch (m_ori) {
	    case X_AXIS: {
	      switch (ori) {
	      case X_AXIS:
		break;
	      case Y_AXIS: {
		UINT16 max = getResource(*scene)->getSizeY();
		switch (m_rotation) {
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 0: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 2: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case Z_AXIS: {
		UINT16 max = getResource(*scene)->getSizeZ();
		switch (m_rotation) {
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 2: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 0: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case NO_AXIS:
	      default:
		assert(!"Should never get here!");
	      }
	    }
	      break;
	    case Y_AXIS: {
	      switch (ori) {
	      case X_AXIS: {
		UINT16 max = getResource(*scene)->getSizeX();
		switch (m_rotation) {
		case 0: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 2: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case Y_AXIS: {
	      }
		break;
	      case Z_AXIS: {
		UINT16 max = getResource(*scene)->getSizeZ();
		switch (m_rotation) {
		case 2: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 0: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case NO_AXIS:
	      default:
		assert(!"Should never get here!");
	      }
	    }
	      break;
	    case Z_AXIS: {
	      switch (ori) {
	      case X_AXIS: {
		UINT16 max = getResource(*scene)->getSizeX();
		switch (m_rotation) {
		case 0: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 2: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case Y_AXIS: {
		UINT16 max = getResource(*scene)->getSizeY();
		switch (m_rotation) {
		case 2: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 3: {
		  float coord = (2.0f*sliceNumber)/max - 1 + 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case 0: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(-1.05f, coord);
		  glVertex2f( 1.05f, coord);
		}
		  break;
		case 1: {
		  float coord = 2 - ((2.0f*sliceNumber)/max - 1) - 2 - 1.0f/max;
		  glVertex2f(coord, -1.05f);
		  glVertex2f(coord,  1.05f);
		}
		  break;
		case NO_AXIS:
		default:
		  assert(!"Should never get here!");
		}
	      }
		break;
	      case Z_AXIS:
		break;
	      case NO_AXIS:
	      default:
		assert(!"Should never get here!");
	      }
	    }
	      break;
	    case NO_AXIS:
	    default:
	      assert(!"Should never get here!");
	    }
	    glEnd();
	  }

	  glEnable(GL_TEXTURE_2D);                     // required for widgets overlay
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // and text rendering

	  glPopMatrix();
	  glMatrixMode(GL_PROJECTION);
	  glPopMatrix();
	}

	bool
	SingleSlice::mouseDoubleClick(QGraphicsSceneMouseEvent* event)
	{
	  if (!ImageSceneState::mouseDoubleClick(event)) {
	    if (event->button() & Qt::LeftButton) {
	      memset(m_position, 0, sizeof(m_position));
	      m_scaling = 1.0f;

	      return true;
	    }
	  }
	  return false;
	}

	void
	SingleSlice::rotate90(int sign)
	{
	  m_rotation += sign;
	  m_rotation %= 4;
	  if (m_rotation < 0)
	    m_rotation += 4;
	}

	bool
	SingleSlice::mousePress(QGraphicsSceneMouseEvent* event)
	{
	  if (ImageSceneState::mousePress(event))
	    return true;

	  if (isPicking() && getResource() && (event->modifiers() & Qt::ControlModifier)) {
	    float posX = event->scenePos().x();
	    float posY = getHeight() - event->scenePos().y();
	    float virtualMousePos[2] = { 2 *(posX / getWidth())  - 1,
					 2 *(posY / getHeight()) - 1 };
	    float topleft[] = { -1.0f, 1.0f };
	    float botright[] = { 1.0f, -1.0f };

	    float resolX = getResource()->getResolX();
	    float resolY = getResource()->getResolY();
	    float resolZ = getResource()->getResolZ();
	    float maxResol = std::max(resolX, std::max(resolY, resolZ));
	    float sizeX = (float) getResource()->getSizeX();
	    float sizeY = (float) getResource()->getSizeY();
	    float sizeZ = (float) getResource()->getSizeZ();
	    float maxSize = std::max(sizeX, std::max(sizeY, sizeZ));
	    float newResX = 1.0f, newResY = 1.0f;
	    float texSizeX = 0, texSizeY = 0;
	    switch (m_ori) {
	    case X_AXIS: {
	      texSizeX = sizeY;
	      texSizeY = sizeZ;
	      newResX = resolY/maxResol * sizeY/maxSize;
	      newResY = resolZ/maxResol * sizeZ/maxSize;
	    }
	      break;
	    case Y_AXIS: {
	      texSizeX = sizeX;
	      texSizeY = sizeZ;
	      newResX = resolX/maxResol * sizeX/maxSize;
	      newResY = resolZ/maxResol * sizeZ/maxSize;
	    }
	      break;
	    case Z_AXIS: {
	      texSizeX = sizeX;
	      texSizeY = sizeY;
	      newResX = resolX/maxResol * sizeX/maxSize;
	      newResY = resolY/maxResol * sizeY/maxSize;
	    }
	      break;
	    case NO_AXIS:
	    default:
	      assert(!"Should never get here!");
	    }
	    // ensure a minimum size, to allow some visibility for very small object
	    // (eg. one slice in a very big image)
	    if (newResX < 0.025f)
	      newResX = 0.025f;
	    if (newResY < 0.025f)
	      newResY = 0.025f;

	    topleft[0] *= newResX * 0.85 * m_scaling;
	    botright[0] *= newResX * 0.85 * m_scaling;
	    topleft[1] *= newResY * 0.85 * m_scaling;
	    botright[1] *= newResY * 0.85 * m_scaling;
	    topleft[0] += m_position[0];
	    topleft[1] += m_position[1];
	    botright[0] += m_position[0];
	    botright[1] += m_position[1];
	    float ratio = getWidth() / getHeight();
	    if (ratio > 1) {
	      topleft[0] /= ratio;
	      botright[0] /= ratio;
	    }
	    else {
	      topleft[1] *= ratio;
	      botright[1] *= ratio;
	    }
	    // inside the texture
	    if (topleft[0]-botright[0] != 0.0f && botright[1]-topleft[1] != 0.0f &&        // zoom to a point
		virtualMousePos[0] >= topleft[0] && virtualMousePos[0] <= botright[0] &&
		virtualMousePos[1] >= botright[1] && virtualMousePos[1] <= topleft[1]) {  // inside the texture
	      float virtualCoord[] = { (topleft[0]-virtualMousePos[0])/(topleft[0]-botright[0]),
				       (botright[1]-virtualMousePos[1])/(botright[1]-topleft[1]) };
	      picked(virtualCoord[0] * texSizeX, (1-virtualCoord[1]) * texSizeY, m_sliceNb-1);
	      return true;
	    }
	  }
	  return false;
	}

	bool
	SingleSlice::mouseMove(QGraphicsSceneMouseEvent* event)
	{
	  if (ImageSceneState::mouseMove(event))
	    return true;

	  if (event->buttons() & Qt::LeftButton) {
	    QPointF newPos  = event->scenePos();
	    QPointF lastPos = event->lastScenePos();
	    float distX = newPos.x() - lastPos.x();
	    float distY = lastPos.y() - newPos.y();
	    static const float translationFactor = 0.008f;
	    m_position[0] += distX * translationFactor;
	    m_position[1] += distY * translationFactor;

	    return true;
	  }
	  return false;
	}

	bool
	SingleSlice::wheelEvent(QGraphicsSceneWheelEvent* event)
	{
	  static const float scalingFactor = 1.0f/1200;
	  m_scaling += event->delta() * scalingFactor;
	  if (m_scaling < 0.1)
	    m_scaling = 0.1;
	  return true;
	}

      }
    }
  }
}
