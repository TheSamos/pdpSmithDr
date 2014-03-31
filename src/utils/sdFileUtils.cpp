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

#include "sdFileUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

bool
sd::utils::fileExistsAndReadable(const std::string& filename)
{
  std::fstream fs;
  fs.open(filename.c_str(), std::ios::in);
  bool flag = fs.is_open();
  fs.close();
  return flag;
}

std::string
sd::utils::cutFileExtension(const std::string& filename, const std::string& extensions)
{
  std::stringstream st(extensions);

  std::string ext;
  while (st) {
    ext = "";
    std::getline(st, ext, ';');
    if (!ext.empty()) {
      size_t pos = filename.find(ext);
      if (pos != std::string::npos)
	return filename.substr(0, pos);
    }
  }

  return filename;
}

std::string
sd::utils::getFileExtension(const std::string& filename, const std::string& extensions)
{
  std::stringstream st(extensions);

  std::string ext;
  while (st) {
    ext = "";
    std::getline(st, ext, ';');
    if (!ext.empty()) {
      size_t pos = filename.find(ext);
      if (pos != std::string::npos) {
	// check for exact match: ext is the end of filename
	if (pos + ext.length() == filename.length())
	  return ext;
      }
    }
  }

  return "";
}

bool
sd::utils::endsWith(const std::string& filename, const std::string& s)
{
  size_t pos = filename.find(s);
  if (pos == std::string::npos)
    return false;

  // check for exact match: s is the end of filename
  return (pos + s.length() == filename.length());
}

bool
sd::utils::fileMatchesFilters(const std::string& filename, const std::string& filters)
{
  std::stringstream st(filters);

  std::string filt;
  while (st) {
    filt = "";
    std::getline(st, filt, ';');
    if (!filt.empty()) {
      if (filt[0] == '*') {
	std::string ext = filt.substr(1); // suffix.ext or .ext
	if (endsWith(filename, ext))
	  return true;
      }
      else { // check for exact match name.ext
	if (filename == filt)
	  return true;
      }
    }
  }

  return false;
}
