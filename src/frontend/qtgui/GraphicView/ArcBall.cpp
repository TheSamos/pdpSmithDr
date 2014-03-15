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

#include "ArcBall.hpp"

#include <cassert>
#include <cmath>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      namespace {
	static const double Epsilon = 1.0e-5;
      }

      ArcBall::ArcBall()
      	: m_ckVec(),
	  m_dgVec(),
	  m_width(0),
	  m_height(0)
      {
	for (int i = 0; i < 3; ++i)
	  m_ckVec.T[i] = m_dgVec.T[i] = 0.0f;
      }

      ArcBall::~ArcBall()
      {
      }

      float
      ArcBall::getWidth() const
      {
	return m_width;
      }

      float
      ArcBall::getHeight() const
      {
	return m_height;
      }

      void
      ArcBall::setBounds(float width, float height)
      {
	assert((width > 1.0f) && (height > 1.0f));

	// Set adjustment factor for width/height
	m_width  = 1.0f / ((width  - 1.0f) * 0.5f);
	m_height = 1.0f / ((height - 1.0f) * 0.5f);
      }

      void
      ArcBall::click(float x, float y)
      {
	// Map the point to the sphere
	mapToSphere(x, y, m_ckVec);
      }

      void
      ArcBall::drag(float x, float y, Quat4fT& q)
      {
	// Map the point to the sphere
	mapToSphere(x, y, m_dgVec);

	// Return the quaternion equivalent to the rotation
	Vector3fT Perp;

	// Compute the vector perpendicular to the begin and end vectors
	Vector3fCross(Perp, m_ckVec, m_dgVec);

	// Compute the length of the perpendicular vector
	double length = Vector3fLength(Perp);
	if (length > Epsilon) { // if its non-zero
	  // We're ok, so return the perpendicular vector as the transform after all
	  q.p.X = Perp.p.X;
	  q.p.Y = Perp.p.Y;
	  q.p.Z = Perp.p.Z;
	  // In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
	  q.p.W = Vector3fDot(m_ckVec, m_dgVec);
	}
	else { // if its zero
	  // The begin and end vectors coincide, so return an identity transform
	  for (int i = 0; i < 4; ++i)
	    q.T[i] = 0.0f;
	}
      }

      // Arcball sphere constants:
      // Diameter is       2.0f
      // Radius is         1.0f
      // Radius squared is 1.0f
      void
      ArcBall::mapToSphere(float x, float y, Vector3fT& v) const
      {
	// Adjust point coords and scale down to range of [-1 ... 1]
	x  =        (x * m_width)  - 1.0f;
	y  = 1.0f - (y * m_height);

	// Compute the square of the length of the vector to the point from the center
	float length = (x * x) + (y * y);

	// If the point is mapped outside of the sphere... (length > radius squared)
	if (length > 1.0f) {
	  // Compute a normalizing factor (radius / sqrt(length))
	  float norm = 1.0f / FuncSqrt(length);

	  // Return the "normalized" vector, a point on the sphere
	  v.p.X = x * norm;
	  v.p.Y = y * norm;
	  v.p.Z = 0.0f;
	}
	else { // Else it's on the inside
	  // Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
	  v.p.X = x;
	  v.p.Y = y;
	  v.p.Z = FuncSqrt(1.0f - length);
	}
      }

    }
  }
}
