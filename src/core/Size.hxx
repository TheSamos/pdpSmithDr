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

#ifndef SD_CORE_SIZE_HXX
#define SD_CORE_SIZE_HXX

#ifndef SD_CORE_POINT_HXX
#include <core/Point.hpp>
#endif /*! SD_CORE_POINT_HXX */

template<typename T>
T
sd::core::Size_<T>::dataSize() const {
  T len = 1;
  for (size_t i=0; i<DynamicDimensionalCoordinates<T>::nbDims(); ++i)
    len *= DynamicDimensionalCoordinates<T>::getAt(i);
  assert(len != 0);
  return len;
}

template<typename T>
T
sd::core::Size_<T>::getIndexOf(const sd::core::Point_<T>& p) const {
  T index = p[0];
  T sizeMult = 1;
  for (unsigned int i=1; i<p.nbDims(); ++i) {
    sizeMult *= (*this)[i-1];
    index += p[i] * sizeMult;
  }
  return index;
}

template<typename T>
void
sd::core::Size_<T>::getCoordinatesOf(T index, sd::core::Point_<T>& p) const {
  const unsigned int nbDims = (*this).nbDims();
  p.clear(nbDims);
  for (unsigned int i=0; i<nbDims; ++i) {
    const T s = (*this)[i];
    p.setAt(i, index % s);
    index /= s;
  }
}

#endif /*! SD_CORE_SIZE_HXX */
