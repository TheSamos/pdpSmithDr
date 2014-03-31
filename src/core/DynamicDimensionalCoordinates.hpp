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

#ifndef SD_CORE_DYNAMIC_DIMENSIONAL_COORDINATES_HPP
#define SD_CORE_DYNAMIC_DIMENSIONAL_COORDINATES_HPP

#include "Coordinates.hpp"

#include <iostream>
#include <vector>

namespace sd {
  
  namespace core {

    template<typename T>
    class DynamicDimensionalCoordinates : public Coordinates<T> {

    public:
      
      DynamicDimensionalCoordinates(T defaultValue = 0.);
      
      template<typename ...VALUES>
      DynamicDimensionalCoordinates(T defaultValue, VALUES ...values);
      
      DynamicDimensionalCoordinates(const DynamicDimensionalCoordinates<T>& p);
      
      virtual ~DynamicDimensionalCoordinates() {}
      
      //virtual inline T& getAt(unsigned int n);
      virtual inline T getAt(unsigned int n) const;
      virtual inline void setAt(unsigned int n, const T& value);
      virtual inline void push_back(const T& value);
      
      virtual inline unsigned int nbDims() const {return m_size;}
      virtual inline unsigned int size() const {return nbDims();}
      
      //virtual inline T& operator[] (unsigned int n) {return (*this).getAt(n);}
      virtual inline T operator()(unsigned int n) const {return (*this).getAt(n);}
      virtual inline T operator[](unsigned int n) const {return (*this).getAt(n);}
      
      inline void clear(unsigned int n = 0);
      
      const DynamicDimensionalCoordinates<T>& operator=(const DynamicDimensionalCoordinates<T>& p);
      
      std::string toString() const;
      
    protected:
      
      void setDefaultValue(const T& value) {m_defaultValue = value;}
      
    private:
      
      unsigned int m_size;
      T m_defaultValue;
      
    };
    
    template<typename T>
    std::ostream&
    operator<<(std::ostream& os, const sd::core::DynamicDimensionalCoordinates<T>& s);
    
    template<typename T>
    bool
    operator==(const sd::core::DynamicDimensionalCoordinates<T>& s1,
	       const sd::core::DynamicDimensionalCoordinates<T>& s2);
    
    template<typename T>
    bool operator!=(const sd::core::DynamicDimensionalCoordinates<T>& dc1,
		    const sd::core::DynamicDimensionalCoordinates<T>& dc2);
  }
  
}

#include "DynamicDimensionalCoordinates.hxx"

#endif /*! SD_CORE_DYNAMIC_DIMENSIONAL_COORDINATES_HPP */
