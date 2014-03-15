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

#ifndef SD_CORE_POINT3D_HPP
#define SD_CORE_POINT3D_HPP

#include "ThreeDimensionalCoordinates.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Vector3d;
    
    template<typename T>
    class Point3d : public ThreeDimensionalCoordinates<T> {

    public:

      Point3d(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove = false);

      Point3d(const T& px=0, const T& py=0, const T& pz=0);

      Point3d(const Point3d<T>& p);

      virtual ~Point3d() {}
      
      const Point3d<T>& operator+=(const sd::core::Vector3d<T>& v);
      const Point3d<T>& operator-=(const sd::core::Vector3d<T>& v);
      
    };
    
    template<typename T>
    Point3d<T> operator+(const sd::core::Point3d<T>& p, const sd::core::Vector3d<T>& v);
    
    template<typename T>
    Point3d<T> operator-(const sd::core::Point3d<T>& p, const sd::core::Vector3d<T>& v);
    
  }
  
}

#include "Point3d.hxx"

#endif /*! SD_CORE_POINT3D_HPP */
