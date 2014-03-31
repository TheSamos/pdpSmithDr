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

#ifndef SD_CORE_N_DIMENSIONAL_COORDINATES_HPP
#define SD_CORE_N_DIMENSIONAL_COORDINATES_HPP

#include "Coordinates.hpp"

#include <cstdarg>
#include <string>

namespace sd {
  
  namespace core {

    template<typename T, unsigned int N>
    class NDimensionalCoordinates : public Coordinates<T> {
      
    public:
      
      NDimensionalCoordinates();

      NDimensionalCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove);
      
      template<typename ...VALUES>
      NDimensionalCoordinates(VALUES ...values);
      
      explicit NDimensionalCoordinates(const NDimensionalCoordinates<T, N>& p);
      
      virtual ~NDimensionalCoordinates() {}
      
      virtual inline unsigned int nbDims() const {return N;}
      
      static inline unsigned int card() {return N;}
      
      inline const T& operator() (unsigned int i) const;
      inline const T& operator[] (unsigned int i) const;
      inline const T& getAt(unsigned int i) const;
      
      const NDimensionalCoordinates<T, N>& operator=(const NDimensionalCoordinates<T, N>& p);
      
      std::string toString() const;
      
    };
    
    template<typename T, unsigned int N>
    std::ostream& operator<<(std::ostream& os, const sd::core::NDimensionalCoordinates<T, N>& dc);

    template<typename T, unsigned int N>
    bool operator==(const sd::core::NDimensionalCoordinates<T, N>& dc1,
		    const sd::core::NDimensionalCoordinates<T, N>& dc2);

    template<typename T, unsigned int N>
    bool operator!=(const sd::core::NDimensionalCoordinates<T, N>& dc1,
		    const sd::core::NDimensionalCoordinates<T, N>& dc2);

  }
  
}

#include "NDimensionalCoordinates.hxx"

#endif /*! SD_CORE_N_DIMENSIONAL_COORDINATES_HPP */
