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

#ifndef SD_QTGUI_GRAPHIC_VIEW_ARCBALL_HPP
#define SD_QTGUI_GRAPHIC_VIEW_ARCBALL_HPP

#include "SceneViewer3d_Maths.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class ArcBall
      {

      public:

	ArcBall();
	~ArcBall();

	float getWidth() const;
	float getHeight() const;
	void setBounds(float width, float height);

	void click(float x, float y); // mouse down
	void drag(float x, float y, Quat4fT& q); // mouse drag, calculate rotation

      private:

	void mapToSphere(float x, float y, Vector3fT& v) const;

      protected:

	Vector3fT m_ckVec; // saved click vector
	Vector3fT m_dgVec; // saved drag vector
	float m_width; // mouse bounds width
	float m_height; // mouse bounds height

      };


    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_ARCBALL_HPP */
