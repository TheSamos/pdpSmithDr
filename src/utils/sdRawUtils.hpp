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

#ifndef SD_RAW_RAW_UTILS_HPP
#define SD_RAW_RAW_UTILS_HPP

#include <Types.hpp>
#include <core/image/ImageView_.hpp>

#include <cstdio>
#include <string>

namespace sd {
  namespace raw {
    void skipAsciiUseless(char*& data, size_t& readable);
    std::string readAsciiString(char*& data, size_t& readble);
    UINT8 readAsciiByte(char*& data, size_t& readable);
    int readAsciiInt(char*& data, size_t& readable);
    float readAsciiFloat(char*& data, size_t& readable);

    template<typename T>
    T readAcsii(const std::string& str);
    
    template<typename U, typename V>
    size_t numberOf(const std::string& str, U pliter, V ender = '\0');
    
    template<typename U>
    void split(const std::string& str, U pliter, std::vector<std::string>& result);
    
    template<typename U>
    void merge(const std::vector<std::string>& vect, U merger, std::string& str);
    
    template<typename T> T readBinaryValueAs(char*& data, size_t& readable);

    void skipAsciiSpaces(char*& data, size_t& readable);
    // comments are lines beginning with a # (sharp) character
    void skipAsciiComments(char*& data, size_t& readable);
    void skipAsciiEndOfLine(char*& data, size_t& readable);

    void checkCorrupted(size_t nbRead, size_t nbExpected, std::string& errMsg);

    // swap each byte pair from the given list of n pairs [short]
    // Example: swap(ab) = ba
    void swap_2bytes(size_t n, void *ar);

    // swap 4 bytes at a time from the given list of n sets of 4 bytes [int, float]
    // Example: swap(abcd) = dcba
    void swap_4bytes(size_t n, void *ar);

    // swap 8 bytes at a time from the given list of n sets of 8 bytes [long long, double]
    // Example: swap(abcdDCBA) = ABCDdcba
    void swap_8bytes(size_t n, void *ar);

    // swap 16 bytes at a time from the given list of n sets of 16 bytes [long double]
    // Example: swap(abcdefghHGFEDCBA) = ABCDEFGHhgfedcba
    void swap_16bytes(size_t n, void *ar);

    // based on sz, call the appropriate swap_Nbytes() function
    void swap_Nbytes(size_t n, int sz, void *ar);
    
    template<typename T>
    void swapEndian(T& value);
    
    template<typename T>
    void swapEndian(sd::core::Vector<T>& values);
    
    template<typename T>
    core::ImageView_<T>* loadBinary(char* data, size_t readable,
				    sd::ColorSpace colorSpace, sd::UINT8 nbChannels,
				    sd::Orientation orientation,
				    Size sz, Resolution res,
				    std::string& errMsg);

    template<typename T>
    core::ImageView_<T>* loadAscii(char* data, size_t readable,
				   sd::ColorSpace colorSpace, sd::UINT8 nbChannels,
				   sd::Orientation orientation,
				   Size sz, Resolution res,
				   std::string& errMsg);

  }
}

#include "sdRawUtils.hxx"

#endif /*! SD_RAW_RAW_UTILS_HPP */
