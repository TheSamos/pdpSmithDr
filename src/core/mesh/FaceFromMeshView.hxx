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

#ifndef SD_CORE_FACE_FROM_MESH_VIEW_HXX
#define SD_CORE_FACE_FROM_MESH_VIEW_HXX

#include <cmath>

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>::FaceFromMeshView(sd::core::MeshView_<T, N>* creationView)
  : VertexBasedFromMeshView<T, N>(creationView)
{}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>::~FaceFromMeshView() {}

template<typename T, unsigned int N>
inline bool
sd::core::FaceFromMeshView<T, N>::isNormalSet(sd::core::MeshView_<T, N>* askingView) const {
  return VertexBasedFromMeshView<T, N>::template isNormalSet<FaceFromMeshView<T, N> >(askingView);
}

template<typename T, unsigned int N>
inline sd::core::Normal<T> 
sd::core::FaceFromMeshView<T, N>::getNormal(sd::core::MeshView_<T, N>* askingView) const {
  return VertexBasedFromMeshView<T, N>::template getNormal<FaceFromMeshView<T, N> >(askingView);
}

template<typename T, unsigned int N>
inline void 
sd::core::FaceFromMeshView<T, N>::setNormal(const Normal<T>& normal, sd::core::MeshView_<T, N>* askingView) {
  VertexBasedFromMeshView<T, N>::template setNormal<FaceFromMeshView<T, N> >(normal, askingView);
}

template<typename T, unsigned int N>
inline bool 
sd::core::FaceFromMeshView<T, N>::isRGBCoordinatesSet(sd::core::MeshView_<T, N>* askingView) const {
  return VertexBasedFromMeshView<T, N>::template isRGBCoordinatesSet<FaceFromMeshView<T, N> >(askingView);
}

template<typename T, unsigned int N>
inline const sd::core::RGBCoordinates<unsigned char>& 
sd::core::FaceFromMeshView<T, N>::getRGBCoordinates(sd::core::MeshView_<T, N>* askingView) const {
  return VertexBasedFromMeshView<T, N>::template getRGBCoordinates<FaceFromMeshView<T, N> >(askingView);
}

template<typename T, unsigned int N>
inline void 
sd::core::FaceFromMeshView<T, N>::setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb,
						    sd::core::MeshView_<T, N>* askingView) {
  VertexBasedFromMeshView<T, N>::template setRGBCoordinates<FaceFromMeshView<T, N> >(rgb, askingView);
}

      
template<typename T, unsigned int N>
inline bool 
sd::core::FaceFromMeshView<T, N>::isAttributeSet(const sd::core::AttributeKey& akey,
						 sd::core::MeshView_<T, N>* askingView) const {
 return VertexBasedFromMeshView<T, N>::template isAttributeSet<FaceFromMeshView<T, N> >(akey, askingView);
}

template<typename T, unsigned int N>
template<typename A>
inline const A& 
sd::core::FaceFromMeshView<T, N>::getAttribute(const AttributeKey& akey,
					       sd::core::MeshView_<T, N>* askingView) {
  return VertexBasedFromMeshView<T, N>::template getAttribute<FaceFromMeshView<T, N>, A >(akey, askingView);
}

template<typename T, unsigned int N>
template<typename A>
inline void 
sd::core::FaceFromMeshView<T, N>::setAttribute(const AttributeKey& akey, const A& val,
					       sd::core::MeshView_<T, N>* askingView) {
  VertexBasedFromMeshView<T, N>::template setAttribute<FaceFromMeshView<T, N>, A >(akey, val, askingView);
}

template<typename T>
sd::core::TriangularFaceFromMeshView<T>::TriangularFaceFromMeshView(sd::core::MeshView_<T, 3>* creationView)
  : FaceFromMeshView<T, 3>(creationView)
{}

