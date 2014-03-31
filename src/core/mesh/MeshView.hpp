/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_CORE_MESH_VIEW_HPP
#define SD_CORE_MESH_VIEW_HPP

#include "../ViewNode.hpp"

//#include "RawMesh.hpp"

#include <vector>
#include <typeinfo>

namespace sd {
  
  namespace core {

    typedef DataType VertexDataType;
    
    class MeshView : public ViewNode {

    public:
      
      typedef unsigned int vertex_id;
      
    public:
      
      virtual ~MeshView();
      
      virtual VertexDataType getVertexDataType() const = 0;
      virtual unsigned int getFaceDegree() const = 0;
      //virtual RawMesh* getRawMesh() = 0;// const = 0;
      
      static inline vertex_id getVertexIdFromIndex(unsigned int index) {return index/3;}
      static inline unsigned int getIndexFromVertexId(vertex_id vertexId) {return vertexId*3;}
      
      virtual void* getVertexRawData() = 0;

      // Run it first before all if the default constructor has been used (for example in a plugins...)
      virtual void build() = 0;

    protected:
      
      inline MeshView(ViewNode* previousViewNode, const std::string& name);
      inline MeshView(std::vector<ViewNode*>& previousViewNodes, const std::string& name);
      
    private:
      
      MeshView(const sd::core::MeshView&) = delete;
      
    };
    
  }
}

#include "MeshView.hxx"

#endif /* ! SD_CORE_MESH_VIEW_HPP */
