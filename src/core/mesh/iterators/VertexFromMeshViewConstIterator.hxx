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

#ifndef SD_CORE_VERTEX_FROM_MESH_VIEW_CONST_ITERATOR_HXX
#define SD_CORE_VERTEX_FROM_MESH_VIEW_CONST_ITERATOR_HXX

template<typename T>
sd::core::VertexFromMeshViewConstIterator<T>::VertexFromMeshViewConstIterator(const MeshView* askingView, 
									      const std::map<MeshView*,
										       std::list<std::pair<MeshView::vertex_id,
													   MeshView::vertex_id> > >& verticesIds)
  : m_askingView((MeshView*)askingView),
    m_verticesIds(verticesIds),
    m_currentId(),
    m_intervalIt(),
    m_mapIt()
{}

template<typename T>
void
sd::core::VertexFromMeshViewConstIterator<T>::setBegin() {
  m_mapIt = m_verticesIds.begin();
  while(m_mapIt != m_verticesIds.end()
	&& (*m_mapIt).second.begin() == (*m_mapIt).second.end())
    ++m_mapIt;
  
  if (m_mapIt == m_verticesIds.end())
    return;
  
  m_intervalIt = (*m_mapIt).second.begin();
  m_currentId = (*m_intervalIt).first;
}

template<typename T>
void
sd::core::VertexFromMeshViewConstIterator<T>::setEnd() {
  m_mapIt = m_verticesIds.end();
}

template<typename T>
const sd::core::VertexFromMeshView<T>
sd::core::VertexFromMeshViewConstIterator<T>::operator*() const {
  return VertexFromMeshView<T>(m_askingView,
			       (*m_mapIt).first,
			       m_currentId);
}

template<typename T>
sd::core::VertexFromMeshViewConstIterator<T>&
sd::core::VertexFromMeshViewConstIterator<T>::operator++() {
  *this+=1;
  return *this;
}

template<typename T>
sd::core::VertexFromMeshViewConstIterator<T>&
sd::core::VertexFromMeshViewConstIterator<T>::operator+=(unsigned int inc) {
  for (unsigned int i=0; i<inc; ++i) {
    MeshView::vertex_id newId = m_currentId+1;
    while (m_mapIt != m_verticesIds.end()
	   && newId >= (*m_intervalIt).second) {
      ++m_intervalIt;
      while (m_intervalIt == (*m_mapIt).second.end()) {
	++m_mapIt;
	if (m_mapIt != m_verticesIds.end())
	  m_intervalIt = (*m_mapIt).second.begin();
	else
	  return *this;
      }
      if (m_mapIt != (m_verticesIds).end()
	  && m_intervalIt != (*m_mapIt).second.end()) {
	newId = (*m_intervalIt).first;
      }
    }
    m_currentId = newId;
  }
  return *this;
}

template<typename T>
sd::core::VertexFromMeshViewConstIterator<T>
sd::core::VertexFromMeshViewConstIterator<T>::operator+(unsigned int inc) {
  VertexFromMeshViewConstIterator<T> r(*this);
  r+=inc;
  return r;
}

template<typename T>
bool
sd::core::VertexFromMeshViewConstIterator<T>::operator==(const sd::core::VertexFromMeshViewConstIterator<T>& vi) const {
  return ((*this).m_mapIt == vi.m_mapIt && 
	  (((*this).m_mapIt == (*this).m_verticesIds.end())
	   || ((*this).m_intervalIt == vi.m_intervalIt && (*this).m_currentId == vi.m_currentId)));
}

template<typename T>
bool
sd::core::VertexFromMeshViewConstIterator<T>::operator!=(const sd::core::VertexFromMeshViewConstIterator<T>& vi) const {
  return !(*this == vi);
}

#endif /*! SD_CORE_VERTEX_FROM_MESH_VIEW_CONST_ITERATOR_HXX */
