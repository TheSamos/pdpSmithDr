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

#ifndef SD_CORE_N_DIMENSIONAL_COORDINATES_HXX
#define SD_CORE_N_DIMENSIONAL_COORDINATES_HXX

#include <cassert>
#include <stdlib.h>
#include <cstring>

#include <utils/sdTypeUtils.hpp>

template<typename T, unsigned int N>
sd::core::NDimensionalCoordinates<T, N>::NDimensionalCoordinates()
  : Coordinates<T>(new sd::core::Vector<T>(N), 0, true) {}

template<typename T, unsigned int N>
sd::core::NDimensionalCoordinates<T, N>::NDimensionalCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove)
  : Coordinates<T>(data, index, deleteOnRemove) {}

template<typename T, unsigned int N>
sd::core::NDimensionalCoordinates<T, N>::NDimensionalCoordinates(const NDimensionalCoordinates<T, N>& p)
  : Coordinates<T>((sd::core::Vector<T>*)NULL, 0, true) {
  //Coordinates<T>::copy(N, p);
  Coordinates<T>::nocopy(p);
}

template<typename T, unsigned int N>
const sd::core::NDimensionalCoordinates<T, N>&
sd::core::NDimensionalCoordinates<T, N>::operator=(const sd::core::NDimensionalCoordinates<T, N>& p) {
  Coordinates<T>::copy(N, p);
  ///Coordinates<T>::nocopy(p);
  return *this;
}

template<typename T, unsigned int N>
template<typename ...VALUES>
sd::core::NDimensionalCoordinates<T, N>::NDimensionalCoordinates(VALUES ...values)
  : Coordinates<T>(N, std::forward<VALUES>(values)...) {}

template<typename T, unsigned int N>
const T&
sd::core::NDimensionalCoordinates<T, N>::operator() (unsigned int i) const {
  assert(i<N);
  return Coordinates<T>::getAt(i);
}

template<typename T, unsigned int N>
const T&
sd::core::NDimensionalCoordinates<T, N>::operator[] (unsigned int i) const {
  assert(i<N);
  return Coordinates<T>::getAt(i);
}

template<typename T, unsigned int N>
const T&
sd::core::NDimensionalCoordinates<T, N>::getAt(unsigned int i) const {
  assert(i<N);
  return Coordinates<T>::getAt(i);
}

template<typename T, unsigned int N>
std::string
sd::core::NDimensionalCoordinates<T, N>::toString() const {
  std::string result = "";
  for (unsigned int i=0; i<N; ++i) {
    if (i != 0)
      result = result + ", ";
    result = sd::utils::concatenation(result, (double)(*this)[i]);
  }
  return result;
}

template<typename T, unsigned int N>
std::ostream&
sd::core::operator<<(std::ostream& os, const sd::core::NDimensionalCoordinates<T, N>& dc) {
  os << dc.toString();
  return os;
}

template<typename T, unsigned int N>
bool
sd::core::operator==(const sd::core::NDimensionalCoordinates<T, N>& dc1,
		     const sd::core::NDimensionalCoordinates<T, N>& dc2) {
  bool equals = true;
  for (unsigned int i=0; i<N; ++i)
    equals &= dc1[i] == dc2[i];
  return equals;
}

template<typename T, unsigned int N>
bool
sd::core::operator!=(const sd::core::NDimensionalCoordinates<T, N>& dc1,
		     const sd::core::NDimensionalCoordinates<T, N>& dc2) {
  return !(dc1 == dc2);
}

#endif /*! SD_CORE_N_DIMENSIONAL_COORDINATES_HXX */
