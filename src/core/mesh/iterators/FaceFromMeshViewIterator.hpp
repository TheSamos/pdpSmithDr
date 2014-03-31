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

#ifndef SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HPP
#define SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HPP

#include "VertexBasedFromMeshViewIterator.hpp"

#include "../MeshView.hpp"
#include "../VertexFromMeshView.hpp"
#include "../FaceFromMeshView.hpp"

namespace sd {
  namespace core {

    template<typename T, unsigned int N>
    class FaceFromMeshViewIterator : public VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> > {

    public:

      FaceFromMeshViewIterator(MeshView* askingView,
			       std::map<MeshView*,
					std::vector<std::vector<FaceFromMeshView<T, N>* > > >* basedVertex);

      FaceFromMeshViewIterator(const FaceFromMeshViewIterator<T,N>&);

      virtual ~FaceFromMeshViewIterator();

      const FaceFromMeshViewIterator<T,N>& operator=(const FaceFromMeshViewIterator<T,N>&);

    };

    template<typename T, unsigned int N>
    bool operator==(const sd::core::FaceFromMeshViewIterator<T, N>& fi1,
		    const sd::core::FaceFromMeshViewIterator<T, N>& fi2);

    template<typename T, unsigned int N>
    bool operator!=(const sd::core::FaceFromMeshViewIterator<T, N>& fi1,
		    const sd::core::FaceFromMeshViewIterator<T, N>& fi2);

  }
}

#include "FaceFromMeshViewIterator.hxx"

#endif /* ! SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HPP */
