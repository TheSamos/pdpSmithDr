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

#ifndef SD_OBJ_SAVER_OBJ_HXX
#define SD_OBJ_SAVER_OBJ_HXX

#include "SaverObj.hpp"

#include <fstream>
#include <sstream>

#include <core/mesh/MeshView_.hpp>

#ifdef DEBUG
#include <iostream>
#endif

template<typename T, unsigned int N>
bool
sd::obj::SaverObj<T, N>::saveObjImage(sd::core::MeshView_<T, N>* mesh, const std::string& fileURI,
				      bool saveVertexNormals, bool saveVertexColors,
				      bool saveFaceNormals, bool saveFaceColors) const {
      
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
    std::cerr << "SaverObj::saveObjMesh() : unable to open file: " << fileURI << std::endl;
#endif
    return false;
  }

  outfile << "# SmithDR generated file" << std::endl << std::endl;
  
  typename sd::core::MeshView_<T, N>::vertices_const_iterator itV =  mesh->beginVertices();
  const typename sd::core::MeshView_<T, N>::vertices_const_iterator endV =  mesh->endVertices();
  for (; itV!=endV; ++itV) {
    outfile << "v" << " " << (*itV).x() << " " << (*itV).y() << " " << (*itV).z() << std::endl;
    if (saveVertexNormals) {
      outfile << "vn"
	      << " " << (*itV).getNormal().x()
	      << " " << (*itV).getNormal().y()
	      << " " << (*itV).getNormal().z() << std::endl;
    }
    if (saveVertexColors) {
      outfile << "vc"
	      << " " << ((double)(*itV).getRGBCoordinates().r()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().g()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().b()/255.)
	      << " " << ((double)(*itV).getRGBCoordinates().a()/255.) << std::endl;
    }
  }

  outfile << std::endl;

  typename sd::core::MeshView_<T, N>::faces_const_iterator itF =  mesh->beginFaces();
  const typename sd::core::MeshView_<T, N>::faces_const_iterator endF =  mesh->endFaces();
  for (; itF!=endF; ++itF) {
    outfile << "f";
    for (unsigned int i=0; i<N; ++i) {
      unsigned int gblIdx = ((*itF)->getAt(mesh, i).globalIndex()+1);
      outfile << " " << gblIdx;
      if (saveVertexNormals && (*itF)->getAt(mesh, i).isNormalSet()) {
	outfile << "//" << gblIdx;
      }
    }
    outfile << std::endl;
    if (saveFaceNormals) {
      outfile << "fn"
	      << " " << (*itF)->getNormal(mesh).x()
	      << " " << (*itF)->getNormal(mesh).y()
	      << " " << (*itF)->getNormal(mesh).z() << std::endl;
    }
    if (saveFaceColors) {
      /* // TODO:J:23.05.2012 NOT YET !!!
      outfile << "fc"
	      << " " << ((double)(*itF)->getRGBCoordinates().r()/255.)
	      << " " << ((double)(*itF)->getRGBCoordinates().g()/255.)
	      << " " << ((double)(*itF)->getRGBCoordinates().b()/255.) << std::endl;
      */
    }
  }
  
  outfile << std::endl;

  outfile.close();
  
  return true;
}

#endif /*! SD_OBJ_SAVER_OBJ_HXX */
