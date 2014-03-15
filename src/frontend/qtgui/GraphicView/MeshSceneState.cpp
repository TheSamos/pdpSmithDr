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

#include "MeshSceneState.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
RESTORE_CONTEXT()

#include "MeshScene.hpp"
#include "Widget.hpp"
#include "GLLayer.hpp"

#include <GL/glew.h>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      namespace {
	static const float nameBorderInPercent = 0.015f;
      }

      int MeshSceneState::m_nbDisplayList = 0;

      MeshSceneState::MeshSceneState(MeshScene& scene, MeshSceneState::Rendering r, bool picking)
	: SceneState(scene, picking),
	  m_dspList(-1),
	  m_meshScene(scene),
	  m_rendering(r),
	  m_coloring(VERTEX_COLOR),
	  m_viewer(scene) {
      }

      MeshSceneState::~MeshSceneState() {
	std::vector<GLLayer*>::iterator it = m_layers.begin();
	const std::vector<GLLayer*>::iterator end = m_layers.end();
	for (; it != end; ++it)
	  delete *it;
	if (m_dspList > 0)
	  glDeleteLists(m_dspList, 1);
      }

      int
      MeshSceneState::newUniqueDisplayListId() {
	return ++m_nbDisplayList;
      }

      MeshSceneState::Rendering
      MeshSceneState::getRendering() const {
	return m_rendering;
      }

      SceneViewer3d&
      MeshSceneState::viewer() {
	return m_viewer;
      }

      MeshResourcePtr
      MeshSceneState::getResource() const {
	return m_meshScene.m_res;
      }

      MeshResourcePtr
      MeshSceneState::getResource(MeshScene& scene) const {
	return scene.m_res;
      }

      void
      MeshSceneState::drawOverlay(QPainter* painter) {
	QRectF rect = m_meshScene.sceneRect();
	QString name;
	
	std::vector<GLLayer*>::iterator it = m_layers.begin();
	const std::vector<GLLayer*>::iterator end = m_layers.end();
	for (; it != end; ++it)
	  (*it)->draw(rect, painter);
	
	if(getResource()) {
	  const sd::core::MeshView* view = getResource()->getData();
	  if (sd::core::AttributeKey::containsKey("GLLayers")) {
	    sd::core::AttributeKey layersKey = sd::core::AttributeKey::getKey("GLLayers");
	    if (view->getAttribute(layersKey)) {
	      const sd::core::Attribute* att = view->getAttribute(layersKey);
	      assert(dynamic_cast<const sd::core::AttributeVectPtr<GLLayer*>* >(att));
	      const std::vector<GLLayer*>& vect = ((const sd::core::AttributeVectPtr<GLLayer*>*) att)->getData();
	      for (unsigned int i=0; i<vect.size(); ++i)
		vect[i]->draw(rect, painter);
	    }
	  }
	}
	
	if (isPicking()) {
	  if (getResource())
	    name = QString::fromStdString(getResource()->getName());
	} else {
	  name = m_meshScene.getWidget().getName();
	  if (getResource() && !getResource()->getName().empty())
	    name += " (" + QString::fromStdString(getResource()->getName()) + ")";
	}

	int textHeight = painter->fontMetrics().height();
	painter->setPen(Qt::black);
	// scale the textHeight since it seems (visual check) that the font metrics
	// return 1.5 the actual printed height
	painter->drawText(QPointF(nameBorderInPercent*rect.width(),
				  nameBorderInPercent*rect.height() + 2/3.0 * textHeight),
			  name);
      }

      void
      MeshSceneState::addLayer(GLLayer* layer) {
	m_layers.push_back(layer);
      }

      void
      MeshSceneState::glDraw() {
	m_viewer.draw(std::bind(&MeshSceneState::drawObject, this));
      }


      void
      MeshSceneState::changeRendering(MeshSceneState::Rendering r) {
	m_rendering = r;
	glDeleteLists(m_dspList, 1);
	m_dspList = -1;
      }
      
      void
      MeshSceneState::changeColoring(MeshSceneState::Coloring c) {
	m_coloring = c;
	glDeleteLists(m_dspList, 1);
	m_dspList = -1;
      }
      
      int
      MeshSceneState::getDisplayList() {
	if (m_dspList != -1)
	  return m_dspList;
	
	m_dspList = newUniqueDisplayListId();
	glNewList(m_dspList, GL_COMPILE);

	if (m_rendering == FACE_SMOOTH || m_rendering == FACE || m_rendering == FACE_EDGE)
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (m_rendering == EDGE)
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// :TODO: using glColor/glMaterial & color per vertex/face
	// :TODO: consider flat/smooth visu & normals per vertex
	  
	MeshResourcePtr mesh = getResource();
	
	glLineWidth(2);
	
	glEnable(GL_COLOR_MATERIAL);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	if (m_rendering == FACE || m_rendering == FACE_EDGE) {
	  std::vector<float> vertices(0);
	  unsigned int cardVertex=0;
	  unsigned int cardNormal=0;
	  unsigned int cardColor=0;
	  mesh->getVNCData(vertices,
			   cardVertex,
			   cardNormal,
			   cardColor);
	  const unsigned int factor = cardVertex * cardNormal * cardColor;
	    
	  glVertexPointer(cardVertex, GL_FLOAT, factor*sizeof(GLfloat), vertices.data());
	  glNormalPointer(GL_FLOAT, factor*sizeof(GLfloat), vertices.data()+cardVertex);
	  glColorPointer(cardColor, GL_FLOAT, factor*sizeof(GLfloat), vertices.data()+cardVertex+cardNormal);
	  
	  if (mesh->getFacesDegree() == 3)
	    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/factor);
	  else //4
	    glDrawArrays(GL_QUADS, 0, vertices.size()/factor);

	  if (m_rendering == FACE) { // _EDGE
	    glDisableClientState(GL_COLOR_ARRAY);
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    glEnable(GL_LINE_SMOOTH);
	    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	  
	    glColor3f(0.0, 0.0, 0.0);
	  
	    if (mesh->getFacesDegree() == 3)
	      glDrawArrays(GL_TRIANGLES, 0, vertices.size()/factor);
	    else //4
	      glDrawArrays(GL_QUADS, 0, vertices.size()/factor);
	  }
	} else {
	  glVertexPointer(3, GL_FLOAT, 0, mesh->getVertices().data());
	  glNormalPointer(GL_FLOAT, 0, mesh->getVertexNormals().data());
	  glColorPointer(4, GL_FLOAT, 0, mesh->getVertexColorsRGBA().data());
	  
	  if (m_rendering != DOT)
	    if (mesh->getFacesDegree() == 3)
	      glDrawElements(GL_TRIANGLES, mesh->getFaces().size(), GL_UNSIGNED_INT, mesh->getFaces().data());
	    else //4
	      glDrawElements(GL_QUADS, mesh->getFaces().size(), GL_UNSIGNED_INT, mesh->getFaces().data());
	  else
	    glDrawArrays(GL_POINTS, 0, mesh->getVertices().size()/3);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisable(GL_COLOR_MATERIAL);
	
	glEndList();

	return m_dspList;
      }
      
      void
      MeshSceneState::drawObject() {
	int dspList = getDisplayList();
	if (dspList > 0)
	  glCallList(dspList);
      }

      void
      MeshSceneState::meshChanged() {
	glDeleteLists(m_dspList, 1);
	m_dspList = -1;
      }

      bool
      MeshSceneState::mouseDoubleClick(QGraphicsSceneMouseEvent* event) {
	if (!SceneState::mouseDoubleClick(event)) {
	  if (event->button() & Qt::LeftButton) {
	    m_viewer.initView();
	    return true;
	  }
	}
	return false;
      }

      bool
      MeshSceneState::mousePress(QGraphicsSceneMouseEvent* event) {
	if (!SceneState::mousePress(event)) {
	  if ((event->button() & Qt::LeftButton) ||
	      (event->button() & Qt::MidButton) ||
	      (event->button() & Qt::RightButton)) {
	    m_viewer.pushContext(event);
	    return true;
	  }
	}
	return false;
      }

      bool
      MeshSceneState::mouseRelease(QGraphicsSceneMouseEvent* event) {
	if (!SceneState::mouseRelease(event)) {
	  if ((event->button() & Qt::LeftButton) ||
	      (event->button() & Qt::MidButton) ||
	      (event->button() & Qt::RightButton)) {
	    m_viewer.popContext();
	    return true;
	  }
	}
	return false;
      }

      bool
      MeshSceneState::mouseMove(QGraphicsSceneMouseEvent* event) {
	if (!SceneState::mouseMove(event)) {
	  if (isPicking())
	    m_meshScene.update();

	  if (event->buttons() & (Qt::LeftButton | Qt::MidButton)) {
	    if (event->modifiers() & Qt::MetaModifier)
	      return m_viewer.translate(event);
	    
	    return m_viewer.rotate(event);
	  }
	}

	return false;
      }

      bool
      MeshSceneState::wheelEvent(QGraphicsSceneWheelEvent* event) {
	return m_viewer.scale(event);
      }

    }
  }
}
