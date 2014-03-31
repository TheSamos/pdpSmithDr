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

#ifndef SD_CORE_PIXEL_HPP
#define SD_CORE_PIXEL_HPP

#include <core/StaticDimensionalCoordinates.hpp>

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Pixel : public StaticDimensionalCoordinates<T> {
      
    public:
      
      typedef T data_type;
      
    public:
      
      Pixel(sd::core::Vector<T>* data, unsigned int index, unsigned int size)
	: StaticDimensionalCoordinates<T>(data, index, size)
      {}
      
      Pixel(const Pixel<T>& p)
	: StaticDimensionalCoordinates<T>((const StaticDimensionalCoordinates<T>&)p)
      {}
      
      Pixel(Pixel<T>& p)
	: StaticDimensionalCoordinates<T>((StaticDimensionalCoordinates<T>&)p)
      {}
      
      template<typename ...VALUES>
      Pixel(VALUES ...values)
	: StaticDimensionalCoordinates<T>(std::forward<VALUES>(values)...)
      {}
      
      virtual ~Pixel() {}
      
      unsigned int nbChannels() const {return StaticDimensionalCoordinates<T>::size();}
      
    private:
      
      Pixel() = delete;
      
    };
    
    template<typename T>
    bool operator==(const Pixel<T>& p1, const Pixel<T>& p2) {
      if (p1.nbChannels() != p2.nbChannels())
	return false;
      for (unsigned int i=0; i<p1.nbChannels(); ++i)
	if (p1[i] != p2[i])
	  return false;
      return true;
    }
    
  }

}

#endif /*! SD_CORE_PIXEL_HPP */
