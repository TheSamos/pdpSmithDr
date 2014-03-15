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

#ifndef SD_PGM_SAVER_PGM_HXX
#define SD_PGM_SAVER_PGM_HX

#include "SaverPgm.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <core/image/Image.hpp>

static const char* creator = "# CREATOR: SmithDR";

namespace sd {
  namespace pgm {

    namespace {

      // fprintf(file, "%f", ...) may write decimals using commas instead of points
      // --> converting float to string fix it
      std::string
      floatToString(float f)
      {
	std::ostringstream oss;
	oss << f;
	return oss.str();
      }

      template<typename T>
      bool
      saveAscii(const std::string& filename, const sd::core::ImageView_<T>* data, bool sdp3d)
      {
	FILE* file = fopen(filename.c_str(), "w");
	if (file == NULL)
	  return false;

	Size sz = data->size();
	size_t sizeX = sz.width();
	size_t sizeY = sz.height();
	size_t sizeZ = sz.depth();
	bool colored = (data->colorSpace() == sd::RGB);

	if (sdp3d) {
	  Resolution res = data->resolution();
	  std::string rx = floatToString(res.width());
	  std::string ry = floatToString(res.height());
	  std::string rz = floatToString(res.depth());
	  fprintf(file, "SD%s3D\n%s\n%d %d %d\n%s %s %s\n%d\n", colored?"C":"P", creator, (int) sizeX, (int) sizeY, (int) sizeZ, rx.c_str(), ry.c_str(), rz.c_str(), (int) data->getMax());
	}
	else {
	  if (sizeZ == 1)
	    fprintf(file, "P%d\n%s\n%d %d\n%d\n", colored?3:2, creator, (int) sizeX, (int) sizeY, (int) data->getMax());
	  else
	    fprintf(file, "P%sM3D\n%s\n%d %d %d\n%d\n", colored?"C":"G", creator, (int) sizeX, (int) sizeY, (int) sizeZ, (int) data->getMax());
	}

	auto end = data->end();
	for (auto it = data->begin(); it != end; ++it)
	  fprintf(file, "%d\n", (int) *it);

	fclose(file);

	return true;
      }

      template<typename T>
      bool
      saveBinary(const std::string& filename, const sd::core::ImageView_<T>* data, bool sdp3d)
      {
	std::ofstream os(filename.c_str(), std::ios::binary);
	if (!os.is_open())
	  return false;

	Size sz = data->size();
	size_t sizeX = sz.width();
	size_t sizeY = sz.height();
	size_t sizeZ = sz.depth();
	bool colored = (data->colorSpace() == sd::RGB);

	if (sdp3d) {
	  Resolution res = data->resolution();
	  if (colored)
	    os << "SDC3Db\n";
	  else
	    os << "SDP3Db\n";
	  os << creator << std::endl;
	  os << (int) sizeX << " " << (int) sizeY << " " << (int) sizeZ << std::endl;
	  os << res.width() << " " << res.height() << " " << res.depth() << std::endl;
	}
	else {
	  if (sizeZ == 1) {
	    if (colored)
	      os << "P6\n";
	    else
	      os << "P5\n";
	    os << creator << std::endl;
	    os << (int) sizeX << " " << (int) sizeY << std::endl;
	  }
	  else {
	    if (colored)
	      os << "PCM3Db\n";
	    else
	      os << "PGM3Db\n";
	    os << creator << std::endl;
	    os << (int) sizeX << " " << (int) sizeY << " " << (int) sizeZ << std::endl;
	  }
	}

	os << (int) data->getMax() << std::endl;
	size_t n = sizeX * sizeY * sizeZ;
	if (colored)
	  n *= 3;
	os.write((const char *) data->begin().operator->(), n * sizeof(T));
	os.close();
	return true;
      }

      template<typename T>
      bool
      saveBitmapAscii(const std::string&, const sd::core::ImageView_<T>*, bool)
      {
	return false;
      }

