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

#ifndef SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HPP
#define SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HPP

#include "VertexBasedFromMeshViewIterator.hpp"
#include "../MeshView.hpp"
#include "../VertexFromMeshView.hpp"
#include "../EdgeFromMeshView.hpp"

namespace sd {
  namespace core {

    template<typename T>
    class EdgeFromMeshViewIterator
      : public VertexBasedFromMeshViewIterator<T, 2, EdgeFromMeshView<T> >
    {

    public:

      EdgeFromMeshViewIterator(MeshView* askingView,
			       std::map<MeshView*,
					std::vector<std::vector<EdgeFromMeshView<T>* > > >* basedVertex);

    };

    template<typename T>
    bool operator==(const sd::core::EdgeFromMeshViewIterator<T>& ei1,
		    const sd::core::EdgeFromMeshViewIterator<T>& ei2);

    template<typename T>
    bool operator!=(const sd::core::EdgeFromMeshViewIterator<T>& ei1,
		    const sd::core::EdgeFromMeshViewIterator<T>& ei2);

  }
}

#include "EdgeFromMeshViewIterator.hxx"

#endif /* ! SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HPP */
