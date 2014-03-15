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

#ifndef SD_MESH_PROCESSING_GENERATORS_MESH_GENERATOR_HPP
#define SD_MESH_PROCESSING_GENERATORS_MESH_GENERATOR_HPP

#include <core/Point3d.hpp>

#include <core/mesh/MeshView_.hpp>

#include <vector>

namespace sd {

  namespace meshprocessing {
    
    namespace generator {

      template<typename T, unsigned int N>
      class MeshGenerator {

      public:
	
	virtual std::string name() const = 0;
	
	virtual sd::core::MeshView_<T, N>* getMesh() = 0;
	
	virtual ~MeshGenerator() {}
	
      protected:
	
	MeshGenerator() {}
	
      };
      
    }
    
  }

}

#endif /*! SD_MESH_PROCESSING_GENERATORS_MESH_GENERATOR_HPP */