template<typename T>
sd::core::TriangularFaceFromMeshView<T>::TriangularFaceFromMeshView(sd::core::MeshView_<T, 3>* creationView,
								    const sd::core::VertexFromMeshView<T>& v0,
								    const sd::core::VertexFromMeshView<T>& v1,
								    const sd::core::VertexFromMeshView<T>& v2)
  : FaceFromMeshView<T, 3>(creationView) {
  this->setAt(0, v0);
  this->setAt(1, v1);
  this->setAt(2, v2);
  this->setNormal(this->getNormal(creationView), creationView);
}

template<typename T>
sd::core::QuadrangularFaceFromMeshView<T>::QuadrangularFaceFromMeshView(sd::core::MeshView_<T, 4>* creationView)
  : FaceFromMeshView<T, 4>(creationView)
{}

template<typename T>
sd::core::QuadrangularFaceFromMeshView<T>::QuadrangularFaceFromMeshView(sd::core::MeshView_<T, 4>* creationView,
									const sd::core::VertexFromMeshView<T>& v0,
									const sd::core::VertexFromMeshView<T>& v1,
									const sd::core::VertexFromMeshView<T>& v2,
									const sd::core::VertexFromMeshView<T>& v3)
  : FaceFromMeshView<T, 4>(creationView) {
  this->setAt(0, v0);
  this->setAt(1, v1);
  this->setAt(2, v2);
  this->setAt(3, v3);
  this->setNormal(this->getNormal(creationView), creationView);
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::clone(sd::core::MeshView_<T, N>* creationView,
		sd::core::FaceFromMeshView<T, N>* face) {
  return (N == 3) ?
    newFace(creationView,
	    (*face).getAt(creationView, 0),
	    (*face).getAt(creationView, 1),
	    (*face).getAt(creationView, 2))
    : newFace(creationView,
	      (*face).getAt(creationView, 0),
	      (*face).getAt(creationView, 1),
	      (*face).getAt(creationView, 2),
	      (*face).getAt(creationView, 3));
}
    
template<typename T>
sd::core::FaceFromMeshView<T, 3>*
sd::core::newFace(sd::core::MeshView_<T, 3>* creationView,
		  const sd::core::VertexFromMeshView<T>& v0,
		  const sd::core::VertexFromMeshView<T>& v1,
		  const sd::core::VertexFromMeshView<T>& v2) {
  return new sd::core::TriangularFaceFromMeshView<T>(creationView, v0, v1, v2);
}

template<typename T>
sd::core::FaceFromMeshView<T, 4>*
sd::core::newFace(sd::core::MeshView_<T, 4>* creationView,
		  const sd::core::VertexFromMeshView<T>& v0,
		  const sd::core::VertexFromMeshView<T>& v1,
		  const sd::core::VertexFromMeshView<T>& v2,
		  const sd::core::VertexFromMeshView<T>& v3) {
  return new sd::core::QuadrangularFaceFromMeshView<T>(creationView, v0, v1, v2, v3);
}

template<typename T>
sd::core::FaceFromMeshView<T, 4>*
sd::core::newFace(sd::core::MeshView_<T, 4>* /* creationView */,
		  const sd::core::VertexFromMeshView<T>& /* v0 */,
		  const sd::core::VertexFromMeshView<T>& /* v1 */,
		  const sd::core::VertexFromMeshView<T>& /* v2 */) {
  abort();
  return NULL;
}

template<typename T>
sd::core::FaceFromMeshView<T, 3>*
sd::core::newFace(sd::core::MeshView_<T, 3>* /* creationView */,
		  const sd::core::VertexFromMeshView<T>& /* v0 */,
		  const sd::core::VertexFromMeshView<T>& /* v1 */,
		  const sd::core::VertexFromMeshView<T>& /* v2 */,
		  const sd::core::VertexFromMeshView<T>& /* v3 */) {
  abort();
  return NULL;
}

#endif /*! SD_CORE_FACE_FROM_MESH_VIEW_HXX */
