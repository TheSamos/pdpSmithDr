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

#ifndef SD_CORE_VERTEX_BASED_FROM_MESH_HXX
#define SD_CORE_VERTEX_BASED_FROM_MESH_HXX

namespace sd {
  namespace core {
    template<typename T, unsigned int N>
    class MeshView_;
  }
}

#ifndef SD_CORE_MESH_VIEW__HPP
#include "MeshView_.hpp"
#endif /*! SD_CORE_MESH_VIEW__HPP */

#include <algorithm>

#include <assert.h>

template<typename T, unsigned int N>
sd::core::VertexBasedFromMeshView<T, N>::VertexBasedFromMeshView(sd::core::MeshView* creationView)
  : m_data(),
    m_creationView(creationView),
    m_removeViews(0) {
  assert(creationView);
}

template<typename T, unsigned int N>
sd::core::VertexBasedFromMeshView<T, N>::VertexBasedFromMeshView(const VertexBasedFromMeshView<T,N>& rhs)
  : m_data(rhs.m_data),
    m_creationView(rhs.m_creationView),
    m_removeViews(rhs.m_removeViews) {}

template<typename T, unsigned int N>
sd::core::VertexBasedFromMeshView<T, N>::~VertexBasedFromMeshView() {}

template<typename T, unsigned int N>
const sd::core::VertexBasedFromMeshView<T,N>&
sd::core::VertexBasedFromMeshView<T, N>::operator=(const VertexBasedFromMeshView<T,N>& rhs) {
  if (&rhs != this) {
    m_data = rhs.m_data;
    m_creationView = rhs.m_creationView;
    m_removeViews = rhs.m_removeViews;
  }
  return *this;
}

template<typename T, unsigned int N>
void
sd::core::VertexBasedFromMeshView<T, N>::setAt(unsigned int index,
					       const VertexFromMeshView<T>& vertex) {
  m_data[index].first = (MeshView*)vertex.containingView(); m_data[index].second = vertex.id();
}

template<typename T, unsigned int N>
sd::core::VertexFromMeshView<T>
sd::core::VertexBasedFromMeshView<T, N>::getAt(MeshView* askingView, unsigned int index) {
  assert(isValidAtView(askingView) || (std::find(m_removeViews.begin(),
						 m_removeViews.end(),
						 m_creationView) != m_removeViews.end()
						 && m_creationView == askingView));
  return VertexFromMeshView<T>(askingView,
			       m_data[index].first,
			       m_data[index].second);
}

template<typename T, unsigned int N>
const sd::core::VertexFromMeshView<T>
sd::core::VertexBasedFromMeshView<T, N>::getAt(MeshView* askingView, unsigned int index) const {
  assert(isValidAtView(askingView));
  return VertexFromMeshView<T>(askingView,
			       m_data[index].first,
			       m_data[index].second);
}

template<typename T, unsigned int N>
unsigned int
sd::core::VertexBasedFromMeshView<T, N>::find(const VertexFromMeshView<T>& v) const {
  for (unsigned int i = 0; i<N; ++i)
    if (v.containingView() == m_data[i].first && v.id() == m_data[i].second)
      return i;
  return N;
}
     
template<typename T, unsigned int N>
unsigned int
sd::core::VertexBasedFromMeshView<T, N>::size() const {
  return N;
}

template<typename T, unsigned int N>
bool
sd::core::VertexBasedFromMeshView<T, N>::isValidAtView(MeshView* view) const {
  if (isDeletable() || !(*m_creationView <= *view))
    return false;
  bool isValid = true;
  for (unsigned int i=0; i<m_removeViews.size(); ++i) {
    isValid &= !(*view >= *m_removeViews[i]);
    if (!isValid) return false;
  }
  return true;
}

template<typename T, unsigned int N>
void
sd::core::VertexBasedFromMeshView<T, N>::removeAt(MeshView* view) {
  if (view == m_creationView) {
    m_removeViews.resize(1);
    m_removeViews[0] = (MeshView*)m_creationView;
    return;
  }
  bool added = false;
  for (unsigned int i=0; i<m_removeViews.size(); ++i) {
    if (*m_removeViews[i] >= *view) {
      m_removeViews[i] = view;
      added = true;
    }
  }
  if (!added)
    m_removeViews.push_back(view);
  else { // Remove duplicate
    m_removeViews.resize(unique(m_removeViews.begin(), m_removeViews.end())
			 - m_removeViews.begin());
  }
}

