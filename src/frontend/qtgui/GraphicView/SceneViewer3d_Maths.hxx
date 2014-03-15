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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HXX
#define SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HXX

#include <cmath>
#include <cassert>
#include <cstdio>

namespace sd {
  namespace qtgui {
    namespace graphicView {

#define FuncSqrt sqrtf

      void
      Vector3fCross(Vector3fT& NewObj, const Vector3fT& v1, const Vector3fT& v2)
      {
        Vector3fT Result; //safe not to initialize

        // store on stack once for aliasing-safty
        // i.e. safe when a.cross(a, b)

        Result.p.X = (v1.p.Y * v2.p.Z) - (v1.p.Z * v2.p.Y);
        Result.p.Y = (v1.p.Z * v2.p.X) - (v1.p.X * v2.p.Z);
        Result.p.Z = (v1.p.X * v2.p.Y) - (v1.p.Y * v2.p.X);

        // copy result back
        NewObj = Result;
      }

      float
      Vector3fDot(const Vector3fT& NewObj, const Vector3fT& v1)
      {
        return (NewObj.p.X * v1.p.X) +
	  (NewObj.p.Y * v1.p.Y) +
	  (NewObj.p.Z * v1.p.Z);
      }

      float
      Vector3fLengthSquared(const Vector3fT& NewObj)
      {
        return (NewObj.p.X * NewObj.p.X) +
	  (NewObj.p.Y * NewObj.p.Y) +
	  (NewObj.p.Z * NewObj.p.Z);
      }

      float
      Vector3fLength(const Vector3fT& NewObj)
      {
        return FuncSqrt(Vector3fLengthSquared(NewObj));
      }

      void
      Matrix3fSetZero(Matrix3fT& NewObj)
      {
	for (int i = 0; i < 9; ++i)
	  NewObj.M[i] = 0.0f;
      }

      void
      Matrix3fSetIdentity(Matrix3fT& NewObj)
      {
        Matrix3fSetZero(NewObj);

        // then set diagonal as 1
	for (int i = 0; i < 3; ++i)
	  NewObj.m[i][i] = 1.0f;
      }

      void
      Matrix3fSetRotationFromQuat4f(Matrix3fT& NewObj, const Quat4fT& q1)
      {
        float n = (q1.p.X * q1.p.X) + (q1.p.Y * q1.p.Y) + (q1.p.Z * q1.p.Z) + (q1.p.W * q1.p.W);
        float s = (n > 0.0f) ? (2.0f / n) : 0.0f;

        float xs = q1.p.X * s;
	float ys = q1.p.Y * s;
	float zs = q1.p.Z * s;

        float wx = q1.p.W * xs;
	float wy = q1.p.W * ys;
	float wz = q1.p.W * zs;

        float xx = q1.p.X * xs;
	float xy = q1.p.X * ys;
	float xz = q1.p.X * zs;

        float yy = q1.p.Y * ys;
	float yz = q1.p.Y * zs;
	float zz = q1.p.Z * zs;

        NewObj.p.XX = 1.0f - (yy + zz);
	NewObj.p.YX =         xy - wz;
	NewObj.p.ZX =         xz + wy;

        NewObj.p.XY =         xy + wz;
	NewObj.p.YY = 1.0f - (xx + zz);
	NewObj.p.ZY =         yz - wx;

        NewObj.p.XZ =         xz - wy;
	NewObj.p.YZ =         yz + wx;
	NewObj.p.ZZ = 1.0f - (xx + yy);
      }

      void
      Matrix3fMulMatrix3f(Matrix3fT& NewObj, const Matrix3fT& m1)
      {
        Matrix3fT Result; //safe not to initialize

        // alias-safe way.
        Result.p.M00 = (NewObj.p.M00 * m1.p.M00) + (NewObj.p.M01 * m1.p.M10) + (NewObj.p.M02 * m1.p.M20);
        Result.p.M01 = (NewObj.p.M00 * m1.p.M01) + (NewObj.p.M01 * m1.p.M11) + (NewObj.p.M02 * m1.p.M21);
        Result.p.M02 = (NewObj.p.M00 * m1.p.M02) + (NewObj.p.M01 * m1.p.M12) + (NewObj.p.M02 * m1.p.M22);

        Result.p.M10 = (NewObj.p.M10 * m1.p.M00) + (NewObj.p.M11 * m1.p.M10) + (NewObj.p.M12 * m1.p.M20);
        Result.p.M11 = (NewObj.p.M10 * m1.p.M01) + (NewObj.p.M11 * m1.p.M11) + (NewObj.p.M12 * m1.p.M21);
        Result.p.M12 = (NewObj.p.M10 * m1.p.M02) + (NewObj.p.M11 * m1.p.M12) + (NewObj.p.M12 * m1.p.M22);

        Result.p.M20 = (NewObj.p.M20 * m1.p.M00) + (NewObj.p.M21 * m1.p.M10) + (NewObj.p.M22 * m1.p.M20);
        Result.p.M21 = (NewObj.p.M20 * m1.p.M01) + (NewObj.p.M21 * m1.p.M11) + (NewObj.p.M22 * m1.p.M21);
        Result.p.M22 = (NewObj.p.M20 * m1.p.M02) + (NewObj.p.M21 * m1.p.M12) + (NewObj.p.M22 * m1.p.M22);

        //copy result back to this
        NewObj = Result;
      }

