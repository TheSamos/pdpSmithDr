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

#ifndef SD_CORE_POINT_HXX
#define SD_CORE_POINT_HXX

template<typename T>
sd::core::Point_<T>&
sd::core::Point_<T>::operator%=(const sd::core::Size_<T>& size); {
  const unsigned int nbDims = size.nbDims();
  for (unsigned int i=0; i<nbDims-1; ++i) {
    (*this)[i+1] += (*this)[i] % size[i];
  }
  
  T mod = (*this)[size-1] % size[size-1];
  if (mod != 0)
    (*this)[size] += mod;
    
  return *this;
}

#endif /*! SD_CORE_POINT_HXX */
