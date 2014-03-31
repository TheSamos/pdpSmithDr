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

#ifndef SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HXX
#define SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HXX

template<typename T, unsigned int N>
sd::core::FaceFromMeshViewIterator<T, N>::FaceFromMeshViewIterator(MeshView* askingView, 
								   std::map<MeshView*,
									    std::vector<std::vector<FaceFromMeshView<T, N>* > > >* faces)
  : VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> >(askingView, (faces))
{}

template<typename T, unsigned int N>
sd::core::FaceFromMeshViewIterator<T, N>::FaceFromMeshViewIterator(const FaceFromMeshViewIterator<T,N>& rhs)
  : VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> >(rhs)
{}

template<typename T, unsigned int N>
sd::core::FaceFromMeshViewIterator<T, N>::~FaceFromMeshViewIterator()
{}

template<typename T, unsigned int N>
const sd::core::FaceFromMeshViewIterator<T,N>&
sd::core::FaceFromMeshViewIterator<T, N>::operator=(const FaceFromMeshViewIterator<T,N>& rhs) {
  if (&rhs != this) {
    VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> >::operator=(rhs);
  }
  return *this;
}

template<typename T, unsigned int N>
bool
sd::core::operator==(const sd::core::FaceFromMeshViewIterator<T, N>& fi1,
		     const sd::core::FaceFromMeshViewIterator<T, N>& fi2) {
  return (static_cast<VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> > >(fi1)
	  == static_cast<VertexBasedFromMeshViewIterator<T, N, FaceFromMeshView<T, N> > >(fi2));
}
    
template<typename T, unsigned int N>
bool
sd::core::operator!=(const sd::core::FaceFromMeshViewIterator<T, N>& fi1,
		     const sd::core::FaceFromMeshViewIterator<T, N>& fi2) {
  return !(fi1 == fi2);
}

#endif /*! SD_CORE_FACE_FROM_MESH_VIEW_ITERATOR_HXX */
