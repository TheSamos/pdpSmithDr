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

#ifndef SD_CORE_BOUNDING_BOX_HXX
#define SD_CORE_BOUNDING_BOX_HXX

#include "Math.hpp"

template<typename T>
sd::core::BoundingBox<T>::BoundingBox()
  : m_corner0(),
    m_corner1(),
    m_isValid(false)
{}

template<typename T>
sd::core::BoundingBox<T>::BoundingBox(const T& x, const T& y, const T& z,
                                      const T& w, const T& h, const T& d)
  : m_corner0(x, y, z),
    m_corner1(x+w, y+h, z+d),
    m_isValid(true)
{}

template<typename T>
sd::core::BoundingBox<T>::BoundingBox(const Point3d<T>& origin,
                                      const T& w, const T& h, const T& d)
  : m_corner0(origin),
    m_corner1(m_corner0.x()+w, m_corner0.y()+h, m_corner0.z()+d),
    m_isValid(true)
{}

template<typename T>
sd::core::BoundingBox<T>::BoundingBox(const Point3d<T>& corner0,
                                      const Point3d<T>& corner1)
  : m_corner0(corner0),
    m_corner1(corner1),
    m_isValid(true)
{}

template<typename T>
sd::core::BoundingBox<T>::BoundingBox(const BoundingBox<T>& bb)
  : m_corner0(bb.m_corner0),
    m_corner1(bb.m_corner1),
    m_isValid(bb.m_isValid)
{}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::corner0() const {
  return m_corner0;
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::corner1() const {
  return m_corner1;
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::setCorner0(const sd::core::Point3d<T>& p) {
  m_corner0 = p;
  if (!isValid()) {
    m_corner1 = m_corner0;
    m_isValid = true;
  }
  return m_corner0;
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::setCorner1(const sd::core::Point3d<T>& p) {
  m_corner1 = p;
  if (!isValid()) {
    m_corner0 = m_corner1;
    m_isValid = true;
  }
  return m_corner1;
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::origin() const {
  return m_corner0;
}

template<typename T>
T
sd::core::BoundingBox<T>::x() const {
  return m_corner0.x();
}

template<typename T>
T
sd::core::BoundingBox<T>::y() const {
  return m_corner0.y();
}

template<typename T>
T
sd::core::BoundingBox<T>::z() const {
  return m_corner0.z();
}

template<typename T>
T
sd::core::BoundingBox<T>::width() const {
  return m_corner1.x() - m_corner0.x();
}

template<typename T>
T
sd::core::BoundingBox<T>::height() const {
  return m_corner1.y() - m_corner0.y();
}

template<typename T>
T
sd::core::BoundingBox<T>::depth() const {
  return m_corner1.z() - m_corner0.z();
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::BoundingBox<T>::setOrigin(const sd::core::Point3d<T>& p) {
  sd::core::Vector3d<T> v(m_corner0, p);
  m_corner0 += v;
  m_corner1 += v;
  return m_corner0;
}

template<typename T>
void
sd::core::BoundingBox<T>::setWidth(T w) {
  m_corner1.setX(m_corner0.x() + w);
}

template<typename T>
void
sd::core::BoundingBox<T>::setHeight(T h) {
  m_corner1.setY(m_corner0.y() + h);
}

template<typename T>
void
sd::core::BoundingBox<T>::setDepth(T d) {
  m_corner1.setZ(m_corner0.z() + d);
}

template<typename T>
void
sd::core::BoundingBox<T>::add(const sd::core::Point3d<T>& p) {
  if (!isValid()) {
    m_corner0 = m_corner1 = p;
    m_isValid = true;
    return;
  }

  if (m_corner0.x()>p.x())
    m_corner0.setX(p.x());
  if (m_corner0.y()>p.y())
    m_corner0.setY(p.y());
  if (m_corner0.z()>p.z())
    m_corner0.setZ(p.z());

  if (m_corner1.x()<p.x())
    m_corner1.setX(p.x());
  if (m_corner1.y()<p.y())
    m_corner1.setY(p.y());
  if (m_corner1.z()<p.z())
    m_corner1.setZ(p.z());
}

template<typename T>
bool
sd::core::BoundingBox<T>::contains(const sd::core::Point3d<T>& p) const {
  return  m_isValid && ((m_corner0.x()<=p.x() || about_equals(m_corner0.x(), p.x()))
			&& (m_corner0.y()<=p.y() || about_equals(m_corner0.y(), p.y()))
			&& (m_corner0.z()<=p.z() || about_equals(m_corner0.z(), p.z()))
			&& (m_corner1.x()>=p.x() || about_equals(m_corner1.x(), p.x()))
			&& (m_corner1.y()>=p.y() || about_equals(m_corner1.y(), p.y()))
			&& (m_corner1.z()>=p.z() || about_equals(m_corner1.z(), p.z())));
}

template<typename T>
bool
sd::core::BoundingBox<T>::intersects(const sd::core::BoundingBox<T>& bb) const {
  if (!m_isValid || !bb.isValid())
    return false;
  
  Point3d<T> corner0(std::max(m_corner0.x(), bb.corner0().x()),
		     std::max(m_corner0.y(), bb.corner0().y()),
		     std::max(m_corner0.z(), bb.corner0().z()));
  
  Point3d<T> corner1(std::min(m_corner1.x(), bb.corner1().x()),
		     std::min(m_corner1.y(), bb.corner1().y()),
		     std::min(m_corner1.z(), bb.corner1().z()));
  
  return (corner0 != corner1);
}

template<typename T>
bool
sd::core::BoundingBox<T>::includes(const sd::core::BoundingBox<T>& bb) const {
  return  m_isValid
    && bb.m_isValid &&((m_corner0.x()<=bb.m_corner0.x())
		       && (m_corner0.y()<=bb.m_corner0.y())
		       && (m_corner0.z()<=bb.m_corner0.z())
		       && (m_corner1.x()>=bb.m_corner1.x())
		       && (m_corner1.y()>=bb.m_corner1.y())
		       && (m_corner1.z()>=bb.m_corner1.z()));
}

template<typename T>
bool
sd::core::BoundingBox<T>::isValid() const {
  return m_isValid;
}

#endif /*! SD_CORE_BOUNDING_BOX_HXX */
