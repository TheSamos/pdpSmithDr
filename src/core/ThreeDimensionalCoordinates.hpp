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

#ifndef SD_CORE_THREE_DIMENSIONAL_COORDINATES_HPP
#define SD_CORE_THREE_DIMENSIONAL_COORDINATES_HPP

#include "NDimensionalCoordinates.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    class ThreeDimensionalCoordinates : public NDimensionalCoordinates<T, 3> {

    public:
      
      ThreeDimensionalCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove);

      ThreeDimensionalCoordinates(const T& px, const T& py, const T& pz);

      explicit ThreeDimensionalCoordinates(const ThreeDimensionalCoordinates<T>& p);

      virtual ~ThreeDimensionalCoordinates();
      
      inline const T& x() const;
      inline const T& y() const;
      inline const T& z() const;
      
      inline const sd::core::ThreeDimensionalCoordinates<T>& set(const T& x, const T& y, const T& z);
      inline const T& setX(const T& px);
      inline const T& setY(const T& py);
      inline const T& setZ(const T& pz);
      
    };

  }
  
}

#include "ThreeDimensionalCoordinates.hxx"

#endif /*! SD_CORE_THREE_DIMENSIONAL_COORDINATES_HPP */
