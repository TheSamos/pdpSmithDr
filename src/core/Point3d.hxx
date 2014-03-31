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

#ifndef SD_CORE_POINT3D_HXX
#define SD_CORE_POINT3D_HXX

#include "Math.hpp"

template<typename T>
sd::core::Point3d<T>::Point3d(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove)
  : ThreeDimensionalCoordinates<T>(data, index, deleteOnRemove) {
}

template<typename T>
sd::core::Point3d<T>::Point3d(const T& px, const T& py, const T& pz)
  : ThreeDimensionalCoordinates<T>(px, py, pz) {
}

template<typename T>
sd::core::Point3d<T>::Point3d(const Point3d<T>& p)
  : ThreeDimensionalCoordinates<T>(p.x(), p.y(), p.z()) {
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::Point3d<T>::operator+=(const Vector3d<T>& v) {
  (*this).set((*this).x() + v.x(), (*this).y() + v.y(), (*this).z() + v.z());
  return *this;
}

template<typename T>
const sd::core::Point3d<T>&
sd::core::Point3d<T>::operator-=(const Vector3d<T>& v) {
  (*this).set((*this).x() - v.x(), (*this).y() - v.y(), (*this).z() - v.z());
  return *this;
}

template<typename T>
sd::core::Point3d<T>
sd::core::operator+(const sd::core::Point3d<T>& p, const sd::core::Vector3d<T>& v) {
  sd::core::Point3d<T> result(p);
  result += v;
  return result;
}

template<typename T>
sd::core::Point3d<T>
sd::core::operator-(const sd::core::Point3d<T>& p, const sd::core::Vector3d<T>& v) {
  sd::core::Point3d<T> result(p);
  result -= v;
  return result;
}
     
#endif /*! SD_CORE_POINT3D_HXX */
