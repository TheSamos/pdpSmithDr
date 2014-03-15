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

#ifndef SD_STL_SAVER_STL_HXX
#define SD_STL_SAVER_STL_HXX

#include "SaverStl.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <core/mesh/MeshView_.hpp>

static const char* creator = "# CREATOR: SmithDR";

namespace sd {
  namespace stl {
    
    namespace {

      template<typename T>
      bool
      saveStlAscii(const std::string&, const sd::core::MeshView_<T, 3>*)
      {
	return false;
      }
      
      template<>
      bool
      saveStlAscii(const std::string& filename, const core::MeshView_<FLOAT, 3>* cmesh)
      {

	FILE* file = fopen(filename.c_str(), "w");
	if (file == NULL)
	  return false;
	
	core::MeshView_<FLOAT, 3>* mesh = (core::MeshView_<FLOAT, 3>*) cmesh;
	
	fprintf(file,"%s\nsolid %s\n", creator, (mesh->name()).c_str());

	for (auto it = mesh->beginFaces(); it != mesh->endFaces(); ++it) {	  
	    if (!(*it)->isNormalSet(mesh))
	      assert(!"must have normals\n");
	    auto normal = (*it)->getNormal(mesh);
	    //write normal
	    fprintf(file, "  facet normal %13e %13e %13e\n    outer loop\n",normal[0],normal[1],normal[2]);	   
	    for (int i = 0; i < 3; ++i) {
	      auto vertex = (*it)->getAt(mesh, i);
	      //write vertex
	      fprintf(file,"      vertex  %13e %13e %13e\n",vertex.x(),vertex.y(),vertex.z());   	     
	    }
	    fprintf(file,"    endloop\n  endfacet\n");
	}
	fprintf(file,"endsolid vcg\n");
	fclose(file);
	return true;
      }
      
      template<typename T>
      bool
      saveStlBinary(const std::string&, const sd::core::MeshView_<T, 3>*)
      {
	return false;
      }
      
      template<>
      bool
      saveStlBinary(const std::string& filename, const sd::core::MeshView_<FLOAT, 3>* cmesh)
      {
	FILE* file = fopen(filename.c_str(), "w");
	if (file == NULL)
	  return false;
	core::MeshView_<FLOAT, 3>* mesh = (core::MeshView_<FLOAT, 3>*) cmesh;
	int nb_faces = mesh->nbFaces();
	
	char head[80] ;
	strncpy(head, (mesh->name()).c_str(), 80);
	fwrite(head,80,1,file);
	// nb faces
	fwrite(&nb_faces,1,sizeof(int),file); 
	unsigned short byte_count = 0;

	for (auto it = mesh->beginFaces(); it != mesh->endFaces(); ++it) {
	  
	    if (!(*it)->isNormalSet(mesh))
	      assert(!"must have normals\n");
	    auto normal = (*it)->getNormal(mesh);
	    //write normal
	    fwrite(&normal[0],3,sizeof(float),file);
    	   
	    for (int i = 0; i < 3; ++i) {
	      auto vertex = (*it)->getAt(mesh, i);
	      //write vertex
	      fwrite(&vertex.x(),1,sizeof(float),file);
	      fwrite(&vertex.y(),1,sizeof(float),file);
	      fwrite(&vertex.z(),1,sizeof(float),file);
	      	     
	    }
	    fwrite(&byte_count,1,sizeof(short),file);

	}
	fclose(file);
	return true;
      }
      
    }
    
    template<typename T>
    bool
    SaverStl<T>::saveStlImage(const core::MeshView_<T, 3>* mesh, const std::string& filename, bool binary) const
    {
      if (mesh->getFaceDegree() != 3) // only work with dergree = 3
	return false;
      if (!binary)
	return saveStlBinary(filename, mesh);
      return saveStlAscii(filename, mesh);
    }
  }
}
#endif /*! SD_STL_SAVER_STL_HXX */
