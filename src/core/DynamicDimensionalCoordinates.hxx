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

#include <cassert>

#include <utils/sdTypeUtils.hpp>

template<typename T>
sd::core::DynamicDimensionalCoordinates<T>::DynamicDimensionalCoordinates(T defaultValue)
  : Coordinates<T>(new sd::core::Vector<T>(), 0, true),
    m_size(0),
    m_defaultValue(defaultValue) {}

template<typename T>
template<typename ...VALUES>
sd::core::DynamicDimensionalCoordinates<T>::DynamicDimensionalCoordinates(T defaultValue, VALUES ...values)
  : Coordinates<T>(&m_size, std::forward<VALUES>(values)...),
    m_defaultValue(defaultValue) {}

template<typename T>
sd::core::DynamicDimensionalCoordinates<T>::DynamicDimensionalCoordinates(const DynamicDimensionalCoordinates<T>& p)
  : Coordinates<T>((sd::core::Vector<T>*)NULL, 0, true),
    m_size(p.m_size),
    m_defaultValue(p.m_defaultValue) {
  Coordinates<T>::copy(m_size, p);
}

/*
template<typename T>
T&
sd::core::DynamicDimensionalCoordinates<T>::getAt(unsigned int n) {
  if (m_size <= n) {
    Coordinates<T>::resize(n+1, m_defaultValue);
    m_size = n+1;
  }
  return Coordinates<T>::getAt(n);
}
*/
template<typename T>
T
sd::core::DynamicDimensionalCoordinates<T>::getAt(unsigned int n) const {
  return (m_size > n) ? Coordinates<T>::getAt(n) : m_defaultValue;
}

template<typename T>
void
sd::core::DynamicDimensionalCoordinates<T>::setAt(unsigned int n, const T& value) {
  if (m_size <= n) {
    Coordinates<T>::resize(n+1, m_defaultValue);
    m_size = n+1;
  }
  Coordinates<T>::setAt(n, value);
}

template<typename T>
void
sd::core::DynamicDimensionalCoordinates<T>::push_back(const T& value) {
  Coordinates<T>::resize(++m_size, m_defaultValue);
  Coordinates<T>::setAt(m_size-1, value);
}

template<typename T>
void
sd::core::DynamicDimensionalCoordinates<T>::clear(unsigned int n) {
  m_size = n;
  if (n != 0) {
    Coordinates<T>::resize(n, m_defaultValue);
    Coordinates<T>::reset(n, m_defaultValue);
  }
}

template<typename T>
const sd::core::DynamicDimensionalCoordinates<T>&
sd::core::DynamicDimensionalCoordinates<T>::operator=(const sd::core::DynamicDimensionalCoordinates<T>& p) {
  m_size = p.m_size;
  m_defaultValue = p.m_defaultValue;
  Coordinates<T>::copy(m_size, p);
  return *this;
}

template<typename T>
std::string
sd::core::DynamicDimensionalCoordinates<T>::toString() const {
  std::string result = "";
  const unsigned int dims = this->nbDims();
  for (unsigned int i=0; i<dims; ++i) {
    if (i != 0)
      result = result + ", ";
    result = sd::utils::concatenation(result, (double)(*this)[i]);
  }
  return result;
}

template<typename T>
std::ostream&
sd::core::operator<<(std::ostream& os, const sd::core::DynamicDimensionalCoordinates<T>& dc) {
  os << dc.toString();
  return os;
}

template<typename T>
bool
sd::core::operator==(const sd::core::DynamicDimensionalCoordinates<T>& dc1,
		     const sd::core::DynamicDimensionalCoordinates<T>& dc2) {
  bool equals = dc1.nbDims() == dc1.nbDims();
  const unsigned int dims = dc1.nbDims();
  for (unsigned int i=0; i<dims; ++i)
    equals &= dc1[i] == dc2[i];
  return equals;
}

template<typename T>
bool
sd::core::operator!=(const sd::core::DynamicDimensionalCoordinates<T>& dc1,
		     const sd::core::DynamicDimensionalCoordinates<T>& dc2) {
  return !(dc1 == dc2);
}
