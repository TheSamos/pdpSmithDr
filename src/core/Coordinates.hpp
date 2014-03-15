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

#ifndef SD_CORE_COORDINATES_HPP
#define SD_CORE_COORDINATES_HPP

#include <core/Vector.hpp>

namespace sd {
  
  namespace core {

    template<typename T>
    class Coordinates {
      
    public:

      typedef T type;
      
    public:
      
      virtual ~Coordinates();
      
      inline const T& operator() (unsigned int i) const;
      inline const T& operator[] (unsigned int i) const;
      //inline T& operator[] (unsigned int i);
      inline const T& getAt(unsigned int i) const;
      inline const T& setAt(unsigned int i, const T& value);
      
      // std::string toString() const;
      
      // virtual unsigned int size() = 0;
      
      virtual unsigned int nbDims() const = 0;
      
      inline void unlink();
      
    protected:
      
      Coordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove);
      
      template<typename ...VALUES>
      Coordinates(unsigned int size, VALUES ...values);
      
      template<typename ...VALUES>
      Coordinates(unsigned int* size, VALUES ...values);
      
      bool resize(unsigned int newSize, T defaultValue);
      
      void reset(unsigned int n, T defaultValue);
      
      void copyTo(unsigned int n, T* dest) const;
      
      void copy(unsigned int n, const Coordinates<T>& c);
      
      void nocopy(const Coordinates<T>& c);
      
    private:
      
      Coordinates(const Coordinates& c) = delete;
      
      const Coordinates<T>& operator=(const Coordinates<T>& c) = delete;
      
      Coordinates() = delete;
      
      inline const T* data() const {return m_data->data()+m_index;}
      inline T* data() {return m_data->data()+m_index;}
      
      void fill(unsigned int sizeMax, unsigned int idx);
      
      void fill(unsigned int sizeMax, unsigned int idx, T value);
      
      template<typename ...VALUES>
      void fill(unsigned int sizeMax, unsigned int idx, T value, VALUES ...rest);
      
    private:
      
      sd::core::Vector<T>* m_data;
      unsigned int m_index;
      bool m_isStandAlone;

    };
    
  }
  
}

#include "Coordinates.hxx"

#endif /*! SD_CORE_COORDINATES_HPP */
