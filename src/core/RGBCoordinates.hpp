/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_CORE_RGB_COORDINATES_HPP
#define SD_CORE_RGB_COORDINATES_HPP

#include "NDimensionalCoordinates.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    class RGBCoordinates : public NDimensionalCoordinates<T, 4> {

    public:

      RGBCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove = false);
      
      RGBCoordinates(const T& cr=0, const T& cg=0, const T& cb=0, const T& ca=255);
      
      RGBCoordinates(const RGBCoordinates<T>& c);
      
      virtual ~RGBCoordinates() {}
      
      inline const T& r() const;
      inline const T& g() const;
      inline const T& b() const;
      inline const T& a() const;

      inline const T& operator() (int i) const;
      inline const T& operator[] (int i) const;

      inline const sd::core::RGBCoordinates<T>& set(const T& cr=0, const T& cg=0,
						    const T& cb=0, const T& ca=255);
      inline const T& setR(const T& cr);
      inline const T& setG(const T& cg);
      inline const T& setB(const T& cb);
      inline const T& setA(const T& cb);
      
      const RGBCoordinates<T>& operator=(const RGBCoordinates<T>& c);
      
    };
    
  }

}

#include "RGBCoordinates.hxx"

#endif /*! SD_CORE_RGB_COORDINATES_HPP */
