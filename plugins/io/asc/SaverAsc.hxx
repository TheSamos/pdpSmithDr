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

#ifndef SD_ASC_SAVER_ASC_HXX
#define SD_ASC_SAVER_ASC_HXX

#include "SaverAsc.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <core/mesh/MeshView_.hpp>

namespace sd {
  
  namespace asc {
    
    template<typename T, unsigned int N>
    bool
    SaverAsc<T, N>::saveAsc(core::MeshView_<T, N>* mesh,
			    const std::string& fileURI,
			    bool saveVertexNormals) const {
      std::ofstream outfile(fileURI.c_str());
      if (!outfile) {
#ifdef DEBUG
	std::cerr << "SaverAsc::saveAscMesh() : unable to open file: " << fileURI << std::endl;
#endif
	return false;
      }
      
      outfile << "# SmithDR generated file" << std::endl << std::endl;

      typename sd::core::MeshView_<T, N>::vertices_const_iterator itV =  mesh->beginVertices();
      const typename sd::core::MeshView_<T, N>::vertices_const_iterator endV =  mesh->endVertices();
      for (; itV!=endV; ++itV) {
	outfile << (*itV).x() << " " << (*itV).y() << " " << (*itV).z();
	if (saveVertexNormals) {
	  outfile << " " << (*itV).getNormal().x() << " " << (*itV).getNormal().y() << " " << (*itV).getNormal().z();
	}
	outfile << std::endl;
      }
      
      outfile.close();
      
      return true;
    }
    
  }
  
}
#endif /*! SD_ASC_SAVER_ASC_HXX */
