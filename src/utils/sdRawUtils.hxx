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

#ifndef SD_RAW_RAW_UTILS_HXX
#define SD_RAW_RAW_UTILS_HXX

#include <core/image/Image.hpp>
#include <core/image/ImageViewInfo.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

sd::UINT8
sd::raw::readAsciiByte(char*& data, size_t& readable) {
  UINT8 byte = UINT8(*data++ - '0');
  --readable;
  return byte;
}

std::string
sd::raw::readAsciiString(char*& data, size_t& readable) {
  std::string ret;
  char in[80];
  int index = 0;
  while (!isspace(*data) && index<80) {
    in[index++]=*data++;
    --readable;
  }
  in[index]='\0';
  return in;
}

int
sd::raw::readAsciiInt(char*& data, size_t& readable) {
  int signe = 1;
  if (*data == '-'){
    signe = -1;
    ++data;
    --readable;
  }
  int i = *data++ - '0';
  --readable;
  while (*data && isdigit(*data)) {
    i = 10*i + *data++ - '0';
    --readable;
  }
  return signe* i;
}
	
float
sd::raw::readAsciiFloat(char*& data, size_t& readable) {
  int signe = 1;
  if (*data == '-'){
    signe = -1;
    ++data;
    --readable;
  }
  int ipart = signe*readAsciiInt(data, readable);
  float dpart = float(ipart);
     
  if (*data == '.' or *data == ','){
    ++data; // skip '.'
    --readable;

    float mult = 10;
    while (*data && isdigit(*data)) {
      float f = float(*data++ - '0');
      --readable;
      dpart += signe*f / mult;
      mult *= 10;
    }
  }
  if (*data == 'e'){
    ++data; // skip 'e'
    --readable;
    signe = 1;
    if (*data == '-'){
      signe = -1;
      ++data;
      --readable;
    }
    else if (*data == '+'){
      signe = 1;
      ++data;
      --readable;
    }
    int p = signe*readAsciiInt(data,readable);
    dpart *= pow(10.0,p);
  }
  return dpart;
}
/*
  float
  sd::raw::readAsciiFloat(char*& data, size_t& readable)
  {
  int ipart = readAsciiInt(data, readable);
  if (*data != '.')
  return float(ipart);

  ++data; // skip '.'
  --readable;
  float dpart = float(ipart);
  float mult = 10;
  while (*data && isdigit(*data)) {
  float f = float(*data++ - '0');
  --readable;
  dpart += f / mult;
  mult *= 10;
  }

  return dpart;
  }
*/
    
template<typename T>
T
sd::raw::readAcsii(const std::string& str) {
  std::stringstream ss;
  T res;
  ss << str;
  ss >> res;
  return res;
}
    
template<typename T, typename U>
size_t
sd::raw::numberOf(const std::string& str, T pliter, U ender = '\0') {
  int start=0;
  int end=0;
  size_t nb=0;
  int length=str.find_first_of(ender, 0);
  while (start < length) {
    end=str.find_first_of(pliter, start);
    if (end == -1 || end > length)
      return nb;
    ++nb;
    start=end+1;
  }
  return nb;
}

template<typename T>
void
sd::raw::split(const std::string& str, T pliter, std::vector<std::string>& result) {
  unsigned int start=0;
  int end=0;
  result.clear();
  while (start < str.length()) {
    end = str.find_first_of(pliter, start);
    if (end != -1)
      result.push_back(str.substr(start, end-start));
    else {
      result.push_back(str.substr(start));
      return;
    }
    start=end+1;
  }
}
    
template<typename T>
void
sd::raw::merge(const std::vector<std::string>& vect, T merger, std::string& str) {
  if (vect.size() == 0) return;
  str = vect[0];
  for(unsigned int i=1; i<vect.size(); ++i) {
    str += merger;
    str += vect[i];
  }
}

template<typename T>
T
sd::raw::readBinaryValueAs(char*& data, size_t& readable) {
  size_t nBytes = sizeof(T);
  std::vector<char> v(data, data+nBytes);
  T* p = (T*) &(data[0]);
  data += nBytes;
  readable -= nBytes;
  return T(p[0]);
}

void
sd::raw::skipAsciiSpaces(char*& data, size_t& readable) {
  while (readable && isspace(*data)) {
    ++data;
    --readable;
  }
}

void
sd::raw::skipAsciiComments(char*& data, size_t& readable) {
  while (*data == '#') {
    --readable;
    while (*data++ != '\n') {
      --readable;
    }
  }
}

void
sd::raw::skipAsciiUseless(char*& data, size_t& readable) {
  while (readable) {
    if(isspace(*data)){
      ++data;	
      --readable;
      continue;
    }
    else if (*data == '#'){
      data++;
      readable--;
      while (*data != '\n' && readable) {
	data++;
	readable--;
      }
      continue;
    }
    else 
      return;
  }
}
void
sd::raw::skipAsciiEndOfLine(char*& data, size_t& readable) {
  while (readable && *data != 10) { // 10 is Line Feed
    ++data;
    --readable;
  }

  if (readable && data) {  // skip line feed
    ++data;
    --readable;
  }
}

