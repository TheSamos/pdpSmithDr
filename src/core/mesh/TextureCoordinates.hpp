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

#ifndef SD_CORE_TEXTURE_COORDINATES_HPP
#define SD_CORE_TEXTURE_COORDINATES_HPP

#include "../ThreeDimensionalCoordinates.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    class TextureCoordinates : private ThreeDimensionalCoordinates<T> {

      
    public:

      TextureCoordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove = false);

      TextureCoordinates(const T& pu=0, const T& pv=0, const T& pw=0);

      TextureCoordinates(const TextureCoordinates<T>& t);

      virtual ~TextureCoordinates() {}
      
      static inline unsigned int card() {return ThreeDimensionalCoordinates<T>::card();}
      
      inline void unlink() {return Coordinates<T>::unlink();}
      
      inline const T& u() const;
      inline const T& v() const;
      inline const T& w() const;

      inline const T& operator() (int i) const;
      inline const T& operator[] (int i) const;

      inline const sd::core::TextureCoordinates<T>& set(const T& pu, const T& pv, const T& pw);
      inline T& setU(const T& pu);
      inline T& setV(const T& pv);
      inline T& setW(const T& pw);

      const TextureCoordinates<T>& operator=(const TextureCoordinates<T>& t);

    };
    
  }

}

#include "TextureCoordinates.hxx"

#endif /*! SD_CORE_TEXTURE_COORDINATES_HPP */