      void
      Matrix4fSetRotationScaleFromMatrix4f(Matrix4fT& NewObj, const Matrix4fT& m1)
      {
        NewObj.p.XX = m1.p.XX; NewObj.p.YX = m1.p.YX; NewObj.p.ZX = m1.p.ZX;
        NewObj.p.XY = m1.p.XY; NewObj.p.YY = m1.p.YY; NewObj.p.ZY = m1.p.ZY;
        NewObj.p.XZ = m1.p.XZ; NewObj.p.YZ = m1.p.YZ; NewObj.p.ZZ = m1.p.ZZ;
      }

      float
      Matrix4fSVD(const Matrix4fT& NewObj, Matrix3fT* rot3, Matrix4fT* rot4)
      {
        // this is a simple svd.
        // Not complete but fast and reasonable.
        // See comment in Matrix3d.

        float s = FuncSqrt(
		     ( (NewObj.p.XX * NewObj.p.XX) + (NewObj.p.XY * NewObj.p.XY) + (NewObj.p.XZ * NewObj.p.XZ) + 
		       (NewObj.p.YX * NewObj.p.YX) + (NewObj.p.YY * NewObj.p.YY) + (NewObj.p.YZ * NewObj.p.YZ) +
		       (NewObj.p.ZX * NewObj.p.ZX) + (NewObj.p.ZY * NewObj.p.ZY) + (NewObj.p.ZZ * NewObj.p.ZZ) ) / 3.0f );

        if (rot3) {  //if pointer not null
	  //this.getRotationScale(rot3);
	  rot3->p.XX = NewObj.p.XX; rot3->p.XY = NewObj.p.XY; rot3->p.XZ = NewObj.p.XZ;
	  rot3->p.YX = NewObj.p.YX; rot3->p.YY = NewObj.p.YY; rot3->p.YZ = NewObj.p.YZ;
	  rot3->p.ZX = NewObj.p.ZX; rot3->p.ZY = NewObj.p.ZY; rot3->p.ZZ = NewObj.p.ZZ;

	  // zero-div may occur.
	  float n;
	  n = 1.0f / FuncSqrt( (NewObj.p.XX * NewObj.p.XX) +
			       (NewObj.p.XY * NewObj.p.XY) +
			       (NewObj.p.XZ * NewObj.p.XZ) );
	  rot3->p.XX *= n;
	  rot3->p.XY *= n;
	  rot3->p.XZ *= n;

	  n = 1.0f / FuncSqrt( (NewObj.p.YX * NewObj.p.YX) +
			       (NewObj.p.YY * NewObj.p.YY) +
			       (NewObj.p.YZ * NewObj.p.YZ) );
	  rot3->p.YX *= n;
	  rot3->p.YY *= n;
	  rot3->p.YZ *= n;

	  n = 1.0f / FuncSqrt( (NewObj.p.ZX * NewObj.p.ZX) +
			       (NewObj.p.ZY * NewObj.p.ZY) +
			       (NewObj.p.ZZ * NewObj.p.ZZ) );
	  rot3->p.ZX *= n;
	  rot3->p.ZY *= n;
	  rot3->p.ZZ *= n;
	}

        if (rot4) {   //if pointer not null
	  if (rot4 != &NewObj) {
	    Matrix4fSetRotationScaleFromMatrix4f(*rot4, NewObj);  // private method
	  }

	  // zero-div may occur.
	  float n;
	  n = 1.0f / FuncSqrt( (NewObj.p.XX * NewObj.p.XX) +
			       (NewObj.p.XY * NewObj.p.XY) +
			       (NewObj.p.XZ * NewObj.p.XZ) );
	  rot4->p.XX *= n;
	  rot4->p.XY *= n;
	  rot4->p.XZ *= n;

	  n = 1.0f / FuncSqrt( (NewObj.p.YX * NewObj.p.YX) +
			       (NewObj.p.YY * NewObj.p.YY) +
			       (NewObj.p.YZ * NewObj.p.YZ) );
	  rot4->p.YX *= n;
	  rot4->p.YY *= n;
	  rot4->p.YZ *= n;

	  n = 1.0f / FuncSqrt( (NewObj.p.ZX * NewObj.p.ZX) +
			       (NewObj.p.ZY * NewObj.p.ZY) +
			       (NewObj.p.ZZ * NewObj.p.ZZ) );
	  rot4->p.ZX *= n;
	  rot4->p.ZY *= n;
	  rot4->p.ZZ *= n;
	}

        return s;
      }

      void
      Matrix4fSetRotationScaleFromMatrix3f(Matrix4fT& NewObj, const Matrix3fT& m1)
      {
        NewObj.p.XX = m1.p.XX; NewObj.p.YX = m1.p.YX; NewObj.p.ZX = m1.p.ZX;
        NewObj.p.XY = m1.p.XY; NewObj.p.YY = m1.p.YY; NewObj.p.ZY = m1.p.ZY;
        NewObj.p.XZ = m1.p.XZ; NewObj.p.YZ = m1.p.YZ; NewObj.p.ZZ = m1.p.ZZ;
      }

