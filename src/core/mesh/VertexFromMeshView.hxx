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

#ifndef SD_CORE_VERTEX_FROM_MESH_VIEW_HXX
#define SD_CORE_VERTEX_FROM_MESH_VIEW_HXX

namespace sd {
  namespace core {
    template<typename T, unsigned int N>
    class MeshView_;
  }
}

#ifndef SD_CORE_MESH_VIEW__HPP
#include "MeshView_.hpp"
#endif /* ! SD_CORE_MESH_VIEW__HPP */

#include <iostream> // DEBUG

template<typename T>
sd::core::VertexFromMeshView<T>::VertexFromMeshView()
  : Point3d<T>(),
    m_askingView(NULL),
    m_containingView(NULL),
    m_id(0)
{}

template<typename T>
sd::core::VertexFromMeshView<T>::VertexFromMeshView(MeshView* paskingView,
						    MeshView* pcontainingView,
						    MeshView::vertex_id vid)
  : Point3d<T>(static_cast<sd::core::Vector<T>*>(pcontainingView->getVertexRawData()),
	       pcontainingView->getIndexFromVertexId(vid),
	       false),
    m_askingView(paskingView),
    m_containingView(pcontainingView),
    m_id(vid)
{}

template<typename T>
sd::core::VertexFromMeshView<T>::VertexFromMeshView(const VertexFromMeshView<T>& v)
  : Point3d<T>(),
    m_askingView(v.m_askingView),
    m_containingView(v.m_containingView),
    m_id(v.m_id)
{Coordinates<T>::nocopy(v);}

template<typename T>
sd::core::VertexFromMeshView<T>::~VertexFromMeshView()
{}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isNull() const {
  return m_containingView == NULL;
}

template<typename T>
sd::core::VertexFromMeshView<T>
sd::core::VertexFromMeshView<T>::set(const T& px, const T& py, const T& pz) {
  if (m_containingView != m_askingView
      && m_containingView != NULL && m_askingView != NULL) {
    if (m_askingView->getFaceDegree() == 3) {
      sd::core::MeshView_<T, 3>* askingView = static_cast<sd::core::MeshView_<T, 3>* >(m_askingView);
      sd::core::VertexFromMeshView<T> futur = askingView->addVertex(Point3d<T>(px, py, pz));
      (*this).template swapNeighborhood<3>(askingView, futur);
      askingView->removeVertex(*this);
      *this = futur;
    } else {
      sd::core::MeshView_<T, 4>* askingView = static_cast<sd::core::MeshView_<T, 4>* >(m_askingView);
      sd::core::VertexFromMeshView<T> futur = askingView->addVertex(Point3d<T>(px, py, pz));
      (*this).template swapNeighborhood<4>(askingView, futur);
      askingView->removeVertex(*this);
      *this = futur;
    }
    
  } else {
    ThreeDimensionalCoordinates<T>::set(px, py, pz);
  }
  return *this;
}

template<typename T>
const T&
sd::core::VertexFromMeshView<T>::setX(const T& px) {
  set(px, this->y(), this->z());
  return px;
}

template<typename T>
const T&
sd::core::VertexFromMeshView<T>::setY(const T& py) {
  set(this->x(), py, this->z());
  return py;
}

template<typename T>
const T&
sd::core::VertexFromMeshView<T>::setZ(const T& pz) {
  set(this->x(), this->y(), pz);
  return pz;
}

template<typename T>
const sd::core::MeshView*
sd::core::VertexFromMeshView<T>::askingView() const {
  return m_askingView;
}

template<typename T>
const sd::core::MeshView*
sd::core::VertexFromMeshView<T>::containingView() const {
  return m_containingView;
}

template<typename T>
sd::core::MeshView::vertex_id
sd::core::VertexFromMeshView<T>::id() const {
  return m_id;
}

