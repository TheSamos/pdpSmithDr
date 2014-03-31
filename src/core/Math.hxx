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

#include <limits>
#include <math.h>

#ifndef SD_CORE_MATH_HXX
#define SD_CORE_MATH_HXX

#ifndef SD_CORE_POINT3D_HPP
#include "Point3d.hpp"
#endif /*! SD_CORE_POINT3D_HPP */

#ifndef SD_CORE_VECTOR3D_HPP
#include "Vector3d.hpp"
#endif /*! SD_CORE_VECTOR3D_HPP */

#ifndef SD_CORE_LINE3D_HPP
#include "Line3d.hpp"
#endif /*! SD_CORE_LINE3D_HPP */

template<typename T>
inline static T
sd::core::epsilon() {
  return std::numeric_limits<T>::epsilon();
}

template<typename T>
inline static bool
sd::core::about_equals(const T& a, const T& b, T epsilon) {
  assert(epsilon == abs(epsilon));
  return (a >= b-epsilon && a <= b+epsilon);
}

template<typename T>
inline static bool
sd::core::about_equals(const sd::core::Vector3d<T>& a, const sd::core::Vector3d<T>& b, T epsilon) {
  assert(epsilon >= (T)0.);
  bool equals = (a.nbDims() == b.nbDims());
  for(unsigned int i=0; equals && i<a.nbDims(); ++i)
    equals &= a[i] >= b[i]-epsilon && a[i] <= b[i]+epsilon;
  return equals;
}

template<typename T>
inline static bool
sd::core::about_equals(const sd::core::Point3d<T>& a, const sd::core::Point3d<T>& b, T epsilon) {
  assert(epsilon >= (T)0.);
  bool equals = (a.nbDims() == b.nbDims());
  for(unsigned int i=0; equals && i<a.nbDims(); ++i)
    equals &= a[i] >= b[i]-epsilon && a[i] <= b[i]+epsilon;
  return equals;
}

template<typename T>
inline static T
sd::core::abs(T val) {
  if (val < 0)
    return -val;
  return val;
}

template<typename T>
inline static T
sd::core::acos(T val) {
  if (about_equals(val, (T)1, (T)0.00000000001)) {
    return 0;
  }
  if (about_equals(val, (T)-1, (T)0.00000000001)) {
    return M_PI;
  }
  return acosl(val);
}
  
template<typename T>
inline static T
sd::core::asin(T val) {
  if (about_equals(val, (T)1, (T)0.00000000001)) {
    return M_PI/2;
  }
  if (about_equals(val, (T)-1, (T)0.00000000001)) {
    return -M_PI/2;
  }
  return asinl(val);
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::euclideanDistance(const sd::core::Point3d<T>& a,
			    const sd::core::Point3d<T>& b) {
  return sqrt((b.x()-a.x())*(b.x()-a.x())
	      + (b.y()-a.y())*(b.y()-a.y())
	      + (b.z()-a.z())*(b.z()-a.z()));
}

template<typename T>
typename sd::core::Point3d<T>
sd::core::midpoint(const sd::core::Point3d<T>& a,
		 const sd::core::Point3d<T>& b) {
  return sd::core::Point3d<T>((a.x()+b.x())/2., (a.y()+b.y())/2., (a.z()+b.z())/2.);
}
    
template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::angle(const sd::core::Point3d<T>& a,
		const sd::core::Point3d<T>& b,
		const sd::core::Point3d<T>& c) {
  const sd::core::Vector3d<T> v1(b, a);
  const sd::core::Vector3d<T> v2(b, c);
  return sd::core::angle(v1, v2);
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::angle(const Vector3d<T>& v1,
		const Vector3d<T>& v2,
		const Vector3d<T>& normal) {
  if (sd::core::angle(v1, v2) == 0.)
    return 0.;
  else 
    if (((v1 ^ v2) * normal) > 0)
      return sd::core::acos((v1/(T)v1.norm()) * (v2/(T)v2.norm()));
    else
      return 2 * M_PI - sd::core::acos((v1/(T)v1.norm()) * (v2/(T)v2.norm()));
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::area(const sd::core::Point3d<T>& a,
	       const sd::core::Point3d<T>& b,
	       const sd::core::Point3d<T>& c) {
  // Using Heron formula
  const sd::core::Vector3d<T> ca(c, a);
  const sd::core::Vector3d<T> ab(a, b);
  const sd::core::Vector3d<T> bc(b, c);
  const typename sd::core::DataType_<T>::work_type s = (ab.norm() + bc.norm() + ca.norm()) / 2.;
  return sqrt(s * (s-ab.norm()) * (s-bc.norm()) * (s-ca.norm()));
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::voronoiArea(const sd::core::Point3d<T>& target,
		      const sd::core::Point3d<T>& prev,
		      const sd::core::Point3d<T>& current,
		      const sd::core::Point3d<T>& next) {
  const typename sd::core::DataType_<T>::work_type alphaij = angle(current, prev, target);
  const typename sd::core::DataType_<T>::work_type betaij = angle(current, next, target);
  const typename sd::core::DataType_<T>::work_type distance = euclideanDistance(target, current);
  return (1/tan(alphaij) + 1/tan(betaij)) * distance * distance;
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::voronoiArea(const sd::core::Point3d<T>& a,
		      const sd::core::Point3d<T>& b,
		      const sd::core::Point3d<T>& c) {
  const Point3d<T> c1((b.x() + a.x())/2.,
		      (b.y() + a.y())/2.,
		      (b.z() + a.z())/2.);
  const Point3d<T> c2((b.x() + c.x())/2.,
		      (b.y() + c.y())/2.,
		      (b.z() + c.z())/2.);
  const Point3d<T> cc(circumcenter(a, b, c));
  return area(c1, b, cc) + area(c2, b, cc);
}

template<typename T>
sd::core::Point3d<T>
sd::core::circumcenter(const sd::core::Point3d<T>& a,
		       const sd::core::Point3d<T>& b,
		       const sd::core::Point3d<T>& c) {
  const Point3d<T> c1((b.x() + a.x())/2.,
		      (b.y() + a.y())/2.,
		      (b.z() + a.z())/2.);
  const Point3d<T> c2((b.x() + c.x())/2.,
		      (b.y() + c.y())/2.,
		      (b.z() + c.z())/2.);
  const sd::core::Vector3d<T> v1(b, a);
  const sd::core::Vector3d<T> v2(b, c);
  const sd::core::Vector3d<T> normal = v1 ^ v2;
  const sd::core::Vector3d<T> orthogonal1 = normal ^ v1;
  const sd::core::Vector3d<T> orthogonal2 = normal ^ v2;
  return sd::core::Line3d<T>(c1, orthogonal1).intersection(sd::core::Line3d<T>(c2, orthogonal2));
}

template<typename T>
sd::core::Vector3d<T>
sd::core::computeRotation(const T angle,
			  const typename sd::core::Vector3d<T>& v1,
			  const typename sd::core::Vector3d<T>& axe) {
  const sd::core::Vector3d<T> e2 = axe ^ v1;
  const sd::core::Vector3d<T> e1 = e2 ^ axe;
  const T planNorm = v1 * e1;
  const T planDistance = v1 * axe;
  return (((T)cos(angle) * e1 + (T)sin(angle) * e2) * planNorm)
    + (axe.normalized() * planDistance);
}

#endif /*! SD_CORE_MATH_HXX */
