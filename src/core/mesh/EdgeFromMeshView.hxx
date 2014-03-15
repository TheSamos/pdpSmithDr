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

#ifndef SD_CORE_EDGE_FROM_MESH_VIEW_HXX
#define SD_CORE_EDGE_FROM_MESH_VIEW_HXX

template<typename T>
sd::core::EdgeFromMeshView<T>::EdgeFromMeshView(sd::core::MeshView* creationView)
  : VertexBasedFromMeshView<T, 2>(creationView) {}

template<typename T>
sd::core::EdgeFromMeshView<T>::EdgeFromMeshView(sd::core::MeshView* creationView,
						const sd::core::VertexFromMeshView<T>& v0,
						const sd::core::VertexFromMeshView<T>& v1)
  : VertexBasedFromMeshView<T, 2>(creationView) {
  this->setAt(0, v0);
  this->setAt(1, v1);
}

template<typename T>
const sd::core::VertexFromMeshView<T>
sd::core::EdgeFromMeshView<T>::first(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::getAt(askingView, 0);
}

template<typename T>
const sd::core::VertexFromMeshView<T>
sd::core::EdgeFromMeshView<T>::second(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::getAt(askingView, 1);
}

template<typename T>
inline bool
sd::core::EdgeFromMeshView<T>::isNormalSet(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::template isNormalSet<EdgeFromMeshView<T> >(askingView);
}

template<typename T>
inline sd::core::Normal<T> 
sd::core::EdgeFromMeshView<T>::getNormal(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::template getNormal<EdgeFromMeshView<T> >(askingView);
}

template<typename T>
inline void 
sd::core::EdgeFromMeshView<T>::setNormal(const Normal<T>& normal, sd::core::MeshView* askingView) {
  VertexBasedFromMeshView<T, 2>::template setNormal<EdgeFromMeshView<T> >(normal, askingView);
}

template<typename T>
inline bool 
sd::core::EdgeFromMeshView<T>::isRGBCoordinatesSet(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::template isRGBCoordinatesSet<EdgeFromMeshView<T> >(askingView);
}

template<typename T>
inline const sd::core::RGBCoordinates<unsigned char>& 
sd::core::EdgeFromMeshView<T>::getRGBCoordinates(sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::template getRGBCoordinates<EdgeFromMeshView<T> >(askingView);
}

template<typename T>
inline void 
sd::core::EdgeFromMeshView<T>::setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb,
						 sd::core::MeshView* askingView) {
  VertexBasedFromMeshView<T, 2>::template setRGBCoordinates<EdgeFromMeshView<T> >(rgb, askingView);
}

      
template<typename T>
inline bool 
sd::core::EdgeFromMeshView<T>::isAttributeSet(const sd::core::AttributeKey& akey,
					      sd::core::MeshView* askingView) const {
  return VertexBasedFromMeshView<T, 2>::template isAttributeSet<EdgeFromMeshView<T> >(akey, askingView);
}

template<typename T>
template<typename A>
inline const A& 
sd::core::EdgeFromMeshView<T>::getAttribute(const AttributeKey& akey,
					    sd::core::MeshView* askingView) {
  return VertexBasedFromMeshView<T, 2>::template getAttribute<EdgeFromMeshView<T>, A >(akey, askingView);
}

template<typename T>
template<typename A>
inline void 
sd::core::EdgeFromMeshView<T>::setAttribute(const AttributeKey& akey, const A& val,
					    sd::core::MeshView* askingView) {
  VertexBasedFromMeshView<T, 2>::template setAttribute<EdgeFromMeshView<T>, A >(akey, val, askingView);
}

#endif /*! SD_CORE_EDGE_FROM_MESH_VIEW_HXX */
