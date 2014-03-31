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

#ifndef SD_UTILS_TYPE_UTILS_HPP
#define SD_UTILS_TYPE_UTILS_HPP

#include <string>

#include <Types.hpp>

namespace sd {

  namespace utils {

    template<typename T>
    inline std::string toString(const T &t);

    template<typename T>
    inline T stringToNumeric(const std::string &s);

    template<typename T, typename U>
    inline T* convertDataTo(const U *data, long n);
    
    template<typename T, typename U>
    inline void remove(T& contener, U& element);
    
    template<typename T, typename U>
    inline void removeIf(T& contener, U& element);
    
    template<typename T>
    inline std::string concatenation(const std::string& str, T val);

  }

}

#define DECLARE_OPERATORS(T)						\
  inline std::string operator+(const std::string& str, const T& val);					

DECLARE_OPERATORS(sd::FLOAT)
DECLARE_OPERATORS(sd::DOUBLE)
DECLARE_OPERATORS(sd::LONGDOUBLE)
DECLARE_OPERATORS(sd::INT8)
DECLARE_OPERATORS(sd::UINT8)
DECLARE_OPERATORS(sd::INT16)
DECLARE_OPERATORS(sd::UINT16)
DECLARE_OPERATORS(sd::INT32)
DECLARE_OPERATORS(sd::UINT32)

#undef DECLARE_OPERATORS

//inline std::string operator+=(std::string str, double val);

#include "sdTypeUtils.hxx"

#endif /*! SD_UTILS_TYPE_UTILS_HPP */
