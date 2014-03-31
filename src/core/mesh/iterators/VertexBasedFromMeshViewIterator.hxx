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

#ifndef SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HXX
#define SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HXX

template<typename T, unsigned int N, class U>
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::VertexBasedFromMeshViewIterator(MeshView* askingView,
										    std::map<MeshView*, std::vector<std::vector<U*> > >* basedVertex)
  : m_askingView(askingView),
    m_basedVertex(basedVertex),
    m_mapIt(),
    m_currentIdIndex(),
    m_currentVertexBasedFromMeshViewIndex()
{}

template<typename T, unsigned int N, class U>
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::VertexBasedFromMeshViewIterator(const VertexBasedFromMeshViewIterator<T,N,U>& rhs)
  : m_askingView(rhs.m_askingView), // pointer copy
    m_basedVertex(rhs.m_basedVertex), // pointer copy
    m_mapIt(rhs.m_mapIt),
    m_currentIdIndex(rhs.m_currentIdIndex),
    m_currentVertexBasedFromMeshViewIndex(rhs.m_currentVertexBasedFromMeshViewIndex)
{}

template<typename T, unsigned int N, class U>
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::~VertexBasedFromMeshViewIterator() {
  // trivial
}

template<typename T, unsigned int N, class U>
const sd::core::VertexBasedFromMeshViewIterator<T,N,U>&
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator=(const VertexBasedFromMeshViewIterator<T,N,U>& rhs) {
  if (&rhs != this) {
    m_askingView = rhs.m_askingView; // pointer copy
    m_basedVertex = rhs.m_basedVertex; // pointer copy
    m_mapIt = rhs.m_mapIt;
    m_currentIdIndex = rhs.m_currentIdIndex;
    m_currentVertexBasedFromMeshViewIndex = rhs.m_currentVertexBasedFromMeshViewIndex;
  }
  return *this;
}

template<typename T, unsigned int N, class U>
void
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::setBegin() {
  m_mapIt = m_basedVertex->begin();
  m_currentIdIndex = 0;
  m_currentVertexBasedFromMeshViewIndex = 0;
  
  if (m_mapIt == m_basedVertex->end()) return;

  //:TODO: Check existence  
  if ((*m_mapIt).second.size() == 0
      ||(*m_mapIt).second[m_currentIdIndex].size() == 0
      || !((*m_mapIt).second[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->isValidAtView(m_askingView))
      || !((int)((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->getAt(m_askingView, 0).id() == m_currentIdIndex))
    ++(*this);
}

template<typename T, unsigned int N, class U>
void
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::setEnd() {
  m_mapIt = m_basedVertex->end();
}

template<typename T, unsigned int N, class U>
U*&
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator*() {
  return ((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex];
}

template<typename T, unsigned int N, class U>
sd::core::VertexBasedFromMeshViewIterator<T, N, U>&
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator++() {
  (*this)+=1;
  return *this;
}

template<typename T, unsigned int N, class U>
sd::core::VertexBasedFromMeshViewIterator<T, N, U>&
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator+=(int i) {
  int remainingIncrements = i;
  while (remainingIncrements > 0
	 && m_mapIt != m_basedVertex->end()) {
    ++m_currentVertexBasedFromMeshViewIndex;
    if ((unsigned int)m_currentVertexBasedFromMeshViewIndex >= (*m_mapIt).second[m_currentIdIndex].size()) {
      ++m_currentIdIndex;
      m_currentVertexBasedFromMeshViewIndex = 0;
      if ((unsigned int)m_currentIdIndex == (*m_mapIt).second.size()) {
	++m_mapIt;
	if (m_mapIt != m_basedVertex->end())
	  m_currentIdIndex = 0;
      }
    }
    
    
    if (m_mapIt != m_basedVertex->end())
      if ((int)((*m_mapIt).second)[m_currentIdIndex].size() > m_currentVertexBasedFromMeshViewIndex)// 02:03:2012 : J : See why it's need on simple dataset
	if (((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->isValidAtView(m_askingView))
	  if ((int) ((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->getAt(m_askingView, 0).id() == m_currentIdIndex)
	  --remainingIncrements;
  }
  while (remainingIncrements < 0
	 && m_currentIdIndex != -1) {
      --m_currentVertexBasedFromMeshViewIndex;
      if (m_currentVertexBasedFromMeshViewIndex == -1) {
	--m_currentIdIndex;
	m_currentVertexBasedFromMeshViewIndex = (int) ((*m_mapIt).second)[m_currentIdIndex].size()-1;
	if (m_currentIdIndex == 0) {
	  if (m_mapIt != m_basedVertex->begin()) {
	    --m_mapIt;
	    m_currentIdIndex = (int) (*m_mapIt).second.size()-1;
	  } else {
	    m_currentIdIndex = -1;
	  }
	}
      }
      
      if (m_currentIdIndex != -1
	  && ((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->isValidAtView(m_askingView)
	  && (int) ((*m_mapIt).second)[m_currentIdIndex][m_currentVertexBasedFromMeshViewIndex]->getAt(m_askingView, 0).id() == m_currentIdIndex)
	++remainingIncrements;
  }

  return *this;
}

template<typename T, unsigned int N, class U>
bool
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator==(const VertexBasedFromMeshViewIterator<T, N, U>& vi) const {
return (this->m_mapIt == vi.m_mapIt && 
	  ((this->m_mapIt == this->m_basedVertex->end())
	   || (this->m_currentIdIndex == vi.m_currentIdIndex && this->m_currentVertexBasedFromMeshViewIndex == vi.m_currentVertexBasedFromMeshViewIndex)));
}

template<typename T, unsigned int N, class U>
bool
sd::core::VertexBasedFromMeshViewIterator<T, N, U>::operator!=(const VertexBasedFromMeshViewIterator<T, N, U>& vi) const {
  return !(*this == vi);
}


/*
  unsigned int newVertexBasedFromMeshViewIndex = m_currentVertexBasedFromMeshViewIndex + i;
  while (newVertexBasedFromMeshViewIndex > (*m_mapIt)[m_currentIdIndex].size();
	 && m_mapIt != m_basedVertex->end()) {
    newVertexBasedFromMeshViewIndex -= (*m_mapIt)[m_currentIdIndex].size()
    ++m_currentIdIndex;
    if (m_currentIdIndex == (*m_mapIt).size()) {
      ++m_mapIt;
    }
  }
  m_currentVertexBasedFromMeshViewIndex = newVertexBasedFromMeshViewIndex;
  return *this;

}

template<typename T, unsigned int N, class U>
bool
sd::core::operator==(const sd::core::VertexBasedFromMeshViewIterator<T, N, U>& vi1, const sd::core::VertexBasedFromMeshViewIterator<T, N, U>& vi2) {
  return (vi1.m_mapIt == vi2.m_mapIt && 
	  ((vi1.m_mapIt == vi1.m_basedVertex->end())
	   || (vi1.m_currentIdIndex == vi2.m_currentIdIndex && vi1.m_currentVertexBasedFromMeshViewIndex == vi2.m_currentVertexBasedFromMeshViewIndex)));
}

template<typename T, unsigned int N, class U>
bool
sd::core::operator!=(const sd::core::VertexBasedFromMeshViewIterator<T, N, U>& vi1, const sd::core::VertexBasedFromMeshViewIterator<T, N, U>& vi2) {
  return !(vi1 == vi2);
}
*/

#endif /*! SD_CORE_VERTEX_BASED_FROM_MESH_VIEW_ITERATOR_HXX */
