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

#ifndef SD_CORE_TEXTURE_COORDINATES_HXX
#define SD_CORE_TEXTURE_COORDINATES_HXX

template<typename T>
sd::core::TextureCoordinates<T>::TextureCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove)
  : ThreeDimensionalCoordinates<T>(data, index, deleteOnRemove) {
}
      
template<typename T>
sd::core::TextureCoordinates<T>::TextureCoordinates(const T& pu, const T& pv, const T& pw)
  : ThreeDimensionalCoordinates<T>(pu, pv, pw) {
}
      
template<typename T>
sd::core::TextureCoordinates<T>::TextureCoordinates(const TextureCoordinates<T>& t)
  : ThreeDimensionalCoordinates<T>((const ThreeDimensionalCoordinates<T>&) t) {
}

template<typename T>
const T&
sd::core::TextureCoordinates<T>::u() const {
  return sd::core::ThreeDimensionalCoordinates<T>::x();
}

template<typename T>
const T&
sd::core::TextureCoordinates<T>::v() const {
  return sd::core::ThreeDimensionalCoordinates<T>::y();
}

template<typename T>
const T&
sd::core::TextureCoordinates<T>::w() const {
  return sd::core::ThreeDimensionalCoordinates<T>::z();
}
template<typename T>
const T&
sd::core::TextureCoordinates<T>::operator() (int i) const {
  return sd::core::ThreeDimensionalCoordinates<T>::getAt(i);
}

template<typename T>
const T&
sd::core::TextureCoordinates<T>::operator[] (int i) const {
  return sd::core::ThreeDimensionalCoordinates<T>::getAt(i);
}

template<typename T>
const sd::core::TextureCoordinates<T>&
sd::core::TextureCoordinates<T>::set(const T& pu, const T& pv, const T& pw) {
  sd::core::ThreeDimensionalCoordinates<T>::set(pu, pv, pw);
  return *this;
}

template<typename T>
T&
sd::core::TextureCoordinates<T>::setU(const T& pu) {
  return sd::core::ThreeDimensionalCoordinates<T>::setX(pu);
}

template<typename T>
T&
sd::core::TextureCoordinates<T>::setV(const T& pv) {
  return sd::core::ThreeDimensionalCoordinates<T>::setY(pv);
}

template<typename T>
T&
sd::core::TextureCoordinates<T>::setW(const T& pw) {
  return sd::core::ThreeDimensionalCoordinates<T>::setZ(pw);
}

template<typename T>
const sd::core::TextureCoordinates<T>&
sd::core::TextureCoordinates<T>::operator=(const sd::core::TextureCoordinates<T>& t) {
  sd::core::ThreeDimensionalCoordinates<T>::set(t.u(), t.v(), t.w());
  return *this;
}

#endif /*! SD_CORE_TEXTURE_COORDINATES_HXX */
