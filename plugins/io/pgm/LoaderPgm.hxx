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

#ifndef SD_PGM_LOADER_PGM_HXX
#define SD_PGM_LOADER_PGM_HXX

#include <cstring>
#include <fstream>

#include <core/image/Image.hpp>

#include <utils/sdRawUtils.hpp>

namespace sd {
  namespace pgm {

    namespace {

      template<typename T>
      core::ImageView_<T>*
      loadBitmapAscii(char*, size_t, Size, Resolution, std::string&)
      {
	return NULL;
      }

      template<>
      core::ImageView_<UINT8>*
      loadBitmapAscii(char* data, size_t readable,
		      Size sz, Resolution res,
		      std::string& errMsg)
      // :TODO: for bitmaps, keep packing 8 pixels per byte
      {
	// assume there are no more comments
	size_t nbPixels = sz.dataSize();

	UINT8* values = new UINT8[nbPixels];
	size_t index = 0;

	// each line contains 70 pixels (a single character 0 or 1) + 1 line feed
	// only last line may be shorter than 70 characters
	const size_t nbBytesPerLine = 70;
	size_t nbCompleteRows = nbPixels / nbBytesPerLine;
	size_t nbExtraChar = nbPixels % nbBytesPerLine;

	for (size_t row = 0; readable && row < nbCompleteRows; ++row) {
	  for (size_t i = 0; readable && i < nbBytesPerLine; ++i)
	    values[index++] = UINT8(1 - raw::readAsciiByte(data, readable)); // 0 in a PBM file means white

	  if (readable && nbExtraChar > 0)
	    raw::skipAsciiEndOfLine(data, readable);
	}

	for (size_t i = 0; readable && i < nbExtraChar; ++i)
	  values[index++] = UINT8(1 - raw::readAsciiByte(data, readable)); // 0 in a PBM file means white

	if (readable > 0)
	  errMsg = "Dummy characters in image file.\n";
	else
	  raw::checkCorrupted(index, nbPixels, errMsg);

	sd::core::ImageViewInfo info(sd::GRAYLEVEL, Z_AXIS, // :TODO: change to sd::BITMAP
				     sz, res);
	sd::core::Image<UINT8>* img = new sd::core::Image<UINT8>(info);
	img->setData(values);
	delete[] values;
	return img;
      }

      template<typename T>
      core::ImageView_<T>*
      loadBitmapBinary(char*, size_t, Size, Resolution, std::string&)
      {
	return NULL;
      }

      template<>
      core::ImageView_<UINT8>*
      loadBitmapBinary(char* data, size_t readable,
		       Size sz, Resolution res,
		       std::string& errMsg)
      // :TODO: for bitmaps, keep packing 8 pixels per byte
      {
	size_t nbCompleteBytesPerRow = sz.width() / 8;
	size_t nbExtraBits = sz.width() % 8;
	size_t nbRows = sz.height() * sz.depth();
	raw::checkCorrupted(readable, (nbCompleteBytesPerRow+(nbExtraBits>0))*nbRows, errMsg);

	UINT8* typedData = (UINT8*) data;
	UINT8* ptr = typedData;
	UINT8* values = new UINT8[sz.dataSize()]; // change this if packing
	size_t index = 0;

	for (size_t row = 0; readable && row < nbRows; ++row) {
	  for (size_t i = 0; readable && i < nbCompleteBytesPerRow; ++i) {
	    UINT8 byte = *ptr++;
	    for (int j = 0; j < 8; ++j)
	      values[index++] = UINT8((byte & (1 << (7-j))) == 0); // 0 in a PBM file means white
	  }

	  if (readable && nbExtraBits > 0) {
	    UINT8 byte = *ptr++;
	    for (size_t j = 0; j < nbExtraBits; ++j) {
	      values[index++] = UINT8((byte & (1 << (7-j))) == 0); // 0 in a PBM file means white
	    }
	  }
	}

	sd::core::ImageViewInfo info(sd::GRAYLEVEL, Z_AXIS, // :TODO: change to sd::BITMAP
				     sz, res);
	sd::core::Image<UINT8>* img = new sd::core::Image<UINT8>(info);
	img->setData(values);
	delete[] values;
	return img;
      }

    } // namespace

