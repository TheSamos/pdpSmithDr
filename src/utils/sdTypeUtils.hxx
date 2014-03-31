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

#ifndef SD_UTILS_TYPE_UTILS_HXX
#define SD_UTILS_TYPE_UTILS_HXX

#include <algorithm>

#include <sstream>

template<typename T>
std::string
sd::utils::toString(const T &t)
{
  std::stringstream st;
  st << t;
  return st.str();
}

template<typename T>
T
sd::utils::stringToNumeric(const std::string &s) {
  T val;
  std::stringstream st(s);
  st >> val;
  return val;
}

template<typename T, typename U>
T *
sd::utils::convertDataTo(const U *data, long n) {
  T *newData = new T[n];
  for (long i = 0; i < n; ++i)
    newData[i] = T(data[i]);

  return newData;
}

template<typename T, typename U>
void
sd::utils::remove(T& contener, U& element) {
  if(std::find(contener.begin(), contener.end(), element) != contener.end())
    contener.erase(std::find(contener.begin(), contener.end(), element));
}

template<typename T, typename U>
void
sd::utils::removeIf(T& contener, U& element) {
  while (std::find(contener.begin(), contener.end(), element) != contener.end())
    contener.erase(std::find(contener.begin(), contener.end(), element));
}

template<typename T>
std::string
sd::utils::concatenation(const std::string& str, T val) {
  std::stringstream ss(std::stringstream::in | std::stringstream::out);
  std::string res;
  ss << val << std::flush;
  ss >> res;
  return str + res;
}

#define BUILD_OPERATORS(T)					\
  std::string							\
  operator+(const std::string& str, const T& val) {		\
    return sd::utils::concatenation(str, val);			\
  }							

BUILD_OPERATORS(sd::FLOAT)
BUILD_OPERATORS(sd::DOUBLE)
BUILD_OPERATORS(sd::LONGDOUBLE)
BUILD_OPERATORS(sd::INT8)
BUILD_OPERATORS(sd::UINT8)
BUILD_OPERATORS(sd::INT16)
BUILD_OPERATORS(sd::UINT16)
BUILD_OPERATORS(sd::INT32)
BUILD_OPERATORS(sd::UINT32)

#undef BUILD_OPERATORS

#endif /*! SD_UTILS_TYPE_UTILS_HXX */
