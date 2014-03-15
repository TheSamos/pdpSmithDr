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

#ifndef SD_CORE_POINT_HPP
#define SD_CORE_POINT_HPP

#include "DynamicDimensionalCoordinates.hpp"

#include <iostream>
#include <vector>

namespace sd {
  
  namespace core {

    template<typename T>
    class Size_;
    
    template<typename T>
    class Point_ : public DynamicDimensionalCoordinates<T> {

    public:
      
      template<typename ...VALUES>
      Point_(VALUES ...values) : DynamicDimensionalCoordinates<T>((T)0, std::forward<VALUES>(values)...) {}
      
      virtual ~Point_() {}
      
      inline T x() const {return (*this).getAt(0);}
      inline T y() const {return (*this).getAt(1);}
      inline T z() const {return (*this).getAt(2);}
      inline T t() const {return (*this).getAt(3);}

      inline void setX(const T& val) {(*this).setAt(0, val);}
      inline void setY(const T& val) {(*this).setAt(1, val);}
      inline void setZ(const T& val) {(*this).setAt(2, val);}
      inline void setT(const T& val) {(*this).setAt(3, val);}
    
      Point_<T>& operator%=(const Size_<T>& size);
      
    };
    
  }
  
  typedef sd::core::Point_<unsigned int> Point;
  typedef sd::core::Point_<float> PointF;
  
}

#endif /*! SD_CORE_POINT_HPP */
