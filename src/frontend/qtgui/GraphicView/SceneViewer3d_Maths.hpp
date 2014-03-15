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

#ifndef SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HPP
#define SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HPP

namespace sd {
  namespace qtgui {
    namespace graphicView {

      typedef union Tuple3f_t
      {
        struct
        {
	  float X, Y, Z;
        } p;

        float T[3];
      } Tuple3fT;      //A generic 3-element tuple that is represented by single precision-floating point x,y,z coordinates. 

      typedef union Tuple4f_t
      {
        struct
        {
	  float X, Y, Z, W;
        } p;

        float T[4];
      } Tuple4fT;      //A 4-element tuple represented by single-precision floating point x,y,z,w coordinates. 

#define Quat4fT     Tuple4fT   //A 4 element unit quaternion represented by single precision floating point x,y,z,w coordinates. 

#define Vector3fT   Tuple3fT   //A 3-element vector that is represented by single-precision floating point x,y,z coordinates. 

      /**
       * Sets this vector to be the vector cross product of vectors v1 and v2.
       * @param v1 the first vector
       * @param v2 the second vector
       */
      inline static void Vector3fCross(Vector3fT& NewObj, const Vector3fT& v1, const Vector3fT& v2);

      /**
       * Computes the dot product of the this vector and vector v1.
       * @param  v1 the other vector
       */
      inline static float Vector3fDot(const Vector3fT& NewObj, const Vector3fT& v1);

      /**
       * Returns the squared length of this vector.
       * @return the squared length of this vector
       */
      inline static float Vector3fLengthSquared(const Vector3fT& NewObj);

      /**
       * Returns the length of this vector.
       * @return the length of this vector
       */
      inline static float Vector3fLength(const Vector3fT& NewObj);


      typedef union Matrix3f_t
      {
	struct
	{
	  //column major
	  union { float M00; float XX; float SX; };  //XAxis.X and Scale X
	  union { float M10; float XY;             };  //XAxis.Y
	  union { float M20; float XZ;             };  //XAxis.Z
	  union { float M01; float YX;             };  //YAxis.X
	  union { float M11; float YY; float SY; };  //YAxis.Y and Scale Y
	  union { float M21; float YZ;             };  //YAxis.Z
	  union { float M02; float ZX;             };  //ZAxis.X
	  union { float M12; float ZY;             };  //ZAxis.Y
	  union { float M22; float ZZ; float SZ; };  //ZAxis.Z and Scale Z
	} p;
	float m[3][3];
	float M[9];
      } Matrix3fT;     //A single precision floating point 3 by 3 matrix. 

      typedef union Matrix4f_t
      {
	struct
	{
	  //column major
	  union { float M00; float XX; float SX; };  //XAxis.X and Scale X
	  union { float M10; float XY;             };  //XAxis.Y
	  union { float M20; float XZ;             };  //XAxis.Z
	  union { float M30; float XW;             };  //XAxis.W
	  union { float M01; float YX;             };  //YAxis.X
	  union { float M11; float YY; float SY; };  //YAxis.Y and Scale Y
	  union { float M21; float YZ;             };  //YAxis.Z
	  union { float M31; float YW;             };  //YAxis.W
	  union { float M02; float ZX;             };  //ZAxis.X
	  union { float M12; float ZY;             };  //ZAxis.Y
	  union { float M22; float ZZ; float SZ; };  //ZAxis.Z and Scale Z
	  union { float M32; float ZW;             };  //ZAxis.W
	  union { float M03; float TX;             };  //Trans.X
	  union { float M13; float TY;             };  //Trans.Y
	  union { float M23; float TZ;             };  //Trans.Z
	  union { float M33; float TW; float SW; };  //Trans.W and Scale W
	} p;
	float m[4][4];
	float M[16];
      } Matrix4fT;     //A single precision floating point 4 by 4 matrix. 

      inline static void Matrix3fSetZero(Matrix3fT& NewObj);
      inline static void Matrix3fSetIdentity(Matrix3fT& NewObj);

      /**
       * Sets the value of this matrix to the matrix conversion of the
       * quaternion argument. 
       * @param q1 the quaternion to be converted 
       */
      //$hack this can be optimized some(if s == 0)
      inline static void Matrix3fSetRotationFromQuat4f(Matrix3fT& NewObj, const Quat4fT& q1);

      /**
       * Sets the value of this matrix to the result of multiplying itself
       * with matrix m1. 
       * @param m1 the other matrix 
       */
      inline static void Matrix3fMulMatrix3f(Matrix3fT& NewObj, const Matrix3fT& m1);

      inline static void Matrix4fSetRotationScaleFromMatrix4f(Matrix4fT& NewObj, const Matrix4fT& m1);

      /**
       * Performs SVD on this matrix and gets scale and rotation.
       * Rotation is placed into rot3, and rot4.
       * @param rot3 the rotation factor(Matrix3d). if null, ignored
       * @param rot4 the rotation factor(Matrix4) only upper 3x3 elements are changed. if null, ignored
       * @return scale factor
       */
      inline static float Matrix4fSVD(const Matrix4fT& NewObj, Matrix3fT* rot3, Matrix4fT* rot4);

      inline static void Matrix4fSetRotationScaleFromMatrix3f(Matrix4fT& NewObj, const Matrix3fT& m1);

      inline static void Matrix4fMulRotationScale(Matrix4fT& NewObj, float scale);

      /**
       * Sets the rotational component (upper 3x3) of this matrix to the matrix
       * values in the T precision Matrix3d argument; the other elements of
       * this matrix are unchanged; a singular value decomposition is performed
       * on this object's upper 3x3 matrix to factor out the scale, then this
       * object's upper 3x3 matrix components are replaced by the passed rotation
       * components, and then the scale is reapplied to the rotational
       * components.
       * @param m1 T precision 3x3 matrix
       */
      inline static void Matrix4fSetRotationFromMatrix3f(Matrix4fT& NewObj, const Matrix3fT& m1);

      inline static void Matrix4fMulMatrix4f(Matrix4fT& NewObj, const Matrix4fT& m1);

    }
  }
}

#include "SceneViewer3d_Maths.hxx"

#endif /*! SD_QTGUI_GRAPHIC_VIEW_SCENEVIEWER3D_MATHS_HPP */
