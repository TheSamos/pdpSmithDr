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

#ifndef SD_CORE_RGB_COORDINATES_HXX
#define SD_CORE_RGB_COORDINATES_HXX

template<typename T>
sd::core::RGBCoordinates<T>::RGBCoordinates(sd::core::Vector<T>* data, unsigned int index,
					    bool deleteOnRemove)
  : NDimensionalCoordinates<T, 4>(data, index, deleteOnRemove) {
  assert(data);
}

template<typename T>
sd::core::RGBCoordinates<T>::RGBCoordinates(const T& cr, const T& cg, const T& cb, const T& ca)
  : NDimensionalCoordinates<T, 4>(cr, cg, cb, ca) {}
/*
template<typename T>
sd::core::RGBCoordinates<T>::RGBCoordinates(const RGBCoordinates<T>& c)
  : NDimensionalCoordinates<T, 4>((sd::core::Vector<T>*)NULL, (unsigned int)0, true) {
  Coordinates<T>::nocopy(c);
}
*/

template<typename T>
sd::core::RGBCoordinates<T>::RGBCoordinates(const RGBCoordinates<T>& c)
  : NDimensionalCoordinates<T, 4>((const NDimensionalCoordinates<T, 4>&)c) {}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::r() const {
  return sd::core::NDimensionalCoordinates<T, 4>::getAt(0);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::g() const {
  return sd::core::NDimensionalCoordinates<T, 4>::getAt(1);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::b() const {
  return sd::core::NDimensionalCoordinates<T, 4>::getAt(2);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::a() const {
  return sd::core::NDimensionalCoordinates<T, 4>::getAt(3);
}

template<typename T>
const sd::core::RGBCoordinates<T>&
sd::core::RGBCoordinates<T>::set(const T& cr, const T& cg, const T& cb, const T& ca) {
  setAt(0, cr);
  setAt(1, cg);
  setAt(2, cb);
  setAt(3, ca);
  return *this;
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::setR(const T& cr) {
  return setAt(0, cr);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::setG(const T& cg) {
  return setAt(1, cg);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::setB(const T& cb) {
  return setAt(2, cb);
}

template<typename T>
const T&
sd::core::RGBCoordinates<T>::setA(const T& ca) {
  return setAt(3, ca);
}

template<typename T>
const sd::core::RGBCoordinates<T>&
sd::core::RGBCoordinates<T>::operator=(const sd::core::RGBCoordinates<T>& c) {
  Coordinates<T>::nocopy(c);//sd::core::NDimensionalCoordinates<T, 4>::set(c.r(), c.g(), c.b(), c.a());
  return *this;
}

#endif /* ! SD_CORE_RGB_COORDINATES_HXX */
