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

#ifndef SD_PLY_LOADER_PLY_HPP
#define SD_PLY_LOADER_PLY_HPP

#include <core/mesh/MeshView_.hpp>

#include <string>

namespace sd {
  namespace ply {

    template<typename T, unsigned int N>
    class LoaderPly {
      
    public:
      
      core::MeshView_<T, N>* loadPlyMesh(const std::string& filename) const;
      
    private:
      
      enum data_format {ASCII, BINARY_LITTLE_ENDIAN, BINARY_BIG_ENDIAN};
      
    private:
      
      static bool readHeader(std::ifstream& infile,
			     unsigned int& nbVertices,
			     std::vector<std::pair<std::string, std::string> >& vertexProperties,
			     unsigned int& nbFaces,
			     std::vector<std::pair<std::string, std::string> >& faceProperties,
			     data_format& format);
      
      static bool loadAscii(std::ifstream& infile,
			    unsigned int nbVertices,
			    std::vector<std::pair<std::string, std::string> >& vertexProperties,
			    sd::core::Vector<T>& vertices,
			    sd::core::Vector<typename sd::core::Normal<T>::type>& vertexNormals,
			    sd::core::Vector<unsigned char>& vertexColors,
			    sd::core::Vector<double>& vertexTextures,
			    unsigned int nbFaces,
			    std::vector<std::pair<std::string, std::string> >& faceProperties,
			    sd::core::Vector<unsigned int>& faces,
			    sd::core::Vector<typename sd::core::Normal<T>::type>& faceNormals,
			    sd::core::Vector<unsigned char>& faceColors);
      
      static bool loadBinary(data_format format,
			     std::ifstream& infile,
			     unsigned int nbVertices,
			     std::vector<std::pair<std::string, std::string> >& vertexProperties,
			     sd::core::Vector<T>& vertices,
			     sd::core::Vector<typename sd::core::Normal<T>::type>& vertexNormals,
			     sd::core::Vector<unsigned char>& vertexColors,
			     sd::core::Vector<double>& vertexTextures,
			     unsigned int nbFaces,
			     std::vector<std::pair<std::string, std::string> >& faceProperties,
			     sd::core::Vector<unsigned int>& faces,
			     sd::core::Vector<typename sd::core::Normal<T>::type>& faceNormals,
			     sd::core::Vector<unsigned char>& faceColors);
      
    private:
      
      static void indexOf(const std::vector<std::pair<std::string, std::string> >& properties,
			  const std::string& property,
			  unsigned int& inc,
			  char& idx);
      
    };

  }
}

#include "LoaderPly.hxx"

#endif /* ! SD_PLY_LOADER_PLY_HPP */