template<typename T, unsigned int N>
void
sd::core::VertexBasedFromMeshView<T, N>::isNoMore(MeshView* view) {
  if (view == m_creationView) {
    m_removeViews.resize(1);
    m_removeViews[0] = (MeshView*)m_creationView;
    return;
  }
  const std::vector<MeshView*>::iterator it = std::find(m_removeViews.begin(),
							m_removeViews.end(), view);
  if (it != m_removeViews.end())
    m_removeViews.erase(it);
}

template<typename T, unsigned int N>
bool
sd::core::VertexBasedFromMeshView<T, N>::isDeletable() const {
  return (m_removeViews.size() == 1 && m_creationView == m_removeViews[0]);
}

template<typename T, unsigned int N>
template<typename U>
bool
sd::core::VertexBasedFromMeshView<T, N>::isNormalSet(sd::core::MeshView_<T, N>* askingView) const {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  return askingView->isAttributeSet((U*)this, normalKey);
}

template<typename T, unsigned int N>
template<typename U>
sd::core::Normal<T>
sd::core::VertexBasedFromMeshView<T, N>::getNormal(sd::core::MeshView_<T, N>* askingView) const {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (askingView->isAttributeSet((U*)this, normalKey)) {
    return askingView->template getAttribute<typename sd::core::Normal<T> >((U*)this, normalKey);
  } else {
    sd::core::Normal<T> normal = (sd::core::Vector3d<typename sd::core::Normal<T>::type>(getAt(askingView, 0),
											 getAt(askingView, 1))
				  ^ sd::core::Vector3d<typename sd::core::Normal<T>::type>(getAt(askingView, 0),
											   getAt(askingView, 2)));
    normal.normalize();
    return normal;
  }
}

template<typename T, unsigned int N>
template<typename U>
void
sd::core::VertexBasedFromMeshView<T, N>::setNormal(const sd::core::Normal<T>& normal,
						   sd::core::MeshView_<T, N>* askingView) {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  askingView->template setAttribute<typename sd::core::Normal<T> >((U*)this, normalKey, normal);
}

template<typename T, unsigned int N>
template<typename U>
bool
sd::core::VertexBasedFromMeshView<T, N>::isRGBCoordinatesSet(sd::core::MeshView_<T, N>* askingView) const {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  return askingView->isAttributeSet((U*)this, colorKey);
}

template<typename T, unsigned int N>
template<typename U>
const sd::core::RGBCoordinates<unsigned char>&
sd::core::VertexBasedFromMeshView<T, N>::getRGBCoordinates(sd::core::MeshView_<T, N>* askingView) const {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  return askingView->template getAttribute<typename sd::core::RGBCoordinates<unsigned char> >((U*)this, colorKey);
}

template<typename T, unsigned int N>
template<typename U>
void
sd::core::VertexBasedFromMeshView<T, N>::setRGBCoordinates(const sd::core::RGBCoordinates<unsigned char>& rgb,
							   sd::core::MeshView_<T, N>* askingView) {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  assert(askingView->getVertexDataType() == sd::core::DataType_<T>());
  askingView->template setAttribute<typename sd::core::RGBCoordinates<unsigned char> >((U*)this, colorKey, rgb);
}

template<typename T, unsigned int N>
template<typename U>
inline bool
sd::core::VertexBasedFromMeshView<T, N>::isAttributeSet(const sd::core::AttributeKey& akey,
			   sd::core::MeshView_<T, N>* askingView) const {
  return askingView->template isAttributeSet((U*)this, akey);
}

template<typename T, unsigned int N>
template<typename U, typename A>
inline const A&
sd::core::VertexBasedFromMeshView<T, N>::getAttribute(const AttributeKey& akey,
						      sd::core::MeshView_<T, N>* askingView) {
  return askingView->template getAttribute<A>((U*)this, akey);
}

template<typename T, unsigned int N>
template<typename U, typename A>
inline void
sd::core::VertexBasedFromMeshView<T, N>::setAttribute(const AttributeKey& akey,
						      const A& val,
						      sd::core::MeshView_<T, N>* askingView) {
  askingView->template setAttributeSet((U*)this, val, akey);
}

#endif /*! SD_CORE_VERTEX_BASED_FROM_MESH_HXX */