void
sd::raw::checkCorrupted(size_t nbRead, size_t nbExpected, std::string& errMsg) {
  if (nbRead != nbExpected) {
    std::ostringstream ostr;
    ostr << "Corrupted image file.\n";
    ostr << "Expected size: " << nbExpected << " bytes\n";
    ostr << "Read: " << nbRead << " bytes\n";
    errMsg = ostr.str();
  }
}

void
sd::raw::swap_2bytes(size_t n, void *ar) {
  register size_t ii;
  unsigned char* cp1 = (unsigned char*) ar;
  unsigned char* cp2;
  unsigned char tval;

  for (ii = 0; ii < n; ii++) {
    cp2 = cp1 + 1;
    tval = *cp1;
    *cp1 = *cp2;
    *cp2 = tval;
    cp1 += 2;
  }
}

void
sd::raw::swap_4bytes(size_t n, void *ar) {
  register size_t ii;
  unsigned char* cp0 = (unsigned char*) ar;
  unsigned char* cp1;
  unsigned char* cp2;
  register unsigned char tval;

  for (ii = 0; ii < n; ii++) {
    cp1 = cp0;
    cp2 = cp0+3;
    tval = *cp1;
    *cp1 = *cp2;
    *cp2 = tval;
    cp1++;
    cp2--;
    tval = *cp1;
    *cp1 = *cp2;
    *cp2 = tval;
    cp0 += 4;
  }
}

void
sd::raw::swap_8bytes(size_t n, void *ar) {
  register size_t ii;
  unsigned char* cp0 = (unsigned char*) ar;
  unsigned char* cp1;
  unsigned char* cp2;
  register unsigned char tval;

  for (ii = 0; ii < n; ii++) {
    cp1 = cp0;
    cp2 = cp0+7;
    while (cp2 > cp1) {
      tval = *cp1;
      *cp1 = *cp2;
      *cp2 = tval;
      cp1++;
      cp2--;
    }
    cp0 += 8;
  }
}

void
sd::raw::swap_16bytes(size_t n, void *ar) {
  register size_t ii;
  unsigned char* cp0 = (unsigned char*) ar;
  unsigned char* cp1;
  unsigned char* cp2;
  register unsigned char tval;

  for (ii = 0; ii < n; ii++) {
    cp1 = cp0;
    cp2 = cp0+15;
    while (cp2 > cp1) {
      tval = *cp1;
      *cp1 = *cp2;
      *cp2 = tval;
      cp1++;
      cp2--;
    }
    cp0 += 16;
  }
}

void
sd::raw::swap_Nbytes(size_t n, int sz, void *ar) {
  switch (sz) {
  case  2: swap_2bytes(n, ar); break;
  case  4: swap_4bytes(n, ar); break;
  case  8: swap_8bytes(n, ar); break;
  case 16: swap_16bytes(n, ar); break;
  default:
    fprintf(stderr,"** Cannot swap in %d byte blocks\n", sz);
    break;
  }
}

template<typename T>
void
sd::raw::swapEndian(T& value) {
  char* data = (char*)&value;
  unsigned short length = sizeof(T)/sizeof(char);
  for (unsigned short i=0; i<length/2; ++i)
    std::swap(*(data+i), *(data+length-1-i));
}

template<typename T>
void
sd::raw::swapEndian(sd::core::Vector<T>& values) {
  typename std::vector<T>::iterator it = values.begin();
  const typename std::vector<T>::iterator end = values.end();
  for (; it!=end; ++it)
    swapEndian(*it);
}

template<typename T>
sd::core::ImageView_<T>*
sd::raw::loadBinary(char* data, size_t readable,
		    ColorSpace colorSpace, UINT8 nbChannels,
		    Orientation orientation,
		    Size sz, Resolution res,
		    std::string& errMsg) {
  size_t total = sz.dataSize() * nbChannels;

  checkCorrupted(readable, total, errMsg);

  T* typedData = (T*) data;
  T* values = new T[readable];

  for (size_t i = 0; i < readable && i < total; ++i) {
    values[i] = typedData[i];
  }

  sd::core::ImageViewInfo info(colorSpace, nbChannels, orientation, sz, res);
  sd::core::Image<T> *img = new sd::core::Image<T>(info);
  img->setData(values);
  return img;
}

template<typename T>
sd::core::ImageView_<T>*
sd::raw::loadAscii(char* data, size_t readable,
		   sd::ColorSpace colorSpace, sd::UINT8 nbChannels,
		   sd::Orientation orientation,
		   Size sz, Resolution res,
		   std::string& errMsg) {
  // assume there are no more comments
  size_t total = sz.dataSize() * nbChannels;

  T* values = new T[total];
  size_t i = 0;

  while (readable && i < total) {
    values[i++] = T(readAsciiFloat(data, readable));

    skipAsciiSpaces(data, readable);
    skipAsciiComments(data, readable);
  }

  if (readable > 0) {
    errMsg = "Dummy characters in image file.\n";
  } else
    checkCorrupted(i, total, errMsg);

  sd::core::ImageViewInfo info(colorSpace, orientation, sz, res);
  sd::core::Image<T> *img = new sd::core::Image<T>(info);
  img->setData(values);
  return img;
}

#endif /* ! SD_RAW_RAW_UTILS_HXX */
