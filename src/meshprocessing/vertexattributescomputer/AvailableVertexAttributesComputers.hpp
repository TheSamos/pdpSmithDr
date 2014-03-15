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

#ifndef SD_MESH_PROCESSING_AVAILABLE_MESH_VERTEX_ATTRIBUTES_COMPUTERS_HPP
#define SD_MESH_PROCESSING_AVAILABLE_MESH_VERTEX_ATTRIBUTES_COMPUTERS_HPP

#include <meshprocessing/AvailableToolsManager.hpp>

#include "VertexAttributesComputer.hpp"

#include "NeighborhoodARComputer.hpp"

namespace sd {

  namespace meshprocessing {

    namespace vertexattributescomputer {
      
      template<typename T, unsigned int N>
      class AvailableVertexAttributesComputers : public AvailableToolsManager<sd::meshprocessing::vertexattributescomputer::VertexAttributesComputer<T, N> > {
  
      public:
  
	AvailableVertexAttributesComputers() : AvailableToolsManager<sd::meshprocessing::vertexattributescomputer::VertexAttributesComputer<T, N> >() {
	  this->load();
	  this->loadNames();
	}
  
	virtual ~AvailableVertexAttributesComputers() {}
       
      protected:
	  
	virtual void load() {
	  this->data() = {new sd::meshprocessing::vertexattributescomputer::NeighborhoodARComputer<T, N>(),
	  };
	}
	
      };

    }

  }

}

#endif /*! SD_MESH_PROCESSING_AVAILABLE_MESH_VERTEX_ATTRIBUTES_COMPUTERS_HPP */
