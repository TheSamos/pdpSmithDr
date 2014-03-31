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

#ifndef SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HPP
#define SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HPP

#include <core/Coordinates.hpp>

namespace sd {
  
  namespace core {
    
    template<typename T>
    class StaticDimensionalCoordinates : public Coordinates<T> {
      
    public:
      
      StaticDimensionalCoordinates(sd::core::Vector<T>* data, unsigned int index, unsigned int size);
      
      template<typename ...VALUES>
      StaticDimensionalCoordinates(VALUES ...values);
      
      StaticDimensionalCoordinates(StaticDimensionalCoordinates& sdc);
      
      StaticDimensionalCoordinates(const StaticDimensionalCoordinates& sdc);

      virtual ~StaticDimensionalCoordinates() {}
      
      virtual unsigned int nbDims() const {return m_size;}
     
      unsigned int size() const {return m_size;}
      
      const StaticDimensionalCoordinates<T>& operator=(const StaticDimensionalCoordinates& sdc);

      void copyTo(T*) const;
      
    private:
      
      StaticDimensionalCoordinates() = delete;
      
    private:
      
      unsigned int m_size;
      
    };

  }

}

#include "StaticDimensionalCoordinates.hxx"

#endif /*! SD_CORE_STATIC_DIMENSIONAL_COORDINATES_HPP */
