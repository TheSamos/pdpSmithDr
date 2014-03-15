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

#ifndef SD_STL_SAVER_STL_HPP
#define SD_STL_SAVER_STL_HPP

#include <core/mesh/MeshView_.hpp>

#include <string>

namespace sd {
  namespace stl {

    template<typename T>
    class SaverStl {
    public:
      bool saveStlImage(const core::MeshView_<T, 3>* mesh, const std::string& filename, bool binary) const;
    };

  }
}

#endif /*! SD_STL_SAVER_STL_HPP */
