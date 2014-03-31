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

#ifndef SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HXX
#define SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HXX

template<typename T>
sd::core::StaticDimensionalCoordinates<T>::StaticDimensionalCoordinates(sd::core::Vector<T>* data,
									unsigned int index,
									unsigned int size)
  : Coordinates<T>(data, index, false),
    m_size(size)
{}

template<typename T>
template<typename ...VALUES>
sd::core::StaticDimensionalCoordinates<T>::StaticDimensionalCoordinates(VALUES ...values)
  : Coordinates<T>(&m_size, std::forward<VALUES>(values)...) {}

template<typename T>
sd::core::StaticDimensionalCoordinates<T>::StaticDimensionalCoordinates(StaticDimensionalCoordinates& sdc)
  : Coordinates<T>((sd::core::Vector<T>*)NULL, 0, false),
    m_size(sdc.m_size) {
  Coordinates<T>::nocopy(sdc);
}

template<typename T>
sd::core::StaticDimensionalCoordinates<T>::StaticDimensionalCoordinates(const StaticDimensionalCoordinates& sdc)
  : Coordinates<T>((sd::core::Vector<T>*)NULL, 0, false),
    m_size(sdc.m_size) {
  Coordinates<T>::copy(m_size, sdc);
}

/*
template<typename T>
sd::core::StaticDimensionalCoordinates<T>::StaticDimensionalCoordinates(const StaticDimensionalCoordinates& sdc)
  : Coordinates<T>((sd::core::Vector<T>*)NULL, 0, false),
    m_size(sdc.m_size) {
  Coordinates<T>::nocopy(sdc);
}
*/
template<typename T>
void
sd::core::StaticDimensionalCoordinates<T>::copyTo(T* dest) const {
  Coordinates<T>::copyTo(m_size, dest);
}

template<typename T>
const sd::core::StaticDimensionalCoordinates<T>&
sd::core::StaticDimensionalCoordinates<T>::operator=(const sd::core::StaticDimensionalCoordinates<T>& c) {
  if (&c != this) {
    if (m_size == c.m_size)
      Coordinates<T>::nocopy(c);
    else
      Coordinates<T>::copy(m_size, c);
  }
  return *this;
}

#endif /*! SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HXX */