    template<typename T>
    core::ImageView_<T>*
    loadFile(const std::string& filename, std::string& errMsg) {
      // :TODO: consider PPM files (rgb) --> rename class&file to LoaderPnm
      std::ifstream is(filename.c_str(), std::ios::binary);
      if (!is.is_open())
	return NULL;

      is.seekg(0L, std::ios::end);
      size_t file_len = is.tellg();
      is.seekg(0L, std::ios::beg);

      std::vector<char> pdata(file_len+1);
      char *data = &pdata[0];
      is.read(data, file_len);
      is.close();
      size_t readable = file_len;

      raw::skipAsciiComments(data, readable);

      bool ascii = false;
      bool is3d = false;
      bool bitmap = false;
      bool colored = false;

      // P1,PBM3D,SDB3D: bitmap, ascii
      // P2,PGM3D,SDP3D: gray, ascii
      // P3,PCM3D,SDC3D: color, ascii

      // P4,PBM3Db,SDB3Db: bitmap, binary
      // P5,PGM3Db,SDP3Db: gray, binary
      // P6,PCM3Db,SDC3Db: color, binary

      bool sdp3d = (!strncmp(data, "SDP3D", 5) // gray
		    || !strncmp(data, "SDB3D", 5) // bitmap
		    || !strncmp(data, "SDC3D", 5));// color

      if (sdp3d) {
	bitmap = (data[2] == 'B');
	colored = (data[2] == 'C');
	data += 5;
	readable -= 5;
	is3d = true;
	if (data[0] == 'b') {
	  ++ data;
	  -- readable;
	  ascii = false;
	}
	else
	  ascii = true;
      }
      else {
	if (!strncmp(data, "PGM3D", 5) || !strncmp(data, "PBM3D", 5) || !strncmp(data, "PCM3D", 5)) {
	  bitmap = (data[1] == 'B');
	  colored = (data[1] == 'C');
	  data += 5;
	  readable -= 5;
	  is3d = true;
	  if (data[0] == 'b') {
	    ++ data;
	    -- readable;
	    ascii = false;
	  }
	  else
	    ascii = true;
	}
	else if (!strncmp(data, "P1", 2) || !strncmp(data, "P2", 2) || !strncmp(data, "P3", 2) || !strncmp(data, "P4", 2) || !strncmp(data, "P5", 2)|| !strncmp(data, "P6", 2)) {
	  bitmap = (data[1] == '1' || data[1] == '4');
	  colored = (data[1] == '3' || data[1] == '6');
	  ascii = (data[1] == '1' || data[1] == '2' || data[1] == '3');
	  data += 2;
	  readable -= 2;
	  is3d = false;
	}
	else
	  return NULL;
      }

      raw::skipAsciiSpaces(data, readable);
      raw::skipAsciiComments(data, readable);

      // THOMAS:TODO:
      // A comment could be follow by space which could in turn...
      // We need to have a skip_spaceAndComments call

      int sizeX = raw::readAsciiInt(data, readable);
      raw::skipAsciiSpaces(data, readable);
      raw::skipAsciiComments(data, readable);

      int sizeY = raw::readAsciiInt(data, readable);
      if (is3d || !bitmap) {
	raw::skipAsciiSpaces(data, readable);
	raw::skipAsciiComments(data, readable);
      }

      int sizeZ = 1;
      if (is3d) {
	sizeZ = raw::readAsciiInt(data, readable);
	if (sdp3d || !bitmap) {
	  raw::skipAsciiSpaces(data, readable);
	  raw::skipAsciiComments(data, readable);
	}
      }

      float resolX = 1.0f, resolY = 1.0f, resolZ = 1.0f;
      if (sdp3d) {
	resolX = raw::readAsciiFloat(data, readable);
	raw::skipAsciiSpaces(data, readable);
	raw::skipAsciiComments(data, readable);

	resolY = raw::readAsciiFloat(data, readable);
	raw::skipAsciiSpaces(data, readable);
	raw::skipAsciiComments(data, readable);

	resolZ = raw::readAsciiFloat(data, readable);
	if (!bitmap) {
	  raw::skipAsciiSpaces(data, readable);
	  raw::skipAsciiComments(data, readable);
	}
      }

      int max = 1;
      if (!bitmap) {
	max = raw::readAsciiInt(data, readable);
      }
      raw::skipAsciiEndOfLine(data, readable);

      if (bitmap) {
	// :TODO: for bitmaps, keep packing 8 pixels per byte
	if (typeid(T) != typeid(sd::UINT8))
	  return NULL;
      }
      else {
	if (max < 256) {
	  if (typeid(T) != typeid(sd::UINT8))
	    return NULL;
	}
	else {
	  if (typeid(T) != typeid(sd::UINT16))
	    return NULL;
	}
      }

      sd::Size sz(sizeX, sizeY, sizeZ);
      sd::Resolution res(resolX, resolY, resolZ);
      core::ImageView_<T>* img = 0;
      if (bitmap) {
	// :TODO: for bitmaps, keep packing 8 pixels per byte
	img = ascii
	  ? loadBitmapAscii<T>(data, readable, sz, res, errMsg)
	  : loadBitmapBinary<T>(data, readable, sz, res, errMsg);
      }
      else {
	sd::ColorSpace colorSpace = colored ? sd::RGB : sd::GRAYLEVEL;
	sd::UINT8 nbChannels = colored ? 3 : 1;
	img = ascii
	  ? raw::loadAscii<T>(data, readable, colorSpace, nbChannels, sd::Z_AXIS, sz, res, errMsg)
	  : raw::loadBinary<T>(data, readable, colorSpace, nbChannels, sd::Z_AXIS, sz, res, errMsg);
      }

      return img;
    }

    template<typename T>
    core::ImageView_<T>*
    LoaderPgm<T>::loadPgmImage(const std::string& filename) const {
      std::string errMsg; // :TODO: do something with it!
      return loadFile<T>(filename, errMsg);
    }

  }
}

#endif /*! SD_PGM_LOADER_PGM_HXX */