template<typename T>
unsigned int
sd::core::VertexFromMeshView<T>::globalIndex() const {
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->globalIndexOf(*this);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->globalIndexOf(*this);
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::getNeighborhood(std::vector<sd::core::VertexFromMeshView<T> >& neighbors) const {
  if (m_askingView->getFaceDegree() == 3) {
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getNeighborhood(*this, neighbors);
  } else {
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getNeighborhood(*this, neighbors);
  }
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::getNeighborhood(std::vector<std::vector<sd::core::VertexFromMeshView<T> > >& neighbors,
						 unsigned int nbRings) const {
  if (m_askingView->getFaceDegree() == 3) {
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getNeighborhood(*this, neighbors, nbRings);
  } else {
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getNeighborhood(*this, neighbors, nbRings);
  }
}

template<typename T>
template<unsigned int N>
void
sd::core::VertexFromMeshView<T>::getNeighboringFaces(std::vector<sd::core::FaceFromMeshView<T, N>* >& result) {
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  assert(m_askingView->getFaceDegree() == N);
  static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->getNeighboringFaces(*this, result);
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::getNeighboringEdges(std::vector<sd::core::EdgeFromMeshView<T>* >& result) {
  if (m_askingView->getFaceDegree() == 3) {
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getNeighboringEdges(*this, result);
  } else {
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getNeighboringEdges(*this, result);
  }
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isAttributeSet(const AttributeKey& akey) const {
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, akey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->isAttributeSet(*this, akey);
}

template<typename T>
template<typename A>
const A&
sd::core::VertexFromMeshView<T>::getAttribute(const AttributeKey& akey) const {
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getAttribute<A>(*this, akey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getAttribute<A>(*this, akey);
}

template<typename T>
template<typename A>
void
sd::core::VertexFromMeshView<T>::setAttribute(const AttributeKey& akey,
					      const A& val) {
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->setAttribute<A>(*this, akey, val);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->setAttribute<A>(*this, akey, val);
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isSelected() const {
  const sd::core::AttributeKey& selectedKey = sd::core::AttributeKey::getKey("Selected");
  if (m_askingView->getFaceDegree() == 3)
    return (static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, selectedKey)
	    && static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getAttribute<char>(*this, selectedKey));
  else
    return (static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, selectedKey)
	    && static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getAttribute<char>(*this, selectedKey));
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::setSelected(bool selected) {
  const sd::core::AttributeKey& selectedKey = sd::core::AttributeKey::getKey("Selected");
  if (m_askingView->getFaceDegree() == 3)
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->setAttribute<char>(*this, selectedKey, selected);
  else
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->setAttribute<char>(*this, selectedKey, selected);
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isNormalSet() const {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, normalKey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->isAttributeSet(*this, normalKey);
}

template<typename T>
sd::core::Normal<T>
sd::core::VertexFromMeshView<T>::getNormal() const {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3) {
    if ((static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, normalKey)))
      return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getAttribute<typename sd::core::Normal<T> >(*this, normalKey);
    else
      return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->computeVertexNormal(*this);
  } else {
    if(static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->isAttributeSet(*this, normalKey))
      return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getAttribute<typename sd::core::Normal<T> >(*this, normalKey);
    else
      return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->computeVertexNormal(*this);
  }
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::setNormal(const sd::core::Normal<T>& normal) {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3)
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->setAttribute<typename sd::core::Normal<T> >(*this, normalKey, normal);
  else
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->setAttribute<typename sd::core::Normal<T> >(*this, normalKey, normal);
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isRGBCoordinatesSet() const {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, colorKey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->isAttributeSet(*this, colorKey);
}

template<typename T>  
const sd::core::RGBCoordinates<unsigned char>&
sd::core::VertexFromMeshView<T>::getRGBCoordinates() const {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getAttribute<typename sd::core::RGBCoordinates<unsigned char> >(*this, colorKey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getAttribute<typename sd::core::RGBCoordinates<unsigned char> >(*this, colorKey);
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::setRGBCoordinates(const sd::core::RGBCoordinates<unsigned char>& rgb) {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3)
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->setAttribute<typename sd::core::RGBCoordinates<unsigned char> >(*this, colorKey, rgb);
  else
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->setAttribute<typename sd::core::RGBCoordinates<unsigned char> >(*this, colorKey, rgb);
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::isTextureCoordinatesSet() const {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->isAttributeSet(*this, textureKey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->isAttributeSet(*this, textureKey);
}

template<typename T>  
const sd::core::TextureCoordinates<double>&
sd::core::VertexFromMeshView<T>::getTextureCoordinates() const {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3)
    return static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->getAttribute<typename sd::core::TextureCoordinates<double> >(*this, textureKey);
  else
    return static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->getAttribute<typename sd::core::TextureCoordinates<double> >(*this, textureKey);
}

template<typename T>
void
sd::core::VertexFromMeshView<T>::setTextureCoordinates(const sd::core::TextureCoordinates<double>& uvw) {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  assert(m_askingView->getVertexDataType() == sd::core::DataType_<T>());
  if (m_askingView->getFaceDegree() == 3)
    static_cast<sd::core::MeshView_<T, 3>* >(m_askingView)->setAttribute<typename sd::core::TextureCoordinates<double> >(*this, textureKey, uvw);
  else
    static_cast<sd::core::MeshView_<T, 4>* >(m_askingView)->setAttribute<typename sd::core::TextureCoordinates<double> >(*this, textureKey, uvw);
}

template<typename T>
template<unsigned int N>
void
sd::core::VertexFromMeshView<T>::swapNeighborhood(sd::core::MeshView_<T, N>* askingView,
						  sd::core::VertexFromMeshView<T>& futur) {
#ifdef DEBUG
  assert((static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->importAttributes(futur, *this)));
  //assert(futur.template importAttributes<N>(*this));
#else
  static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->importAttributes(futur, *this);
  //futur.template importAttributes<N>(*this);
#endif
  
  {
    std::vector<sd::core::FaceFromMeshView<T, N>* > result;
    this->getNeighboringFaces(result);
    typename std::vector<sd::core::FaceFromMeshView<T, N>* >::iterator it = result.begin();
    const typename std::vector<sd::core::FaceFromMeshView<T, N>* >::iterator end = result.end();
    for (; it != end; ++it) {
      std::vector<VertexFromMeshView<T> > vs(N);
      for (unsigned int i=0; i<N; ++i) {
	if ((*it)->getAt(askingView, i) != *this) {
	  vs[i] = (*it)->getAt(askingView, i);
	} else {
	  vs[i] = futur;
	}
      }
      FaceFromMeshView<T, N>* face = askingView->addFace(vs);
      static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->importAttributes(*it, face);
      askingView->removeFace(*it);
    }
  }
  {
    std::vector<sd::core::EdgeFromMeshView<T>* > result;
    this->getNeighboringEdges(result);
    typename std::vector<sd::core::EdgeFromMeshView<T>* >::iterator it = result.begin();
    const typename std::vector<sd::core::EdgeFromMeshView<T>* >::iterator end = result.end();
    for (; it != end; ++it) {
      std::vector<VertexFromMeshView<T> > vs(2);
      for (unsigned int i=0; i<2; ++i) {
	if ((*it)->getAt(askingView, i) != *this) {
	  vs[i] = (*it)->getAt(askingView, i);
	} else {
	  vs[i] = futur;
	}
      }
      EdgeFromMeshView<T>* edge = askingView->addEdge(vs);
      static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->importAttributes(*it, edge);
      askingView->removeEdge(*it);
    }
  }   
}

template<typename T>
template<unsigned int N>
bool
sd::core::VertexFromMeshView<T>::importAttributes(const sd::core::VertexFromMeshView<T>& v) {
  return static_cast<sd::core::MeshView_<T, N>* >(m_askingView)->importAttributes(*this, v);
}

template<typename T>
const sd::core::VertexFromMeshView<T>&
sd::core::VertexFromMeshView<T>::operator=(const VertexFromMeshView<T>& v) {
  if (&v != this) {
    Coordinates<T>::nocopy(v);
    m_askingView = v.m_askingView;
    m_containingView = v.m_containingView;
    m_id = v.m_id;
  }
  return *this;
}

template<typename T>
sd::core::VertexFromMeshView<T>&
sd::core::VertexFromMeshView<T>::operator+=(const Vector3d<T>& v) {
  this->set(this->x()+v.x(),
	    this->y()+v.y(),
	    this->z()+v.z());
  return *this;
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::operator<(const sd::core::VertexFromMeshView<T>& v) const {
  return ((*this).containingView() < v.containingView())
    || ((*this).containingView() == v.containingView() && (*this).id() < v.id());
}

template<typename T>
bool
sd::core::VertexFromMeshView<T>::operator>(const sd::core::VertexFromMeshView<T>& v) const {
  return v < *this;
}
    
template<typename T>
bool
sd::core::operator== (const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2) {
  return v1.askingView() == v2.askingView() && v1.containingView() == v2.containingView() && v1.id() == v2.id();
}

template<typename T>
bool
sd::core::operator!= (const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2) {
  return !(v1 == v2);
}


#endif /*! SD_CORE_VERTEX_FROM_MESH_VIEW_HXX */
