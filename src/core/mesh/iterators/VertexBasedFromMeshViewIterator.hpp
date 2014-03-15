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

#ifndef SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HPP
#define SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HPP

#include "../MeshView.hpp"
#include "../VertexFromMeshView.hpp"
#include "../VertexBasedFromMeshView.hpp"

namespace sd {
  
  namespace core {

    template<typename T, unsigned int N, class U>
    class VertexBasedFromMeshViewIterator {

    public:

      VertexBasedFromMeshViewIterator(MeshView* askingView, 
				      std::map<MeshView*, std::vector<std::vector<U*> > >* basedVertex);

      // shallow copy
      VertexBasedFromMeshViewIterator(const VertexBasedFromMeshViewIterator<T,N,U>&);

      virtual ~VertexBasedFromMeshViewIterator();

      // shallow copy
      const VertexBasedFromMeshViewIterator<T,N,U>& operator=(const VertexBasedFromMeshViewIterator<T,N,U>&);

      void setBegin();

      void setEnd();

      U*& operator*();

      VertexBasedFromMeshViewIterator<T, N, U>& operator++();

      VertexBasedFromMeshViewIterator<T, N, U>& operator+=(int i);

      bool operator==(const VertexBasedFromMeshViewIterator<T, N, U>& vi) const;
      bool operator!=(const VertexBasedFromMeshViewIterator<T, N, U>& vi) const;

    private:

      MeshView* m_askingView;

      std::map<MeshView*, std::vector<std::vector<U*> > >* m_basedVertex;

      typename std::map<MeshView*, std::vector<std::vector<U*> > >::iterator m_mapIt;

      int m_currentIdIndex; // std::vector<std::vector<U*> >::iterator

      int m_currentVertexBasedFromMeshViewIndex; // std::vector<U*>::iterator

    };

  }
  
}

#include "VertexBasedFromMeshViewIterator.hxx"

#endif /* ! SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HPP */