      void
      Matrix4fMulRotationScale(Matrix4fT& NewObj, float scale)
      {
        NewObj.p.XX *= scale; NewObj.p.YX *= scale; NewObj.p.ZX *= scale;
        NewObj.p.XY *= scale; NewObj.p.YY *= scale; NewObj.p.ZY *= scale;
        NewObj.p.XZ *= scale; NewObj.p.YZ *= scale; NewObj.p.ZZ *= scale;
      }

      void
      Matrix4fSetRotationFromMatrix3f(Matrix4fT& NewObj, const Matrix3fT& m1)
      {
        float scale = Matrix4fSVD(NewObj, NULL, NULL);

        Matrix4fSetRotationScaleFromMatrix3f(NewObj, m1);
        Matrix4fMulRotationScale(NewObj, scale);
      }

      void
      Matrix4fMulMatrix4f(Matrix4fT& NewObj, const Matrix4fT& m1) {
	Matrix4fT Result;
	
        Result.p.M00 = (NewObj.p.M00 * m1.p.M00) + (NewObj.p.M01 * m1.p.M10) + (NewObj.p.M02 * m1.p.M20) + (NewObj.p.M03 * m1.p.M30);
        Result.p.M01 = (NewObj.p.M00 * m1.p.M01) + (NewObj.p.M01 * m1.p.M11) + (NewObj.p.M02 * m1.p.M21) + (NewObj.p.M03 * m1.p.M31);
        Result.p.M02 = (NewObj.p.M00 * m1.p.M02) + (NewObj.p.M01 * m1.p.M12) + (NewObj.p.M02 * m1.p.M22) + (NewObj.p.M03 * m1.p.M32);
        Result.p.M03 = (NewObj.p.M00 * m1.p.M03) + (NewObj.p.M01 * m1.p.M13) + (NewObj.p.M02 * m1.p.M23) + (NewObj.p.M03 * m1.p.M33);

        Result.p.M10 = (NewObj.p.M10 * m1.p.M00) + (NewObj.p.M11 * m1.p.M10) + (NewObj.p.M12 * m1.p.M20) + (NewObj.p.M13 * m1.p.M30);
        Result.p.M11 = (NewObj.p.M10 * m1.p.M01) + (NewObj.p.M11 * m1.p.M11) + (NewObj.p.M12 * m1.p.M21) + (NewObj.p.M13 * m1.p.M31);
        Result.p.M12 = (NewObj.p.M10 * m1.p.M02) + (NewObj.p.M11 * m1.p.M12) + (NewObj.p.M12 * m1.p.M22) + (NewObj.p.M13 * m1.p.M32);
        Result.p.M13 = (NewObj.p.M10 * m1.p.M03) + (NewObj.p.M11 * m1.p.M13) + (NewObj.p.M12 * m1.p.M23) + (NewObj.p.M13 * m1.p.M33);

        Result.p.M20 = (NewObj.p.M20 * m1.p.M00) + (NewObj.p.M21 * m1.p.M10) + (NewObj.p.M22 * m1.p.M20) + (NewObj.p.M23 * m1.p.M30);
        Result.p.M21 = (NewObj.p.M20 * m1.p.M01) + (NewObj.p.M21 * m1.p.M11) + (NewObj.p.M22 * m1.p.M21) + (NewObj.p.M23 * m1.p.M31);
        Result.p.M22 = (NewObj.p.M20 * m1.p.M02) + (NewObj.p.M21 * m1.p.M12) + (NewObj.p.M22 * m1.p.M22) + (NewObj.p.M23 * m1.p.M32);
        Result.p.M23 = (NewObj.p.M20 * m1.p.M03) + (NewObj.p.M21 * m1.p.M13) + (NewObj.p.M22 * m1.p.M23) + (NewObj.p.M23 * m1.p.M33);
	
        Result.p.M30 = (NewObj.p.M30 * m1.p.M00) + (NewObj.p.M31 * m1.p.M10) + (NewObj.p.M32 * m1.p.M20) + (NewObj.p.M33 * m1.p.M30);
        Result.p.M31 = (NewObj.p.M30 * m1.p.M01) + (NewObj.p.M31 * m1.p.M11) + (NewObj.p.M32 * m1.p.M21) + (NewObj.p.M33 * m1.p.M31);
        Result.p.M32 = (NewObj.p.M30 * m1.p.M02) + (NewObj.p.M31 * m1.p.M12) + (NewObj.p.M32 * m1.p.M22) + (NewObj.p.M33 * m1.p.M32);
        Result.p.M33 = (NewObj.p.M30 * m1.p.M03) + (NewObj.p.M31 * m1.p.M13) + (NewObj.p.M32 * m1.p.M23) + (NewObj.p.M33 * m1.p.M33);
	
        NewObj = Result;
      }
      
    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HXX */
