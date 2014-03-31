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

#ifndef SD_CORE_VERTEX_FROM_MESH_VIEW_ITERATOR_HPP
#define SD_CORE_VERTEX_FROM_MESH_VIEW_ITERATOR_HPP

#include "../MeshView.hpp"
#include "../VertexFromMeshView.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class VertexFromMeshViewIterator {
      
    public:
      
      VertexFromMeshViewIterator(MeshView* askingView, 
				 const std::map<MeshView*,
						std::list<std::pair<MeshView::vertex_id,
								    MeshView::vertex_id> > >& verticesIds);
      
      void setBegin();

      void setEnd();
      
      VertexFromMeshView<T>& operator*();
      
      const VertexFromMeshView<T>& operator*() const;

      VertexFromMeshViewIterator<T>& operator++();
      
      VertexFromMeshViewIterator<T>& operator+=(unsigned int inc);
      
      VertexFromMeshViewIterator<T> operator+(unsigned int inc);
      
      bool operator==(const VertexFromMeshViewIterator<T>& vi) const;
      bool operator!=(const VertexFromMeshViewIterator<T>& vi) const;
      
    private:
      
      MeshView* m_askingView;

      std::map<MeshView*,
	       std::list<std::pair<MeshView::vertex_id,
				   MeshView::vertex_id> > > m_verticesIds;

      VertexFromMeshView<T> m_lastVertex;
      
      MeshView::vertex_id m_currentId;
      
      std::list<std::pair<MeshView::vertex_id,
			  MeshView::vertex_id> >::iterator m_intervalIt;
      
      std::map<MeshView*,
	       std::list<std::pair<MeshView::vertex_id,
				   MeshView::vertex_id> > >::iterator m_mapIt;

    };
    
  }
  
}

#include "VertexFromMeshViewIterator.hxx"

#endif /* ! SD_CORE_VERTEX_FROM_MESH_VIEW_ITERATOR_HPP */
