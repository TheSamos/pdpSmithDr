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

#ifndef SD_UTILS_FILE_UTILS_HPP
#define SD_UTILS_FILE_UTILS_HPP

#include <string>

namespace sd {

  namespace utils {

#ifdef __WIN32__
#define DIR_SEP_STRING "\\"
#define DIR_SEP_CHAR '\\'
#else
#define DIR_SEP_STRING "/"
#define DIR_SEP_CHAR '/'
#endif

    /**
     * Check for file existence and read permissions.
     */
    bool fileExistsAndReadable(const std::string& filename);

    /**
     * Removes extension (if present in filename) from filename.
     * If extensions are not found, return unchanged filename.
     * Extensions are separated by semicolumns.
     */
    std::string cutFileExtension(const std::string& filename, const std::string& extensions);

    /**
     * Check if filename has one of the specified extensions.
     * Return matching extension if found; else return empty string.
     * Extensions are separated by semicolumns.
     */
    std::string getFileExtension(const std::string& filename, const std::string& extensions);

    /**
     * Test if filename ends with the given string (s).
     */
    bool endsWith(const std::string& filename, const std::string& s);

    /**
     * Check if filename matches one of the specified filters.
     * Filters are coma-separated.
     * A filter have one of the following form:
     *    *.ext to identify all files having the given extension (ext)
     *    *fixed.ext to identify all files having both a filename suffix (fixed) and the given extension (ext)
     *    name.ext to check if filename matches exactly both the given name (name) and extension (ext)
     */
    bool fileMatchesFilters(const std::string& filename, const std::string& filters);

  }

}

#endif /*! SD_UTILS_FILE_UTILS_HPP */
