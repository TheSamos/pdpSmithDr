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

#ifndef SD_PLY_SAVER_PLY_HXX
#define SD_PLY_SAVER_PLY_HXX

#include "SaverPly.hpp"

#include <fstream>
#include <sstream>

#include <core/mesh/MeshView_.hpp>

#ifdef DEBUG
#include <iostream>
#endif

template<typename T, unsigned int N>
bool
sd::ply::SaverPly<T, N>::savePlyImage(sd::core::MeshView_<T, N>* mesh, const std::string& fileURI,
				      bool saveVertexNormals, bool saveVertexColors,
				      bool saveFaceNormals, bool /*saveFaceColors*/) const {
  
  {
    typename sd::core::MeshView_<T, N>::vertices_const_iterator itV =  mesh->beginVertices();
    const typename sd::core::MeshView_<T, N>::vertices_const_iterator endV =  mesh->endVertices();
    for (; itV!=endV; ++itV) {
      saveVertexNormals &= (*itV).isNormalSet();
      saveVertexColors &= (*itV).isRGBCoordinatesSet();
    }
    typename sd::core::MeshView_<T, N>::faces_const_iterator itF =  mesh->beginFaces();
    const typename sd::core::MeshView_<T, N>::faces_const_iterator endF =  mesh->endFaces();
    for (; itF!=endF; ++itF) {
      saveFaceNormals &= (*itF)->isNormalSet(mesh);
      // saveFaceColors &= (*itF)->isRGBCoordinatesSet(); // TODO:J:23.05.2012 NOT YET !!!
    }
  }
  std::ofstream outfile(fileURI.c_str());
  if (!outfile) {
#ifdef DEBUG
    std::cerr << "SaverPly::savePlyMesh() : unable to open file: " << fileURI << std::endl;
#endif
    return false;
  }
  std::string typeName(sd::core::DataType_<T>().toString());
  outfile << "ply" << std::endl;
  outfile << "format ascii 1.0" << std::endl;
  outfile << "comment SmithDR generated file" << std::endl;
  outfile << "element vertex " <<  mesh->nbVertices() << std::endl;
  outfile << "property " << typeName << " x" << std::endl;
  outfile << "property " << typeName << " y" << std::endl;
  outfile << "property " << typeName << " z" << std::endl;
  if (saveVertexNormals) {
    outfile << "property " << typeName << " nx" << std::endl;
    outfile << "property " << typeName << " ny" << std::endl;
    outfile << "property " << typeName << " nz" << std::endl;
  }
  if (saveVertexColors) {
    outfile << "property " << typeName << " red" << std::endl;
    outfile << "property " << typeName << " green" << std::endl;
    outfile << "property " << typeName << " blue" << std::endl;
    outfile << "property " << typeName << " alpha" << std::endl;
  }
  outfile << "element face " << mesh->nbFaces() << std::endl;
  outfile << "property list uchar int vertex_indices" << std::endl;
  outfile << "end_header" << std::endl;
  
  typename sd::core::MeshView_<T, N>::vertices_const_iterator itV =  mesh->beginVertices();
  const typename sd::core::MeshView_<T, N>::vertices_const_iterator endV =  mesh->endVertices();
  for (; itV!=endV; ++itV) {
    outfile << (*itV).x() << " " << (*itV).y() << " " << (*itV).z();
    if (saveVertexNormals) {
      outfile << " " << (*itV).getNormal().x()
	      << " " << (*itV).getNormal().y()
	      << " " << (*itV).getNormal().z();
    }
    if (saveVertexColors) {
      outfile << " " << ((double)(*itV).getRGBCoordinates().r()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().g()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().b()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().a()/255.);
    }
    outfile << std::endl;
  }

  typename sd::core::MeshView_<T, N>::faces_const_iterator itF =  mesh->beginFaces();
  const typename sd::core::MeshView_<T, N>::faces_const_iterator endF =  mesh->endFaces();
  for (; itF!=endF; ++itF) {
    outfile << 3;
    for (unsigned int i=0; i<N; ++i) {
      outfile << " " << ((*itF)->getAt(mesh, i).globalIndex());
    }
    outfile << std::endl;
  }
  
  outfile.close();
  
  return true;
}

#endif /*! SD_PLY_SAVER_PLY_HXX */
