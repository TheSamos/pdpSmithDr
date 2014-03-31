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

#ifndef SD_CORE_SIZE_HPP
#define SD_CORE_SIZE_HPP

#include "DynamicDimensionalCoordinates.hpp"

#include <iostream>
#include <vector>

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Point_;
    
    template<typename T>
    class Size_ : public DynamicDimensionalCoordinates<T> {

    public:
      
      template<typename ...VALUES>
      Size_(VALUES ...values) : DynamicDimensionalCoordinates<T>((T)1, std::forward<VALUES>(values)...) {}
      
      template<typename ...VALUES>
      Size_(const Size_& size_) : DynamicDimensionalCoordinates<T>(size_) {}
      
      virtual ~Size_() {}
      
      inline T width() const {return (*this).getAt(0);}
      inline T height() const {return (*this).getAt(1);}
      inline T depth() const {return (*this).getAt(2);}
      inline T time() const {return (*this).getAt(3);}
      
      inline void setWidth(const T& val) {(*this).setAt(0, val);}
      inline void setHeight(const T& val) {(*this).setAt(1, val);}
      inline void setDepth(const T& val) {(*this).setAt(2, val);}
      inline void setTime(const T& val) {(*this).setAt(3, val);}
      
      inline T dataSize() const;
      
      inline T getIndexOf(const sd::core::Point_<T>& p) const;
      
      inline void getCoordinatesOf(T index, sd::core::Point_<T>& p) const;
      
    };
        
  }
  
  typedef sd::core::Size_<unsigned int> Size;
  typedef sd::core::Size_<float> SizeF;
    
  typedef sd::SizeF Resolution;
  typedef sd::SizeF Thickness;
  
}

#include "Size.hxx"

#endif /*! SD_CORE_SIZE_HPP */
