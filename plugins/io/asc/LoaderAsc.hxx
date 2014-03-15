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

#ifndef SD_ASC_LOADER_ASC_HXX
#define SD_ASC_LOADER_ASC_HXX

#include <cstring>
#include <fstream>

#include <core/mesh/MeshView.hpp>
#include <core/mesh/MeshView_.hpp>

#include <utils/sdRawUtils.hpp>

namespace sd {
  
  namespace asc {
    
    template<typename T, unsigned int N>
    core::MeshView_<T, N>*
    LoaderAsc<T, N>::loadAsc(const std::string& fileURI) const {
      std::ifstream infile(fileURI.c_str());
      if (!infile) {
#ifdef DEBUG
	std::cerr << "LoaderAsc::loadAscFile() : unable to open file: " << fileURI << std::endl;
#endif
	return NULL;
      }
      
      sd::core::Vector<T> vertices;
      sd::core::Vector<typename sd::core::Normal<T>::type> vertexNormals;
      sd::core::Vector<unsigned int> vertexNormalsIds;
      
      char buff[256];
      sd::core::Vector<std::string> result;
      
      while (!infile.eof()) {
	infile.getline(buff, 256);
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	if (result.size() > 0 && result[0] == "#") {
	  // ignore
	} else if (result.size() == 3) {
	  vertices.push_back(sd::raw::readAcsii<T>(result[0]));
	  vertices.push_back(sd::raw::readAcsii<T>(result[1]));
	  vertices.push_back(sd::raw::readAcsii<T>(result[2]));
	} else if (result.size() == 6) {
	  vertices.push_back(sd::raw::readAcsii<T>(result[0]));
	  vertices.push_back(sd::raw::readAcsii<T>(result[1]));
	  vertices.push_back(sd::raw::readAcsii<T>(result[2]));
	  vertexNormals.push_back(sd::raw::readAcsii<T>(result[3]));
	  vertexNormals.push_back(sd::raw::readAcsii<T>(result[4]));
	  vertexNormals.push_back(sd::raw::readAcsii<T>(result[5]));
	}
      }
      
      if (vertices.size() != vertexNormals.size())
	vertexNormals.resize(0);
      
      return new sd::core::MeshView_<T, N>(fileURI,
					   vertices,
					   sd::core::Vector<unsigned int>(), // Faces
					   sd::core::Vector<unsigned int>(), // Edges
					   vertexNormals);
    }
    
  }
  
}
#endif /*! SD_ASC_LOADER_ASC_HXX */
