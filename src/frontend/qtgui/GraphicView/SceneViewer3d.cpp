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

#include "SceneViewer3d.hpp"
#include "SceneViewer3d_Maths.hpp"
#include "SceneState.hpp"
#include "Scene.hpp"

#include <cmath>
#include <iostream>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      SceneViewer3d::SceneViewer3d(Scene& scene, bool activUnitCube)
	: m_scene(scene),
	  m_drawUnitCube(activUnitCube) {
	for (int i = 0; i < 4; ++i)
	  for (int j = 0; j < 4; ++j)
	    m_transform.m[i][j] = (i == j ? 1.0f : 0.0f);
	initView();
      }

      SceneViewer3d::~SceneViewer3d()
      {}

      void
      SceneViewer3d::initView()
      {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// initLights
	{
	  GLfloat lightKa[] = {.2f, .2f, .2f, .2f};  // ambient light
	  GLfloat lightKd[] = {.4f, .4f, .4f, .4f};  // diffuse light
	  GLfloat lightKs[] = {0.f, 0.f, 0.f, 0.f};  // specular light
	  glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	  glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);
	  glLightfv(GL_LIGHT1, GL_AMBIENT, lightKa);
	  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightKd);
	  glLightfv(GL_LIGHT1, GL_SPECULAR, lightKs);
	  
	  GLfloat lightPosition[4] = {0.f, 0.f, -10.f, 1.f};
	  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	  
	  glEnable(GL_LIGHT0);
	  glEnable(GL_LIGHT1);
	}
	
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	m_offset[0] = m_offset[1] = 0.0f;
	m_scaling = 1.0f;
	Matrix3fSetIdentity(m_lastRot);
	Matrix3fSetIdentity(m_thisRot);

	// rotate pi radians around x axis to orient y axis from top to bottom
	// (z axis points towards the inside of the screen, i.e. following  user point of view)
	m_thisRot.m[1][1] = -1;
	m_thisRot.m[1][2] = 0;
	m_thisRot.m[2][1] = 0;
	m_thisRot.m[2][2] = -1;

        Matrix4fSetRotationFromMatrix3f(m_transform, m_thisRot);
      }

      void
      SceneViewer3d::draw(std::function<void ()> drawObject)
      {
	if (m_arcBall.getWidth() != m_scene.getWidth() || m_arcBall.getHeight() != m_scene.getHeight())
	  m_arcBall.setBounds(m_scene.getWidth(), m_scene.getHeight());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)(m_scene.getWidth())/(float)(m_scene.getHeight());
	gluPerspective(45.f, ratio, .01f, 10.f);
	//glOrtho(-1.5, +1.5, -1.5, +1.5, -10, +10);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-m_offset[0], -m_offset[1], -4.0f);
	
	glScalef(m_scaling, m_scaling, m_scaling);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMultMatrixf(m_transform.M);

	// unit cube & landmark
	if (m_drawUnitCube)
	  drawUnitCube();
	drawLandmark();
	
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawObject();

	glFlush();

	// required for widgets overlay and text rendering
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      void
      SceneViewer3d::drawUnitCube()
      {
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f,  1.0f, -1.0f);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0f, -1.0f,  1.0f);
	glVertex3f( 1.0f, -1.0f,  1.0f);
	glVertex3f( 1.0f,  1.0f,  1.0f);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glVertex3f(-1.0f, -1.0f,  1.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f,  1.0f, -1.0f);
	glVertex3f(1.0f,  1.0f,  1.0f);
	glVertex3f(1.0f, -1.0f,  1.0f);
	glEnd();
      }

      void
      SceneViewer3d::drawLandmark()
      {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.88f, -0.88f, 0.0f);
	glMultMatrixf(m_transform.M);
	glScalef(0.1f, 0.1f, 0.1f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
      }

      bool
      SceneViewer3d::translate(QGraphicsSceneMouseEvent* event)
      {
	m_offset[0] += (event->lastScenePos().x() - event->scenePos().x()) * 0.005f;
	m_offset[1] -= (event->lastScenePos().y() - event->scenePos().y()) * 0.005f;
	return true;
      }

      bool
      SceneViewer3d::rotate(QGraphicsSceneMouseEvent* event)
      {
	Quat4fT ThisQuat;
	m_arcBall.drag(event->scenePos().x(), event->scenePos().y(), ThisQuat);
	Matrix3fSetRotationFromQuat4f(m_thisRot, ThisQuat);
	Matrix3fMulMatrix3f(m_thisRot, m_lastRot);
	Matrix4fSetRotationFromMatrix3f(m_transform, m_thisRot);
	return true;
      }
      
      bool
      SceneViewer3d::scale(QGraphicsSceneWheelEvent* event)
      {
	static const float scalingFactor = 1.0f/1200;
	m_scaling += event->delta() * scalingFactor;
	if (m_scaling < 0.1)
	  m_scaling = 0.1;
	return true;
      }

      bool
      SceneViewer3d::rotate(double alpha, double beta, double gamma, double scale)
      {
	Matrix4f_t transform;
	transform.m[0][0] = cos(beta)*cos(gamma);
	transform.m[0][1] = cos (gamma)*sin(alpha)*sin(beta)-cos(alpha)*sin(gamma);
	transform.m[0][2] = cos(alpha)*cos(gamma)*sin(beta)+sin(alpha)*sin(gamma);
	transform.m[0][3] = 0;
	transform.m[1][0] = cos(beta)*sin(gamma);
	transform.m[1][1] = cos(alpha)*cos(gamma)+sin(alpha)*sin(beta)*sin(gamma);
	transform.m[1][2] = -cos(gamma)*sin(alpha)+cos(alpha)*sin(beta)*sin(gamma);
	transform.m[1][3] = 0;
	transform.m[2][0] = -sin(beta);
	transform.m[2][1] = cos(beta)*sin(alpha);
	transform.m[2][2] = cos(alpha)*cos(beta);
	transform.m[2][3] = 0;
	transform.m[3][0] = 0;
	transform.m[3][1] = 0;
	transform.m[3][2] = 0;
	transform.m[3][3] = 1;

	transform.m[0][0] *= scale;
	transform.m[1][1] *= scale;
	transform.m[2][2] *= scale;
	
	Matrix4fMulMatrix4f(m_transform, transform);
	return true;
      }
      
      void
      SceneViewer3d::setUnableUnitCube(bool cond) {
	m_drawUnitCube = cond;
      }
      
      bool
      SceneViewer3d::isActivUnitCube() const {
	return m_drawUnitCube;
      }
      
      void
      SceneViewer3d::pushContext(QGraphicsSceneMouseEvent* event)
      {
	m_lastRot = m_thisRot;
	m_arcBall.click(event->scenePos().x(), event->scenePos().y());
      }

      void
      SceneViewer3d::popContext()
      {
      }

    }
  }
}
