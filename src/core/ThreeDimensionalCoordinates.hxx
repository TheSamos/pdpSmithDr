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

#ifndef SD_CORE_THREE_DIMENSIONAL_COORDINATES_HXX
#define SD_CORE_THREE_DIMENSIONAL_COORDINATES_HXX

#include <cassert>
#include <stdlib.h>

template<typename T>
sd::core::ThreeDimensionalCoordinates<T>::ThreeDimensionalCoordinates(sd::core::Vector<T>* data, unsigned int index,
								      bool deleteOnRemove)
  : NDimensionalCoordinates<T, 3>(data, index, deleteOnRemove) {}

template<typename T>
sd::core::ThreeDimensionalCoordinates<T>::ThreeDimensionalCoordinates(const T& px, const T& py, const T& pz)
  : NDimensionalCoordinates<T, 3>() {this->set(px, py, pz);}

template<typename T>
sd::core::ThreeDimensionalCoordinates<T>::ThreeDimensionalCoordinates(const ThreeDimensionalCoordinates<T>& p)
  : NDimensionalCoordinates<T, 3>(static_cast<const NDimensionalCoordinates<T, 3>&>(p)) {}

template<typename T>
sd::core::ThreeDimensionalCoordinates<T>::~ThreeDimensionalCoordinates() {}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::x() const {
  return Coordinates<T>::getAt(0);
}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::y() const {
  return Coordinates<T>::getAt(1);
}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::z() const {
  return Coordinates<T>::getAt(2);
}

template<typename T>
const sd::core::ThreeDimensionalCoordinates<T>&
sd::core::ThreeDimensionalCoordinates<T>::set(const T& px, const T& py, const T& pz) {
  this->setAt(0, px);
  this->setAt(1, py);
  this->setAt(2, pz);
  return *this;
}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::setX(const T& px) {
  return this->setAt(0, px);
}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::setY(const T& py) {
  return this->setAt(1, py);
}

template<typename T>
const T&
sd::core::ThreeDimensionalCoordinates<T>::setZ(const T& pz) {
  return this->setAt(2, pz);
}

#endif /*! SD_CORE_THREE_DIMENSIONAL_COORDINATES_HXX */
