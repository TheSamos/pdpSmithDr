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

#ifndef SD_CORE_LINE3D_HXX
#define SD_CORE_LINE3D_HXX

#ifdef DEBUG
#include <iostream>
#endif

template<typename T>
bool
sd::core::Line3d<T>::isCoplanar(const Line3d<T>& line) const {
  if (sd::core::Vector3d<T>::areCollinear(m_direction, line.m_direction))
    return true;
  const Point3d<T>& p = m_point;
  const Point3d<T>& q = line.m_point;
  const Vector3d<T>& v1 = m_direction;
  const Vector3d<T>& v2 = line.m_direction;
  
  const T k = (!(sd::core::about_equals<T>((v1.x()*v2.y() - v1.y()*v2.x()), 0.))
	       ? ((v2.y()*(q.x()-p.x()) - v2.x()*(q.y()-p.y())) / (v1.x()*v2.y() - v1.y()*v2.x()))
	       : ((!(sd::core::about_equals<T>((v1.x()*v2.z() - v1.z()*v2.x()), 0.))
		   ? ((v2.z()*(q.x()-p.x()) - v2.x()*(q.z()-p.z())) / (v1.x()*v2.z() - v1.z()*v2.x()))
		   : ((v2.y()*(q.z()-p.z()) - v2.z()*(q.y()-p.y())) / (v1.z()*v2.y() - v1.y()*v2.z())))));
	       
  const T l = (!(sd::core::about_equals<T>(v2.z(), 0.))
	       ? (p.z() - q.z() + k * v1.z()) / v2.z()
	       : ((v2.y() != 0.)
		  ? (p.y() - q.y() + k * v1.y()) / v2.y()
		  : (p.x() - q.x() + k * v1.x()) / v2.x()));
  
  return (sd::core::about_equals(p.x() + k * v1.x(), q.x() + l * v2.x(), (T).000000001)
	  && sd::core::about_equals(p.y() + k * v1.y(), q.y() + l * v2.y(), (T).000000001)
	  && sd::core::about_equals(p.z() + k * v1.z(), q.z() + l * v2.z(), (T).000000001)); // ???
}

template<typename T>
inline bool
sd::core::Line3d<T>::isSame(const sd::core::Line3d<T>& line) const {
  return (*this).contains(line.m_point)
    && areCollinear(line.m_direction, m_direction);
}

template<typename T>
bool
sd::core::Line3d<T>::contains(const sd::core::Point3d<T>& point) const {
  return point == m_point
    || areCollinear(sd::core::Vector3d<T>(m_point, point), m_direction);
}

template<typename T>
sd::core::Point3d<T>
sd::core::Line3d<T>::intersection(const Line3d<T>& line) const {
  const Point3d<T>& p = m_point;
  const Point3d<T>& q = line.m_point;
  const Vector3d<T>& v1 = m_direction;
  const Vector3d<T>& v2 = line.m_direction;
  
  const T k = (!(sd::core::about_equals<T>((v1.x()*v2.y() - v1.y()*v2.x()), 0.))
	       ? ((v2.y()*(q.x()-p.x()) - v2.x()*(q.y()-p.y())) / (v1.x()*v2.y() - v1.y()*v2.x()))
	       : ((!(sd::core::about_equals<T>((v1.x()*v2.z() - v1.z()*v2.x()), 0.))
		   ? ((v2.z()*(q.x()-p.x()) - v2.x()*(q.z()-p.z())) / (v1.x()*v2.z() - v1.z()*v2.x()))
		   : ((v2.y()*(q.z()-p.z()) - v2.z()*(q.y()-p.y())) / (v1.z()*v2.y() - v1.y()*v2.z())))));
	       
#ifdef DEBUG
  const T l = (!(sd::core::about_equals<T>(v2.z(), 0.))
	       ? (p.z() - q.z() + k * v1.z()) / v2.z()
	       : ((v2.y() != 0.)
		  ? (p.y() - q.y() + k * v1.y()) / v2.y()
		  : (p.x() - q.x() + k * v1.x()) / v2.x()));
  
  if (!(sd::core::about_equals<T>(p.x() + k * v1.x(), q.x() + l * v2.x(), (T).0001)
	&& sd::core::about_equals<T>(p.y() + k * v1.y(), q.y() + l * v2.y(), (T).0001)
	&& sd::core::about_equals<T>(p.z() + k * v1.z(), q.z() + l * v2.z(), (T).0001)))
    std::cerr << "Error : " << p.x() + k * v1.x() << ", " << q.x() + l * v2.x()
	      << " " << p.y() + k * v1.y() << ", " << q.y() + l * v2.y()
	      << " " << p.z() + k * v1.z() << ", " << q.z() + l * v2.z() << std::endl; // ???
  
#endif

  return Point3d<T>(p.x() + k * v1.x(),  p.y() + k * v1.y(), p.z() + k * v1.z());
}

template<typename T>
sd::core::Point3d<T>
sd::core::Line3d<T>::orthogonalProjection(const Point3d<T>& point) const {
  if (contains(point))
    return point;
  const sd::core::Vector3d<T>& v1 = m_direction;
  sd::core::Vector3d<T> v2(m_point, point);
  v2.normalize();
  const sd::core::Vector3d<T> normal = v1 ^ v2;
  assert(about_equals((T) (v1 * normal), (T) 0.));
  const sd::core::Vector3d<T> orthogonal = normal ^ v1;
  assert(about_equals((T) (v1 * orthogonal), (T) 0.));
  return (*this).intersection(Line3d<T>(point, orthogonal));
}

template<typename T>
T
sd::core::Line3d<T>::euclideanDistance(const Point3d<T>& point) const {
  return Vector3d<T>(point, orthogonalProjection(point)).norm();
}

template<typename T>
sd::core::Line3d<T>&
sd::core::Line3d<T>::operator+=(const sd::core::Vector3d<T>& vector) {
  m_point += vector;
}
      

#endif /*! SD_CORE_LINE3D_HPP */