      template<>
      bool
      saveBitmapAscii(const std::string& filename, const sd::core::ImageView_<UINT8>* data, bool sdp3d)
      {
	FILE* file = fopen(filename.c_str(), "w");
	if (file == NULL)
	  return false;

	Size sz = data->size();
	size_t sizeX = sz.width();
	size_t sizeY = sz.height();
	size_t sizeZ = sz.depth();

	if (sdp3d) {
	  Resolution res = data->resolution();
	  std::string rx = floatToString(res.width());
	  std::string ry = floatToString(res.height());
	  std::string rz = floatToString(res.depth());
	  fprintf(file, "SDB3D\n%s\n%d %d %d\n%s %s %s\n", creator, (int) sizeX, (int) sizeY, (int) sizeZ, rx.c_str(), ry.c_str(), rz.c_str());
	}
	else {
	  if (sizeZ == 1)
	    fprintf(file, "P1\n%s\n%d %d\n", creator, (int) sizeX, (int) sizeY);
	  else
	    fprintf(file, "PBM3D\n%s\n%d %d %d\n", creator, (int) sizeX, (int) sizeY, (int) sizeZ);
	}

	// each line contains 70 pixels (a single character 0 or 1) + 1 line feed
	// only last line may be shorter than 70 characters
	const int nbBytesPerLine = 70;
	int count = 0;
	auto end = data->end();
	for (auto it = data->begin(); it != end; ++it) {
	  fprintf(file, "%d", 1-(int) *it); // 0 in a PBM file means white
	  if (++count == nbBytesPerLine) {
	    fprintf(file, "\n");
	    count = 0;
	  }
	}

	fclose(file);

	return true;
      }

      template<typename T>
      bool
      saveBitmapBinary(const std::string&, const sd::core::ImageView_<T>*, bool)
      {
	return false;
      }

      template<>
      bool
      saveBitmapBinary(const std::string& filename, const sd::core::ImageView_<UINT8>* data, bool sdp3d)
      {
	std::ofstream os(filename.c_str(), std::ios::binary);
	if (!os.is_open())
	  return false;

	Size sz = data->size();
	size_t sizeX = sz.width();
	size_t sizeY = sz.height();
	size_t sizeZ = sz.depth();

	if (sdp3d) {
	  Resolution res = data->resolution();
	  os << "SDB3Db\n";
	  os << creator << std::endl;
	  os << (int) sizeX << " " << (int) sizeY << " " << (int) sizeZ << std::endl;
	  os << res.width() << " " << res.height() << " " << res.depth() << std::endl;
	}
	else {
	  if (sizeZ == 1) {
	    os << "P4\n";
	    os << creator << std::endl;
	    os << (int) sizeX << " " << (int) sizeY << std::endl;
	  }
	  else {
	    os << "PBM3Db\n";
	    os << creator << std::endl;
	    os << (int) sizeX << " " << (int) sizeY << " " << (int) sizeZ << std::endl;
	  }
	}

	size_t nbCompleteBytesPerRow = sizeX / 8;
	UINT8 nbExtraBits = sizeX % 8;
	size_t nbRows = sizeY * sizeZ;
	auto itr = data->begin();

	size_t n = (nbCompleteBytesPerRow+(nbExtraBits>0))*sizeY*sizeZ;
	UINT8* packedData = new UINT8[n];
	size_t index = 0;

	for (size_t row = 0; row < nbRows; ++row) {
	  for (size_t i = 0; i < nbCompleteBytesPerRow; ++i) {
	    UINT8 pack = 0; // 0 in a PBM file means white
	    for (UINT8 j = 0; j < 8; ++j)
	      if (itr[j] == 0)
		pack |= UINT8(1 << (7-j));
	    packedData[index++] = pack;
	    itr += 8;
	  }

	  if (nbExtraBits > 0) {
	    UINT8 pack = 1; // 0 in a PBM file means white
	    for (UINT8 j = 0; j < nbExtraBits; ++j) {
	      if (itr[j] == 1)
		pack &= UINT8(~(1 << (7-j)));
	    }
	    packedData[index++] = pack;
	    itr += nbExtraBits;
	  }
	}

	os.write((const char*) packedData, n*sizeof(UINT8));
	delete [] packedData;

	os.close();
	return true;
      }

    }

    template<typename T>
    bool
    SaverPgm<T>::savePgmImage(const core::ImageView_<T>* data, const std::string& filename, bool binary) const
    {
      bool sdp3d = (filename.find(".sdp3d") != std::string::npos);

      if (data->channelType() == sd::core::DataType_<UINT8>()) {
	UINT8 mini = data->getMin();
	UINT8 maxi = data->getMax();
	if (mini == 0 && maxi == 1) { // Bitmap
	  if (binary)
	    return saveBitmapBinary(filename, data, sdp3d);

	  return saveBitmapAscii(filename, data, sdp3d);
	}
      }

      // not a bitmap or not UINT8 type
      if (binary)
	return saveBinary(filename, data, sdp3d);

      return saveAscii(filename, data, sdp3d);

      return false;
    }

  }
}

#endif /*! SD_PGM_SAVER_PGM_HXX */
