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

#ifndef SD_CORE_VECTOR3D_HXX
#define SD_CORE_VECTOR3D_HXX

template<typename T>
sd::core::Vector3d<T>::Vector3d(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove)
  : ThreeDimensionalCoordinates<T>(data, index, deleteOnRemove)
{}

template<typename T>
sd::core::Vector3d<T>::Vector3d(const T& xval, const T& yval, const T& zval)
  : ThreeDimensionalCoordinates<T>(xval, yval, zval) {
}

template<typename T>
template<typename U>
sd::core::Vector3d<T>::Vector3d(const Point3d<U>& p1, const Point3d<U>& p2)
  : ThreeDimensionalCoordinates<T>(p2.x()-p1.x(), p2.y()-p1.y(), p2.z()-p1.z()) {
}
      
template<typename T>
sd::core::Vector3d<T>::Vector3d(const Vector3d<T>& v)
  : ThreeDimensionalCoordinates<T>(v) {
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::Vector3d<T>::angle(const sd::core::Vector3d<T>& v) const {
  return angle(*this, v);
}

template<typename T>
bool
sd::core::Vector3d<T>::isCollinear(const sd::core::Vector3d<T>& v) const {
  return areCollinear(*this, v);
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::Vector3d<T>::norm() const {
  return sd::core::euclideanDistance(Point3d<T>((T)0, (T)0, (T)0),
				     Point3d<T>((T)this->x(), (T)this->y(), (T)this->z()));
}

template<typename T>
void
sd::core::Vector3d<T>::normalize() {
  if (norm() != 0.)
    *this /= (T)norm();
}

template<typename T>
sd::core::Vector3d<T>
sd::core::Vector3d<T>::normalized() const {
  if (norm() != 0.)
    return (*this) / (T)this->norm();
  return *this;
}

template<typename T>
void
sd::core::Vector3d<T>::cross(const Vector3d<T>& v, Vector3d<T>& result) const {
  result.setX((*this)[1]*v[2] - (*this)[2]*v[1]);
  result.setY((*this)[2]*v[0] - (*this)[0]*v[2]);
  result.setZ((*this)[0]*v[1] - (*this)[1]*v[0]);
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::Vector3d<T>::dot(const Vector3d<T>& v) const {
  return (*this)[0]*v[0] + (*this)[1]*v[1] + (*this)[2]*v[2];
}

template<typename T>
const sd::core::Vector3d<T>&
sd::core::Vector3d<T>::operator+=(T val) {
  this->setX((*this)[0]+val);
  this->setY((*this)[1]+val);
  this->setZ((*this)[2]+val);
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>&
sd::core::Vector3d<T>::operator-=(T val) {
  this->setX((*this)[0]-val);
  this->setY((*this)[1]-val);
  this->setZ((*this)[2]-val);
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>&
sd::core::Vector3d<T>::operator*=(T val) {
  this->setX((*this)[0]*val);
  this->setY((*this)[1]*val);
  this->setZ((*this)[2]*val);
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>&
sd::core::Vector3d<T>::operator/=(T val) {
  this->setX((*this)[0]/val);
  this->setY((*this)[1]/val);
  this->setZ((*this)[2]/val);
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>& 
sd::core::Vector3d<T>::operator+=(const Vector3d<T>& v) {
  *this = *this + v;
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>& 
sd::core::Vector3d<T>::operator-=(const Vector3d<T>& v) {
  *this = *this - v;
  return *this;
}

template<typename T>
const sd::core::Vector3d<T>& 
sd::core::Vector3d<T>::operator^=(const Vector3d<T>& v) {
  *this = *this ^ v;
  return *this;
}

template<typename T>
sd::core::Vector3d<T>
sd::core::operator+(const Vector3d<T>& v, T val) {
  Vector3d<T> res = v;
  res += val;
  return res;
}

template<typename T>
sd::core::Vector3d<T>
sd::core::operator-(const Vector3d<T>& v, T val) {
  Vector3d<T> res = v;
  res -= val;
  return res;
}
    
template<typename T>
sd::core::Vector3d<T>
sd::core::operator*(const Vector3d<T>& v, T val) {
  Vector3d<T> res = v;
  res *= val;
  return res;
}
    
template<typename T>
sd::core::Vector3d<T>
sd::core::operator/(const Vector3d<T>& v, T val) {
  assert (val != 0);
  Vector3d<T> res = v;
  res /= val;
  return res;
}
  
template<typename T>
sd::core::Vector3d<T>
sd::core::operator+(const sd::core::Vector3d<T>& v1,
		    const sd::core::Vector3d<T>& v2) {
  return sd::core::Vector3d<T>(v1.x()+v2.x(),
			       v1.y()+v2.y(),
			       v1.z()+v2.z());
}
    
template<typename T>
sd::core::Vector3d<T>
sd::core::operator-(const sd::core::Vector3d<T>& v1,
		    const sd::core::Vector3d<T>& v2) {
  return sd::core::Vector3d<T>(v1.x()-v2.x(),
			       v1.y()-v2.y(),
			       v1.z()-v2.z());
}
    
template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::operator*(const sd::core::Vector3d<T>& v1,
		    const sd::core::Vector3d<T>& v2) {
  return v1.dot(v2);
}

template<typename T>
sd::core::Vector3d<T>
sd::core::operator^(const sd::core::Vector3d<T>& v1,
		    const sd::core::Vector3d<T>& v2) {
  Vector3d<T> result;
  v1.cross(v2, result);
  return result;
}

template<typename T>
typename sd::core::DataType_<T>::work_type
sd::core::angle(const sd::core::Vector3d<T>& v1,
		const sd::core::Vector3d<T>& v2) {
  assert(v1.norm() != 0 && v2.norm() != 0);
  return sd::core::acos((v1/(T)v1.norm()) * (v2/(T)v2.norm()));
}

template<typename T>
bool
sd::core::areCollinear(const sd::core::Vector3d<T>& v1,
		       const sd::core::Vector3d<T>& v2) {
  return about_equals(v1/(T)v1.norm(), v2/(T)v2.norm()/*, .0000000001*/)
    || about_equals(v1/(T)v1.norm(), v2/-((T)v2.norm())/*, .0000000001*/);
}

#endif /*! SD_CORE_VECTOR3D_HXX */
