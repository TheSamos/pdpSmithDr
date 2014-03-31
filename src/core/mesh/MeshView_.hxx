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

#ifndef SD_CORE_MESH_VIEW__HXX
#define SD_CORE_MESH_VIEW__HXX

#ifndef SD_CORE_VERTEX_FROM_MESH_VIEW_HPP
#include "VertexFromMeshView.hpp"
#endif /* ! SD_CORE_VERTEX_FROM_MESH_VIEW_HPP */

#ifndef SD_CORE_FACE_FROM_MESH_VIEW_HPP
#include "FaceFromMeshView.hpp"
#endif /* ! SD_CORE_FACE_FROM_MESH_VIEW_HPP */

#ifndef SD_CORE_EDGE_FROM_MESH_VIEW_HPP
#include "EdgeFromMeshView.hpp"
#endif /* ! SD_CORE_EDGE_FROM_MESH_VIEW_HPP */

#include <typeinfo>
#include <string.h>

#include <core/RGBCoordinates.hpp>

#include <utils/sdTypeUtils.hpp>

#ifdef DEBUG
#include <iostream> // DEBUG
#endif

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>::MeshView_(const std::string& name,
				     const sd::core::Vector<T>& vertices,
				     const sd::core::Vector<unsigned int>& facesIni,
				     const sd::core::Vector<unsigned int>& edgesIni,
				     const sd::core::Vector<typename sd::core::Normal<T>::type>& verticesNormalsData,
				     const sd::core::Vector<unsigned char>& verticesColorsData,
				     const sd::core::Vector<double>& verticesTexturesData)
  : MeshView(NULL, name),
    m_isMeshRoot(true),
    m_vertices(vertices),
    m_verticesIds(),
    m_verticesAttributes(),
    //m_verticesNormalsData(verticesNormals),
    //m_verticesColorsData(verticesColors),
    //m_verticesTexturesData(verticesTextures),
    m_faces(),
    m_facesAttributes(),
    m_edges(),
    m_edgesAttributes(),
    m_meshStruct(VF),
    m_bb(),
    m_bbIsUpdated(false),
    m_fusionDistance(-1) {
  
  const unsigned int normalCard = sd::core::Normal<T>::card();
  const unsigned int colorCard = sd::core::RGBCoordinates<unsigned char>::card();
  const unsigned int textureCard = sd::core::TextureCoordinates<double>::card();
  
  assert(verticesNormalsData.size()%normalCard == 0);
  assert(verticesNormalsData.size()/normalCard == m_vertices.size()/3 || verticesNormalsData.size() == 0);
  assert(verticesColorsData.size()%colorCard == 0);
  assert(verticesColorsData.size()/colorCard == m_vertices.size()/3 || verticesColorsData.size() == 0);
  assert(verticesTexturesData.size()%textureCard == 0);
  assert(verticesTexturesData.size()/textureCard == m_vertices.size()/3 || verticesTexturesData.size() == 0);
  
  // Fill Vertices
  m_verticesIds[this].push_back(std::pair<vertex_id, vertex_id>(0, m_vertices.size()/3));
  
  // Fill Vertices Normals
  sd::core::AttributeVect<typename sd::core::Normal<T> >* verticesNormalsAtt
     = new sd::core::AttributeVect<typename sd::core::Normal<T> >(verticesNormalsData.size()/normalCard);
  for (unsigned int i=0; i<verticesNormalsData.size(); i+=normalCard)
    //(*verticesNormalsAtt)[i/normalCard] = sd::core::Normal<T>(&m_verticesNormalsData, i);
    (*verticesNormalsAtt)[i/normalCard] = sd::core::Normal<T>(verticesNormalsData[i],
							      verticesNormalsData[i+1],
							      verticesNormalsData[i+2]);
  const AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  setVerticesAttribute(normalKey, this, verticesNormalsAtt);
  
  // Fill Vertices Colors
  sd::core::AttributeVect<sd::core::RGBCoordinates<unsigned char> >* verticesColorsAtt
    = new sd::core::AttributeVect<sd::core::RGBCoordinates<unsigned char> >(verticesColorsData.size()/colorCard);
  for (unsigned int i=0; i<verticesColorsData.size(); i+=colorCard)
    //(*verticesColorsAtt)[i/colorCard] = sd::core::RGBCoordinates<unsigned char>(&m_verticesColorsData, i);
    (*verticesColorsAtt)[i/colorCard] = sd::core::RGBCoordinates<unsigned char>(verticesColorsData[i],
										verticesColorsData[i+1],
										verticesColorsData[i+2],
										verticesColorsData[i+3]);
  const AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  setVerticesAttribute(colorKey, this, verticesColorsAtt);
  
  // Fill Vertices Textures
  sd::core::AttributeVect<sd::core::TextureCoordinates<double> >* verticesTexturesAtt
    = new sd::core::AttributeVect<sd::core::TextureCoordinates<double> >(verticesTexturesData.size()/textureCard);
  for (unsigned int i=0; i<verticesTexturesData.size(); i+=textureCard)
    //(*verticesTexturesAtt)[i/textureCard] = sd::core::TextureCoordinates<double>(&m_verticesTexturesData, i);
    (*verticesTexturesAtt)[i/textureCard] = sd::core::TextureCoordinates<double>(verticesTexturesData[i],
										 verticesTexturesData[i+1],
										 verticesTexturesData[i+2]);
  const AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  setVerticesAttribute(textureKey, this, verticesTexturesAtt);
  
  assert(isBranchRoot());
    
  // Fill faces
  m_faces = map_faces();
  m_facesAttributes = map_faces_attributes();
  if (vertices.size() != 0)
    (*m_faces)[this].resize(vertices.size()/3);
  FaceFromMeshView<T, N>* face = NULL;
  if (N == 3) {
    for (unsigned int i=0; i<facesIni.size(); i+=N) {
      assert(facesIni[i]<m_vertices.size());
      assert(facesIni[i+1]<m_vertices.size());
      assert(facesIni[i+2]<m_vertices.size());
      face = newFace<T>(this,
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i]),
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i+1]),
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i+2]));
      (*m_faces)[this][facesIni[i]].push_back(face);
      (*m_faces)[this][facesIni[i+1]].push_back(face);
      (*m_faces)[this][facesIni[i+2]].push_back(face);
    }
  } else {
    for (unsigned int i=0; i<facesIni.size(); i+=N) {
      assert(facesIni[i]<m_vertices.size());
      assert(facesIni[i+1]<m_vertices.size());
      assert(facesIni[i+2]<m_vertices.size());
      assert(facesIni[i+3]<m_vertices.size());
      face = newFace<T>(this,
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i]),
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i+1]),
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i+2]),
			sd::core::VertexFromMeshView<T>(this, this, facesIni[i+3]));
      (*m_faces)[this][facesIni[i]].push_back(face);
      (*m_faces)[this][facesIni[i+1]].push_back(face);
      (*m_faces)[this][facesIni[i+2]].push_back(face);
      (*m_faces)[this][facesIni[i+3]].push_back(face);
    }
  }
  
  // Fill edges
  m_edges = map_edges();
  if (vertices.size() != 0) // :TODO: remove this
    (*m_edges)[this].resize(vertices.size()/3);
  EdgeFromMeshView<T>* edge = NULL;
  for (unsigned int i=0; i<edgesIni.size(); i+=2) {
    assert(edgesIni[i]<m_vertices.size());
    assert(edgesIni[i+1]<m_vertices.size());
    edge = new EdgeFromMeshView<T>(this,
				   sd::core::VertexFromMeshView<T>(this, this, edgesIni[i]),
				   sd::core::VertexFromMeshView<T>(this, this, edgesIni[i+1]));
    (*m_edges)[this][edgesIni[i]].push_back(edge);
    (*m_edges)[this][edgesIni[i+1]].push_back(edge);
  }
}

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>::MeshView_(ViewNode* previousViewNode,
				     const std::string& name)
  : MeshView(previousViewNode, name),
    m_isMeshRoot(false),
    m_vertices(),
    m_verticesIds(),
    m_verticesAttributes(),
    //m_verticesNormalsData(),
    //m_verticesColorsData(),
    m_faces(),
    m_facesAttributes(),
    m_edges(),
    m_edgesAttributes(),
    m_meshStruct(VF),
    m_bb(),
    m_bbIsUpdated(false),
    m_fusionDistance() {
  init();
}

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>::MeshView_(const std::string& name)
  : MeshView(NULL, name),
    m_isMeshRoot(false),
    m_vertices(),
    m_verticesIds(),
    m_verticesAttributes(),
    //m_verticesNormalsData(),
    //m_verticesColorsData(),
    m_faces(),
    m_facesAttributes(),
    m_edges(),
    m_edgesAttributes(),
    m_meshStruct(VF),
    m_bb(),
    m_bbIsUpdated(false),
    m_fusionDistance() {
  init();
}

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>::MeshView_(const std::vector<ViewNode*>& previousViewNodes,
				     const std::string& name)
  : MeshView(previousViewNodes, name),
    m_isMeshRoot(false),
    m_vertices(),
    m_verticesIds(),
    m_verticesAttributes(),
    //m_verticesNormalsData(),
    //m_verticesColorsData(),
    m_faces(),
    m_facesAttributes(NULL),
    m_edges(),
    m_edgesAttributes(NULL),
    m_meshStruct(VF),
    m_bb(),
    m_bbIsUpdated(false),
    m_fusionDistance() {
  init();
}

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>::~MeshView_() {
#ifdef DEBUG
  std::cerr << "~MeshView_ : " << name() << " at " << this << std::endl;
#endif
  clear();
}

template<typename T, unsigned int N>   
sd::core::VertexDataType
sd::core::MeshView_<T, N>::getVertexDataType() const {
  return sd::core::DataType_<T>();
}
   
template<typename T, unsigned int N>   
unsigned int
sd::core::MeshView_<T, N>::getFaceDegree() const {
  return N;
}
/*
template<typename T, unsigned int N>
sd::core::RawMesh*
sd::core::MeshView_<T, N>::getRawMesh() {
  unsigned int nbOfVertices = this->nbVertices();
  unsigned int nbOfFaces = this->nbFaces();
  unsigned int nbOfEdges = this->nbEdges();
  
  float* nvertices = new float[nbOfVertices*3];
  unsigned int* nfaces = new unsigned int[nbOfFaces*N];
  unsigned int* nedges = new unsigned int[nbOfEdges*2];
  {
    vertices_const_iterator it = beginVertices();
    const vertices_const_iterator end = endVertices();
    for (float *p=nvertices; it != end; ++it, p+=3) {
      VertexFromMeshView<T> v = (*it);
      *p = (float)v.x();
      *(p+1) = (float)v.y();
      *(p+2) = (float)v.z();
    }
  }
  if (N == 3) {
    faces_const_iterator it = beginFaces();
    const faces_const_iterator end = endFaces();
    for (unsigned int *p=nfaces; it != end; ++it, p+=N) {
      FaceFromMeshView<T, N>* f = (*it);
      *p = globalIndexOf(f->getAt(this, 0));
      *(p+1) = globalIndexOf(f->getAt(this, 1));
      *(p+2) = globalIndexOf(f->getAt(this, 2));
    }
  } else { // == 4
    faces_const_iterator it = beginFaces();
    const faces_const_iterator end = endFaces();
    for (unsigned int *p=nfaces; it != end; ++it, p+=N) {
      FaceFromMeshView<T, N>* f = (*it);
      *p = globalIndexOf(f->getAt(this, 0));
      *(p+1) = globalIndexOf(f->getAt(this, 1));
      *(p+2) = globalIndexOf(f->getAt(this, 2));
      *(p+3) = globalIndexOf(f->getAt(this, 3));
    }
  }
  {
    edges_const_iterator it = beginEdges();
    const edges_const_iterator end = endEdges();
    for (unsigned int *p=nedges; it != end; ++it, p+=N) {
      EdgeFromMeshView<T>* e = (*it);
      *p = globalIndexOf(e->getAt(this, 0));
      *(p+1) = globalIndexOf(e->getAt(this, 1));
    }
  }
  return new RawMesh(nvertices, nfaces, nedges,
		     nbOfVertices, nbOfFaces, nbOfEdges,
		     NULL, NULL, NULL, NULL);
}
*/
template<typename T, unsigned int N>
void*
sd::core::MeshView_<T, N>::getVertexRawData() {
  return &m_vertices;
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::build() {
  init();
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setStructurationMode(MeshStructuration s) {
  m_meshStruct = s;
  // TODO
}

template<typename T, unsigned int N>
sd::core::VertexFromMeshView<T>
sd::core::MeshView_<T, N>::addVertex(const T& x, const T& y, const T& z) {
  MeshView::vertex_id newId = 0;
  if (m_verticesIds.find(this) != m_verticesIds.end())
    newId = m_verticesIds[this].back().second;
  unsigned int newIndex = getIndexFromVertexId(newId);
  if (getVertexIdFromIndex((MeshView::vertex_id)m_vertices.size()) <= newId) {
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
  } else {
    m_vertices[newIndex] = x;
    m_vertices[newIndex+1] = y;
    m_vertices[newIndex+2] = z;
  }
  signalVertexAdded(this, newId);
  return sd::core::VertexFromMeshView<T>(this, this, newId);
}

template<typename T, unsigned int N>
sd::core::VertexFromMeshView<T>
sd::core::MeshView_<T, N>::addVertex(const Point3d<T>& p) {
  return addVertex(p.x(), p.y(), p.z());
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::removeVertex(const VertexFromMeshView<T>& vertex) {
  MeshView::vertex_id vertexId = vertex.id();
  if (m_verticesIds.find((sd::core::MeshView*)vertex.containingView()) != m_verticesIds.end()) {
    list_vertices_id::iterator itIdx = m_verticesIds[(sd::core::MeshView*)vertex.containingView()].begin();
    list_vertices_id::iterator endIdx = m_verticesIds[(sd::core::MeshView*)vertex.containingView()].end();
    for (; itIdx != endIdx && ((*itIdx).second <= vertexId); ++itIdx) {}
    if ((*itIdx).first <= vertexId) {
      if ((*itIdx).first == vertexId && (*itIdx).second == vertexId+1)
	m_verticesIds[(sd::core::MeshView*)vertex.containingView()].erase(itIdx);
      else if ((*itIdx).first == vertexId)
	++((*itIdx).first);
      else if ((*itIdx).second == vertexId+1)
	--((*itIdx).second);
      else {
	vertex_id first = (*itIdx).first;
	(*itIdx).first = vertexId+1;
	m_verticesIds[(sd::core::MeshView*)vertex.containingView()].insert(itIdx, std::pair<vertex_id, vertex_id>(first, vertexId));
      }
      
      if ((*faces()).find((sd::core::MeshView*)vertex.containingView()) != (*faces()).end()
	  && (*faces())[(sd::core::MeshView*)vertex.containingView()].size() > vertexId) {
	typename std::vector<FaceFromMeshView<T, N>* >::iterator itFaces = (*faces())[(sd::core::MeshView*)vertex.containingView()][vertexId].begin();
	const typename std::vector<FaceFromMeshView<T, N>* >::iterator endFaces = (*faces())[(sd::core::MeshView*)vertex.containingView()][vertexId].end();
	for (; itFaces != endFaces; ++itFaces)
	  (*itFaces)->removeAt(this);
      }
      
      if ((*edges()).find((sd::core::MeshView*)vertex.containingView()) != (*edges()).end()
	  && (*edges())[(sd::core::MeshView*)vertex.containingView()].size() > vertexId) {
      typename std::vector<EdgeFromMeshView<T>* >::iterator itEdges = (*edges())[(sd::core::MeshView*)vertex.containingView()][vertexId].begin();
      const typename std::vector<EdgeFromMeshView<T>* >::iterator endEdges = (*edges())[(sd::core::MeshView*)vertex.containingView()][vertexId].end();
      for (; itEdges != endEdges; ++itEdges)
	(*itEdges)->removeAt(this);
      }
      
      typename std::vector<ViewNode*>::const_iterator it = getNextViewNodes().begin();
      const typename std::vector<ViewNode*>::const_iterator end = getNextViewNodes().end();
      for (; it != end; ++it) {
	if ((*it)->getSDType() == this->getSDType()
	    && static_cast<MeshView*>(*it)->getVertexDataType() == this->getVertexDataType()) {
	  if (static_cast<MeshView*>(*it)->getFaceDegree() == 3)
	    static_cast<MeshView_<T, 3>* >(*it)->removeVertex(vertex);
	  else
	    static_cast<MeshView_<T, 4>* >(*it)->removeVertex(vertex);
	}
      }
    }
  }
}

template<typename T, unsigned int N>
sd::core::EdgeFromMeshView<T>*
sd::core::MeshView_<T, N>::addEdge(std::vector<VertexFromMeshView<T> > vs) {
  assert(vs.size() == 2);
  return signalEdgeAdded(vs[0], vs[1], true);
}

template<typename T, unsigned int N>    
sd::core::EdgeFromMeshView<T>*
sd::core::MeshView_<T, N>::addEdge(const sd::core::VertexFromMeshView<T>& v1,
				   const sd::core::VertexFromMeshView<T>& v2) {
  return signalEdgeAdded(v1, v2, true);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::removeEdge(sd::core::EdgeFromMeshView<T>* edge) {
  edge->removeAt(this);
  if (edge->isDeletable()) {
    unlinkEdge(edge);
    delete edge;
  }
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::addFace(std::vector<VertexFromMeshView<T> > vs) {
  assert(vs.size() == N);
  if (N == 3)
    return addFace(vs[0], vs[1], vs[2]);
  else
    return addFace(vs[0], vs[1], vs[2], vs[3]);
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::addFace(const Point3d<T>& p1, const Point3d<T>& p2,
				   const Point3d<T>& p3) {
  return addFace(addVertex(p1), addVertex(p2), addVertex(p3));
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::addFace(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2,
				   const VertexFromMeshView<T>& v3) {
  assert(N == 3);
  return signalFaceAdded({v1, v2, v3}, true);
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::addFace(const Point3d<T>& p1, const Point3d<T>& p2,
				   const Point3d<T>& p3, const Point3d<T>& p4) {
  return addFace(addVertex(p1), addVertex(p2), addVertex(p3), addVertex(p4));
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::addFace(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2,
				   const VertexFromMeshView<T>& v3, const VertexFromMeshView<T>& v4) {
  assert(N == 4);
  return signalFaceAdded({v1, v2, v3, v4}, true);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::removeFace(sd::core::FaceFromMeshView<T, N>* face) {
  face->removeAt(this);
  if (face->isDeletable()) {
    unlinkFace(face);
    delete face;
  }
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setFusionDistance(float distance) {
  // :TODO:J:30/08/11
  assert(false || distance == -1.);
}

template<typename T, unsigned int N>
unsigned int
sd::core::MeshView_<T, N>::nbVertices() const {
  unsigned int nb = 0;
  map_vertices_id::const_iterator it = m_verticesIds.begin();
  const map_vertices_id::const_iterator end = m_verticesIds.end();
  for (; it != end; ++it) {
    list_vertices_id::const_iterator itL = (*it).second.begin();
    const list_vertices_id::const_iterator endL = (*it).second.end();
    for (; itL != endL; ++itL)
      nb += (*itL).second - (*itL).first;
  }
  return nb;
}

template<typename T, unsigned int N>
unsigned int
sd::core::MeshView_<T, N>::nbFaces() {
  unsigned int nb = 0;
  FaceFromMeshViewIterator<T, N> it = beginFaces();
  FaceFromMeshViewIterator<T, N> end = endFaces();
  for (; it != end; ++it)
    ++nb;
  return nb;
}

template<typename T, unsigned int N>
unsigned int
sd::core::MeshView_<T, N>::nbEdges() {
  unsigned int nb = 0;
  EdgeFromMeshViewIterator<T> it = beginEdges();
  EdgeFromMeshViewIterator<T> end = endEdges();
  for (; it != end; ++it)
    ++nb;
  return nb;
}

template<typename T, unsigned int N>
sd::core::VertexFromMeshView<T>
sd::core::MeshView_<T, N>::getVertexAt(unsigned int i) {
  assert(i<this->nbVertices());
  vertices_const_geometry_iterator it = beginVerticesConstGeometry();
    it+=i;
  return *it;
}
      
template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::getFaceAt(unsigned int i) {
  faces_const_iterator it = beginFaces();
  it+=i;
  return *it;
}
      
template<typename T, unsigned int N>
sd::core::EdgeFromMeshView<T>*
sd::core::MeshView_<T, N>::getEdgeAt(unsigned int i) {
  edges_const_iterator it = beginEdges();
  it+=i;
  return *it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator
sd::core::MeshView_<T, N>::beginVerticesConstGeometry() {
  sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it(this, m_verticesIds);
  it.setBegin();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator
sd::core::MeshView_<T, N>::endVerticesConstGeometry() {
  sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it(this, m_verticesIds);
  it.setEnd();
  return it;
}
      
template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::vertices_const_iterator
sd::core::MeshView_<T, N>::beginVertices() const {
  sd::core::MeshView_<T, N>::vertices_const_iterator it(this, m_verticesIds);
  it.setBegin();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::vertices_const_iterator
sd::core::MeshView_<T, N>::endVertices() const {
  sd::core::MeshView_<T, N>::vertices_const_iterator it(this, m_verticesIds);
  it.setEnd();
  return it;
}

template<typename T, unsigned int N>     
typename sd::core::MeshView_<T, N>::vertices_modifier_iterator
sd::core::MeshView_<T, N>::beginVerticesModifier() {
  sd::core::MeshView_<T, N>::vertices_modifier_iterator it(this, m_verticesIds);
  it.setBegin();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::vertices_modifier_iterator
sd::core::MeshView_<T, N>::endVerticesModifier() {
  sd::core::MeshView_<T, N>::vertices_modifier_iterator it(this, m_verticesIds);
  it.setEnd();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::faces_iterator
sd::core::MeshView_<T, N>::beginFaces() {
  sd::core::MeshView_<T, N>::faces_iterator it(this, faces());
  it.setBegin();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::faces_iterator
sd::core::MeshView_<T, N>::endFaces() {
  sd::core::MeshView_<T, N>::faces_iterator it(this, faces());
  it.setEnd();
  return it;
}

template<typename T, unsigned int N>   
typename sd::core::MeshView_<T, N>::edges_iterator
sd::core::MeshView_<T, N>::beginEdges() {
  sd::core::MeshView_<T, N>::edges_iterator it(this, edges());
  it.setBegin();
  return it;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::edges_iterator
sd::core::MeshView_<T, N>::endEdges() {
  sd::core::MeshView_<T, N>::edges_iterator it(this, edges());
  it.setEnd();
  return it;
}

template<typename T, unsigned int N>
const sd::core::BoundingBox<T>&
sd::core::MeshView_<T, N>::boundingBox() {
  if (!m_bbIsUpdated)
    updateBoundingBox();
  return m_bb;
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::updateBoundingBox() {
  m_bb = sd::core::BoundingBox<T>();
  vertices_const_iterator it = beginVertices();
  const vertices_const_iterator end = endVertices();
  for(; it != end; ++it) {
    m_bb.add(*it);
  }
  m_bbIsUpdated = true;
}

template<typename T, unsigned int N>  
void
sd::core::MeshView_<T, N>::getNeighborhood(const sd::core::VertexFromMeshView<T>& vertex,
					  std::vector<sd::core::VertexFromMeshView<T> >& result) {
  
  std::vector<sd::core::FaceFromMeshView<T, N>* > facesV;
  std::vector<sd::core::EdgeFromMeshView<T>* > edgesV;
  getNeighboringFaces(vertex, facesV);
  getNeighboringEdges(vertex, edgesV);
  
  { // COMMENT: May be optimize by using a temporary map...
    std::list<sd::core::VertexFromMeshView<T> > listRes;
    unsigned int idx=0;
    sd::core::VertexFromMeshView<T> v1, v2;
    typename std::list<sd::core::VertexFromMeshView<T> >::iterator f1, f2;
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator it = facesV.begin();
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator end = facesV.end();
    while (it != end) {
      idx = (**it).find(vertex);
      assert(idx < N);
      v1 = (**it).getAt(this, (idx+1)%N);
      v2 = (**it).getAt(this, (idx+2)%N);
      f1 = find(listRes.begin(), listRes.end(), v1);
      f2 = find(listRes.begin(), listRes.end(), v2);
      if (f1 == listRes.end() && f2 == listRes.end()) {
	if (listRes.size() == 0 || (facesV.size() != 0 && it+1 == end)) {
	  listRes.push_back(v1);
	  listRes.push_back(v2);
	  facesV.erase(it);
	  it = facesV.begin();
	  end = facesV.end();
	} else {
	  ++it;
	}
      } else if (f1 == listRes.end()) {
	listRes.insert(f2, v1);
	facesV.erase(it);
	it = facesV.begin();
	end = facesV.end();
      } else if (f2 == listRes.end()) {
	listRes.insert(++f1, v2);
	facesV.erase(it);
	it = facesV.begin();
	end = facesV.end();
      } else {
#ifdef DEBUG
	if (!(++f1 == f2 || (f1 == listRes.end() && f2 == listRes.begin()))) {
	  std::cerr << "Target : " << vertex.id() << " v1 : " << v1.id() << " v2 : " << v2.id() << std::endl;
	  typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator it = facesV.begin();
	  const typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator end = facesV.end();
	  for (; it!=end; ++it)
	    std::cerr << (*it)->getAt(this, 0).id() << ", "
		      << (*it)->getAt(this, 1).id() << ", "
		      << (*it)->getAt(this, 2).id() << std::endl;
      
	  typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator itL = listRes.begin();
	  typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator endL = listRes.end();
	  for (; itL != endL; ++itL)
	    std::cerr << (*itL).id() << ", ";
	  std::cerr << std::endl;
	  std::cerr << facesV.size() << std::endl;
	  assert(false && "Error neighborhood");
	}
#endif
	++it;
      }
    }
    typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator itL = listRes.begin();
    typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator endL = listRes.end();
    result.resize(listRes.size());
    for (unsigned int i=0; itL != endL; ++itL, ++i)
      result[i] = *itL;
  }
  /* // Try
  {
    std::list<sd::core::VertexFromMeshView<T> > listRes;
  
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator it = facesV.begin();
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator end = facesV.end();
    while (facesV.begin() != facesV.end()) {
      assert((**it).find(vertex) < N);
      sd::core::VertexFromMeshView<T> currentVertex = (**it).getAt(this, ((**it).find(vertex)+1)%N);
      listRes.push_back(currentVertex);
      facesV.erase(it);
      it = facesV.begin();
      end = facesV.end();
      for (; it != end; ++it) {
	if ((**it).find(currentVertex) < N) {
	  currentVertex = (**it).getAt(this, ((**it).find(currentVertex)+1)%N);
	  listRes.push_back(currentVertex);
	  facesV.erase(it);
	  it = facesV.begin();
	  end = facesV.end();
	}
      }
      it = facesV.begin();
      end = facesV.end();
      currentVertex = (**it).getAt(this, ((**it).find(vertex)+N-1)%N);
      listRes.push_front(currentVertex);
      facesV.erase(it);
      it = facesV.begin();
      end = facesV.end();
      for (; it != end; ++it) {
	if ((**it).find(currentVertex) < N) {
	  currentVertex = (**it).getAt(this, ((**it).find(currentVertex)+N-1)%N);
	  listRes.push_front(currentVertex);
	  facesV.erase(it);
	  it = facesV.begin();
	  end = facesV.end();
	}
      }
    }
    {
      typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator it = listRes.begin();
      typename std::list<sd::core::VertexFromMeshView<T> >::const_iterator end = listRes.end();
      result.resize(listRes.size());
      for (unsigned int i=0; it != end; ++it, ++i)
	result[i] = *it;
    }
  }
  */
  /*
  // Old bug with an uncomplet neighborhood
  {
    std::vector<sd::core::VertexFromMeshView<T> > result1;
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::const_iterator it1 = facesV.begin();
    const typename std::vector<sd::core::FaceFromMeshView<T, N>*>::const_iterator end1 = facesV.end();
    for (; it1 != end1; ++it1) {
      sd::core::VertexFromMeshView<T> currentVertex;
      for (unsigned int i=0; i<N; ++i) {
      
	if ((*it1)->getAt(this, i) == vertex &&
	    std::find(result1.begin(), result1.end(), (*it1)->getAt(this, (i+1)%N)) == result1.end()) {
	  currentVertex = (*it1)->getAt(this, (i+1)%N);
	  result1.push_back(currentVertex);
	
	  typename std::vector<sd::core::FaceFromMeshView<T, N>*>::const_iterator it1b = facesV.begin();
	  for (; it1b != end1; ++it1b) {
	    for (unsigned int i=0; i<N; ++i) {
	      if ((*it1b)->getAt(this, i) == currentVertex) {
		if((*it1b)->getAt(this, (i-1+N)%N) == vertex
		   && std::find(result1.begin(), result1.end(),
				(*it1b)->getAt(this, (i+1)%N)) == result1.end()) {
		  currentVertex = (*it1b)->getAt(this, (i+1)%N);
		  result1.push_back(currentVertex);
		  it1b = facesV.begin();
		} else if ((*it1b)->getAt(this, (i+1)%N) == vertex
			   && std::find(result1.begin(), result1.end(),
					(*it1b)->getAt(this, (i-1+N)%N)) == result1.end()) {
		  //assert(false);
		}
	      }
	    }
	  }
	
	}
      
      }
  
      }
  }
  */
  /*
#ifdef DEBUG
  std::vector<std::vector<sd::core::VertexFromMeshView<T> > > results;
  getNeighborhood(vertex, results, 1);
  std::vector<sd::core::VertexFromMeshView<T> >& result1 = results[0];
  
  bool noerror = (result.size() == result1.size());
  unsigned int j = std::find(result1.begin(), result1.end(), result[0])-result1.begin();
  for (unsigned int i=0; i<result.size(); ++i, ++j) {
    noerror &= (result[i] == result1[j%result1.size()]);
  }
    
  if (!noerror) {
    facesV.clear();
    getNeighboringFaces(vertex, facesV);
    std::cerr << "Target : " << vertex.id() << std::endl;
    typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator it = facesV.begin();
    const typename std::vector<sd::core::FaceFromMeshView<T, N>*>::iterator end = facesV.end();
    for (; it!=end; ++it)
      std::cerr << (*it)->getAt(this, 0).id() << ", "
		<< (*it)->getAt(this, 1).id() << ", "
		<< (*it)->getAt(this, 2).id() << std::endl;
    
    for (unsigned int i=0; i<result.size(); ++i)
      std::cerr << result[i].id() << ", ";
    std::cerr << std::endl;
    for (unsigned int i=0; i<result1.size(); ++i)
      std::cerr << result1[i].id() << ", ";
    std::cerr << std::endl;
    std::cerr << facesV.size() << std::endl;
    assert(false && "Error neighbors");
  }
#endif
  */
  
  {
    typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator it = edgesV.begin();
    const typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator end = edgesV.end();
    for (; it != end; ++it) {
      if (vertex == (*it)->first((MeshView_<T, N>*) vertex.askingView()))
	result.push_back((*it)->second((MeshView_<T, N>*) vertex.askingView()));
      else
	result.push_back((*it)->first((MeshView_<T, N>*) vertex.askingView()));
    }
  }
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::getNeighboringFaces(const sd::core::VertexFromMeshView<T>& vertex,
					       std::vector<sd::core::FaceFromMeshView<T, N>* >& result) {
  assert(faces());
  result.clear();
  if ((*faces()).find((MeshView*)vertex.containingView()) == (*faces()).end()
      || (*faces())[(MeshView*)vertex.containingView()].size() <= vertex.id())
    return;
  for (unsigned int i=0; i<(*faces())[(MeshView*)vertex.containingView()][vertex.id()].size(); ++i) {
    if ((*faces())[(MeshView*)vertex.containingView()][vertex.id()][i]->isValidAtView((MeshView*)vertex.askingView()))
      result.push_back((*faces())[(MeshView*)vertex.containingView()][vertex.id()][i]);
  }
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::getNeighboringFaces(const sd::core::VertexFromMeshView<T>& v1,
					       const sd::core::VertexFromMeshView<T>& v2,
					       std::vector<sd::core::FaceFromMeshView<T, N>* >& result) {
  assert(faces());
  result.clear();

  if ((*faces()).find((MeshView*)v1.containingView()) == (*faces()).end()
      || (*faces())[(MeshView*)v1.containingView()].size() <= v1.id()
      || (*faces()).find((MeshView*)v2.containingView()) == (*faces()).end()
      || (*faces())[(MeshView*)v2.containingView()].size() <= v2.id())
    return;
  
  std::map<sd::core::FaceFromMeshView<T, N>*,unsigned int> intersection;
  for (unsigned int i=0; i<(*faces())[(MeshView*)v1.containingView()][v1.id()].size(); ++i) {
    if ((*faces())[(MeshView*)v1.containingView()][v1.id()][i]->isValidAtView((MeshView*)v1.askingView()))    
      ++intersection[(*faces())[(MeshView*)v1.containingView()][v1.id()][i]];
  }
  for (unsigned int i=0; i<(*faces())[(MeshView*)v2.containingView()][v2.id()].size(); ++i) {
    if ((*faces())[(MeshView*)v2.containingView()][v2.id()][i]->isValidAtView((MeshView*)v2.askingView()))    
      ++intersection[(*faces())[(MeshView*)v2.containingView()][v2.id()][i]];
  }

  typename std::map<sd::core::FaceFromMeshView<T, N>*,unsigned int>::iterator it = intersection.begin();
  const typename std::map<sd::core::FaceFromMeshView<T, N>*,unsigned int>::iterator end = intersection.end();
  for (; it!=end; ++it) {
    if ((*it).second == 2)
      result.push_back((*it).first);
  }
    
  assert(result.size() < 3);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::getNeighboringEdges(const sd::core::VertexFromMeshView<T>& vertex,
					       std::vector<sd::core::EdgeFromMeshView<T>* >& result) {
  assert(edges());
  result.clear();
  if ((*edges()).find((MeshView*)vertex.containingView()) == (*edges()).end()
      || (*edges())[(MeshView*)vertex.containingView()].size() <= vertex.id())
    return;
  for (unsigned int i=0; i<(*edges())[(MeshView*)vertex.containingView()][vertex.id()].size(); ++i)
    if ((*edges())[(MeshView*)vertex.containingView()][vertex.id()][i]->isValidAtView((MeshView*)vertex.askingView()))
      result.push_back((*edges())[(MeshView*)vertex.containingView()][vertex.id()][i]);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::computeVerticesNormals() {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  vertices_const_iterator it = this->beginVertices();
  const vertices_const_iterator end = this->endVertices();
  for (; it != end; ++it)
    setAttribute<typename sd::core::Normal<T> >(*it,  normalKey, this->computeVertexNormal(*it));
}

template<typename T, unsigned int N>
std::set<sd::core::AttributeKey>
sd::core::MeshView_<T, N>::getVerticesAttributesKeys() const {
  const sd::core::VertexFromMeshView<T> vertex = (*beginVertices());
  return getVertexAttributesKeys(vertex);
}     

template<typename T, unsigned int N>
sd::core::AttributeType
sd::core::MeshView_<T, N>::getVerticesAttributeDataTypeAtKey(const AttributeKey& akey) const {
  const sd::core::VertexFromMeshView<T> vertex = (*beginVertices());
  return getVertexAttributeDataTypeAtKey(vertex, akey);
}

template<typename T, unsigned int N> 
bool
sd::core::MeshView_<T, N>::verticesAttributeContainsKey(const AttributeKey& akey) const {
  const sd::core::VertexFromMeshView<T> vertex = (*beginVertices());
  return isAttributeSet(vertex, akey);
}
 
template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeVerticesAttributeKey(const AttributeKey& akey) {
  assert(akey.id() != 0);
  if (m_verticesAttributes.find(akey.id()) != m_verticesAttributes.end()) {
    std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator it = m_verticesAttributes[akey.id()].begin();
    const std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator end = m_verticesAttributes[akey.id()].end();
    for (; it!=end; ++it) {
      delete (*it).second;
      (*it).second = NULL;
    }
    return true;
  }
  return false;
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeVerticesSelection() {
  return removeVerticesAttributeKey(sd::core::AttributeKey::getKey("Selected"));
}


template<typename T, unsigned int N>
std::set<sd::core::AttributeKey>
sd::core::MeshView_<T, N>::getFacesAttributesKeys() const {
  const sd::core::FaceFromMeshView<T, N>* face = (*beginFaces());
  assert(facesAttributes());
  return facesAttributes()->getAttributesKeys(face, this);
}     

template<typename T, unsigned int N>
sd::core::AttributeType
sd::core::MeshView_<T, N>::getFacesAttributeDataTypeAtKey(const AttributeKey& akey) const {
  const sd::core::FaceFromMeshView<T, N>* face = (*beginFaces());
  assert(facesAttributes());
  return facesAttributes()->getVertexAttributeDataTypeAtKey(face, akey, this);
}

template<typename T, unsigned int N> 
bool
sd::core::MeshView_<T, N>::facesAttributeContainsKey(const AttributeKey& akey) const {
  const sd::core::FaceFromMeshView<T, N>* face = (*beginFaces());
  assert(facesAttributes());
  return facesAttributes()->isAttributeSet(face, akey, this);
}
 
template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeFacesAttributeKey(const AttributeKey& akey) {
  assert(facesAttributes());
  return facesAttributes()->removeAttributeKey(akey, this);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeFacesSelection() {
  return facesAttributes()->removeAttributeKey(sd::core::AttributeKey::getKey("Selected"), this);
}


template<typename T, unsigned int N>
std::set<sd::core::AttributeKey>
sd::core::MeshView_<T, N>::getEdgesAttributesKeys() const {
  const sd::core::EdgeFromMeshView<T>* edge = (*beginEdges());
  assert(edgesAttributes());
  return edgesAttributes()->getAttributesKeys(edge, this);
}     

template<typename T, unsigned int N>
sd::core::AttributeType
sd::core::MeshView_<T, N>::getEdgesAttributeDataTypeAtKey(const AttributeKey& akey) const {
  const sd::core::EdgeFromMeshView<T>* edge = (*beginEdges());
  assert(edgesAttributes());
  return edgesAttributes()->getVertexAttributeDataTypeAtKey(edge, akey, this);
}

template<typename T, unsigned int N> 
bool
sd::core::MeshView_<T, N>::edgesAttributeContainsKey(const AttributeKey& akey) const {
  const sd::core::EdgeFromMeshView<T>* edge = (*beginEdges());
  assert(edgesAttributes());
  return edgesAttributes()->isAttributeSet(edge, akey, this);
}
 
template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeEdgesAttributeKey(const AttributeKey& akey) {
  assert(edgesAttributes());
  return edgesAttributes()->removeAttributeKey(akey, this);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::removeEdgesSelection() {
  return edgesAttributes()->removeAttributeKey(sd::core::AttributeKey::getKey("Selected"), this);
}


template<typename T, unsigned int N>
sd::core::Normal<T>
sd::core::MeshView_<T, N>::computeVertexNormal(const sd::core::VertexFromMeshView<T>& vertex) {
  sd::core::Normal<T> normal(0., 0., 0.);
  std::vector<sd::core::FaceFromMeshView<T, N>* > result;
  this->getNeighboringFaces(vertex, result);
  typename std::vector<sd::core::FaceFromMeshView<T, N>* >::iterator itF = result.begin();
  const typename std::vector<sd::core::FaceFromMeshView<T, N>* >::iterator endF = result.end();
  for (; itF != endF; ++itF) {
    normal += (*itF)->getNormal(this);
  }
  normal.normalize();
  return normal;
}

// PRIVATE //

inline
std::vector<sd::core::MeshView*>
filterNonSupportedSDType(const std::vector<sd::core::ViewNode*>& input, const sd::ObjectType& sdType) {
  std::vector<sd::core::MeshView*> output;
  for (unsigned int i=0; i<input.size(); ++i)
    if (input[i]->getSDType() == sdType)
      output.push_back((sd::core::MeshView*)input[i]);
  return output;
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::init() {
  
  clear();
  
  std::vector<MeshView*> supportedParents = filterNonSupportedSDType(getPreviousViewNodes(), (*this).getSDType());
  
  if (supportedParents.size() == 1) {
    if (supportedParents[0]->getFaceDegree() == N) {
      sd::core::MeshView_<T, N>* previousViewNode = static_cast<sd::core::MeshView_<T, N>*>(supportedParents[0]);
      m_verticesIds = previousViewNode->m_verticesIds;
      m_faces = previousViewNode->m_faces;
      m_facesAttributes = previousViewNode->m_facesAttributes;
      m_edges = previousViewNode->m_edges;
      m_edgesAttributes = previousViewNode->m_edgesAttributes;
      m_bb = previousViewNode->m_bb;
      m_bbIsUpdated = previousViewNode->m_bbIsUpdated;
      m_fusionDistance = previousViewNode->m_fusionDistance;
    } else {
      sd::core::MeshView_<T, (1-(N-3)+3)>* previousViewNode = static_cast<sd::core::MeshView_<T, (1-(N-3)+3)>*>(supportedParents[0]);
      m_verticesIds = previousViewNode->m_verticesIds;
      m_bb = previousViewNode->m_bb;
      m_bbIsUpdated = previousViewNode->m_bbIsUpdated;
      m_fusionDistance = previousViewNode->m_fusionDistance;
      // TODO : Convert faces & edges
    }
  } else {
    for (unsigned int i=0; i<supportedParents.size(); ++i) {
      if (supportedParents[i]->getFaceDegree() == this->getFaceDegree()) {
	sd::core::MeshView_<T, N>* previousViewNode = static_cast<sd::core::MeshView_<T, N>*>(supportedParents[i]);
	if (i == 0) {
	  m_fusionDistance = previousViewNode->m_fusionDistance;
	  m_verticesIds = previousViewNode->m_verticesIds;
	} else {
	  m_fusionDistance = std::max(m_fusionDistance, previousViewNode->m_fusionDistance);
	  map_vertices_id::const_iterator itM = previousViewNode->m_verticesIds.begin();
	  const map_vertices_id::const_iterator endM = previousViewNode->m_verticesIds.end();
	  for (; itM != endM; ++itM) {
	    const map_vertices_id::const_iterator itFind = m_verticesIds.find((*itM).first);
	    if (itFind != m_verticesIds.end()) {
	      list_vertices_id newList;
	      list_vertices_id::const_iterator it1 = (*itM).second.begin();
	      const list_vertices_id::const_iterator end1 = (*itM).second.end();
	      list_vertices_id::const_iterator it2 = (*itFind).second.begin();
	      const list_vertices_id::const_iterator end2 = (*itFind).second.end();
	      vertex_id start = std::min((*it1).first, (*it2).first);
	      vertex_id end = std::max((*it1).second, (*it2).second);
	      while (it1 != end1 && it2 != end2) {
		bool changed = false;
		while (it1 != end1 && (*it1).second <= start)
		  ++it1;
		while (it2 != end2 && (*it2).second <= start)
		  ++it2;
		if (start > (*it1).first || end < (*it1).second) {
		  start = std::min(start, (*it1).first);
		  end = std::min(end, (*it1).second);
		  ++it1;
		  changed = true;
		}
		if (start > (*it2).first || end < (*it2).second) {
		  start = std::min(start, (*it2).first);
		  end = std::min(end, (*it2).second);
		  ++it2;
		  changed = true;
		}
		if (!changed) {
		  newList.push_back(std::pair<vertex_id, vertex_id>(start, end));
		  start = std::min((*it1).first, (*it2).first);
		  end = std::max((*it1).second, (*it2).second);
		}
	      }
	      for (; it1 != end1; ++it1)
		newList.push_back(*it1);
	      for (; it2 != end2; ++it2)
		newList.push_back(*it2);
	      m_verticesIds[(*itM).first] = newList;
	    } else {
	      m_verticesIds[(*itM).first] = (*itM).second;
	    }
	  }
	}
      }
    }
  }
  
  if (supportedParents.size() != 1) {
    std::vector<MeshView*> collectedMV(0);
    m_isMeshRoot = true;
    m_faces = map_faces();
    m_edges = map_edges();
    m_facesAttributes = map_faces_attributes();
    m_edgesAttributes = map_edges_attributes();
    for (unsigned int i=0; i<supportedParents.size(); ++i) {
      if (supportedParents[i]->getFaceDegree() == this->getFaceDegree()) {
	faces_const_iterator itF = static_cast<MeshView_<T, N>*>(supportedParents[i])->beginFaces();
	const faces_const_iterator endF = static_cast<MeshView_<T, N>*>(supportedParents[i])->endFaces();
	for (; itF != endF; ++itF) {
	  bool isNew = true;
	  for (unsigned int m=0; m<collectedMV.size(); ++m) {
	    isNew &= !((*itF)->isValidAtView(collectedMV[m]));
	  }
	  assert(isNew); // debug
	  if (isNew) {
	    if (N == 3)
	      addFace((*itF)->getAt((MeshView*)this, 0), (*itF)->getAt((MeshView*)this, 1),
		      (*itF)->getAt((MeshView*)this, 2));
	    else
	      addFace((*itF)->getAt((MeshView*)this, 0), (*itF)->getAt((MeshView*)this, 1),
		      (*itF)->getAt((MeshView*)this, 2), (*itF)->getAt((MeshView*)this, 3));
	  }
	}
      }
      if (supportedParents[i]->getFaceDegree() == 3) {
	typename MeshView_<T, 3>::edges_const_iterator itE = static_cast<MeshView_<T, 3>*>(supportedParents[i])->beginEdges();
	const typename MeshView_<T, 3>::edges_const_iterator endE = static_cast<MeshView_<T, 3>*>(supportedParents[i])->endEdges();
	for (; itE != endE; ++itE) {
	  addEdge((*itE)->getAt((MeshView*)this, 0), (*itE)->getAt((MeshView*)this, 1));
	}
      } else {
	typename MeshView_<T, 4>::edges_const_iterator itE = static_cast<MeshView_<T, 4>*>(supportedParents[i])->beginEdges();
	const typename MeshView_<T, 4>::edges_const_iterator endE = static_cast<MeshView_<T, 4>*>(supportedParents[i])->endEdges();
	for (; itE != endE; ++itE) {
	  bool isNew = true;
	  for (unsigned int m=0; m<collectedMV.size(); ++m) {
	    isNew &= !((*itE)->isValidAtView(collectedMV[m]));
	  }
	  if (isNew)
	    addEdge((*itE)->getAt((MeshView*)this, 0), (*itE)->getAt((MeshView*)this, 1));
	}
      }
      collectedMV.push_back(static_cast<MeshView*>(supportedParents[i]));
    }
  }
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::clear() {
  m_vertices.clear();
  m_verticesIds.clear();
  
  if (facesAttributes()->data())
    facesAttributes()->removeAttributes(this);
  if (edgesAttributes()->data())
    edgesAttributes()->removeAttributes(this);
  
  m_bb = BoundingBox<T>();
  m_bbIsUpdated = false;
  m_fusionDistance = 0.;
  
  if (faces()) {
    {
      std::set<FaceFromMeshView<T, N>* > tmp1;
      typename std::vector<std::vector<FaceFromMeshView<T, N>* > >::iterator it = (*faces())[this].begin();
      const typename std::vector<std::vector<FaceFromMeshView<T, N>* > >::iterator end = (*faces())[this].end();
      for (; it!=end; ++it) {
	typename std::vector<FaceFromMeshView<T, N>* >::iterator itP = (*it).begin();
	const typename std::vector<FaceFromMeshView<T, N>* >::iterator endP = (*it).end();
	for (; itP!=endP; ++itP) {
	  tmp1.insert(*itP);
	  (*itP)->isNoMore(this);
	  *itP = NULL;
	}
      }
      typename std::set<FaceFromMeshView<T, N>* >::iterator it1 = tmp1.begin();
      const typename std::set<FaceFromMeshView<T, N>* >::iterator end1 = tmp1.end();
      for (; it1 != end1; ++it1)
	delete *it1;
      tmp1.clear();
      
      (*faces()).erase((*faces()).find(this));
    }
    {
      typename map_faces::iterator it0 = (*faces()).begin();
      const typename map_faces::iterator end0 = (*faces()).end();
      for (; it0!=end0; ++it0) {
	typename std::vector<std::vector<FaceFromMeshView<T, N>* > >::iterator it1 = (*it0).second.begin();
	const typename std::vector<std::vector<FaceFromMeshView<T, N>* > >::iterator end1 = (*it0).second.end();
	for (; it1!=end1; ++it1) {
	  typename std::vector<FaceFromMeshView<T, N>* >::iterator it2 = (*it1).begin();
	  const typename std::vector<FaceFromMeshView<T, N>* >::iterator end2 = (*it1).end();
	  for (; it2!=end2; ++it2) {
	    (*it2)->isNoMore(this);
	  }
	}
      }
    }
  }
  
  if (edges()) {
    {
      std::set<EdgeFromMeshView<T>* > tmp1;
      typename std::vector<std::vector<EdgeFromMeshView<T>* > >::iterator it = (*edges())[this].begin();
      const typename std::vector<std::vector<EdgeFromMeshView<T>* > >::iterator end = (*edges())[this].end();
      for (; it!=end; ++it) {
	typename std::vector<EdgeFromMeshView<T>* >::iterator itP = (*it).begin();
	const typename std::vector<EdgeFromMeshView<T>* >::iterator endP = (*it).end();
	for (; itP!=endP; ++itP) {
	  tmp1.insert(*itP);
	  *itP = NULL;
	}
      }
      typename std::set<EdgeFromMeshView<T>* >::iterator it1 = tmp1.begin();
      const typename std::set<EdgeFromMeshView<T>* >::iterator end1 = tmp1.end();
      for (; it1 != end1; ++it1)
	delete *it1;
      tmp1.clear();
  
      (*edges()).erase((*edges()).find(this));
    }
    {
      typename map_edges::iterator it0 = (*edges()).begin();
      const typename map_edges::iterator end0 = (*edges()).end();
      for (; it0!=end0; ++it0) {
	typename std::vector<std::vector<EdgeFromMeshView<T>* > >::iterator it1 = (*it0).second.begin();
	const typename std::vector<std::vector<EdgeFromMeshView<T>* > >::iterator end1 = (*it0).second.end();
	for (; it1!=end1; ++it1) {
	  typename std::vector<EdgeFromMeshView<T>* >::iterator it2 = (*it1).begin();
	  const typename std::vector<EdgeFromMeshView<T>* >::iterator end2 = (*it1).end();
	  for (; it2!=end2; ++it2) {
	    (*it2)->isNoMore(this);
	  }
	}
      }
    }
  }
  
  if (/*m_isMeshRoot && m_faces*/ m_faces.isTheLast() && faces()) {
    // :TRICKY: Invalidate faces
    std::vector<FaceFromMeshView<T, N>* > tmp1;
    faces_iterator itF = beginFaces();
    const faces_iterator endF = endFaces();
    for (; itF != endF; ++itF) {
      tmp1.push_back(*itF);
      *itF = NULL;
    }
    typename std::vector<FaceFromMeshView<T, N>* >::iterator it1 = tmp1.begin();
    const typename std::vector<FaceFromMeshView<T, N>* >::iterator end1 = tmp1.end();
    for (; it1 != end1; ++it1)
      delete *it1;
    tmp1.clear();
  }
  
  if (/*m_isMeshRoot && m_faces*/ m_edges.isTheLast() && edges()) {
    // :TRICKY: Invalidate edges     
    std::vector<EdgeFromMeshView<T>* > tmp2;
    edges_iterator itE = beginEdges();
    const edges_iterator endE = endEdges();
    for (; itE != endE; ++itE) {
      tmp2.push_back(*itE);
      *itE = NULL;
    }
    typename std::vector<EdgeFromMeshView<T>* >::iterator it2 = tmp2.begin();
    const typename std::vector<EdgeFromMeshView<T>* >::iterator end2 = tmp2.end();
    for (; it2 != end2; ++it2)
      delete *it2;
    tmp2.clear();
  }
  
  map_vertices_attributes::iterator itVect0 = m_verticesAttributes.begin();
  const map_vertices_attributes::iterator endVect0 = m_verticesAttributes.end();
  for (; itVect0 != endVect0; ++itVect0) {
    std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator itVect1 = (*itVect0).second.begin();
    std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator endVect1 = (*itVect0).second.end();
    for (; itVect1 != endVect1; ++itVect1) {
      delete (*itVect1).second;
    }
  }
  
  m_verticesAttributes.clear();  
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::unlinkFace(sd::core::FaceFromMeshView<T, N>* face) {
  sd::utils::remove((*faces())[(MeshView*)face->getAt(this, 0).containingView()][face->getAt(this, 0).id()], face);
  sd::utils::remove((*faces())[(MeshView*)face->getAt(this, 1).containingView()][face->getAt(this, 1).id()], face);
  sd::utils::remove((*faces())[(MeshView*)face->getAt(this, 2).containingView()][face->getAt(this, 2).id()], face);
  if (N == 4)
    sd::utils::remove((*faces())[(MeshView*)face->getAt(this, 3).containingView()][face->getAt(this, 3).id()], face);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::unlinkEdge(sd::core::EdgeFromMeshView<T>* edge) {
  sd::utils::remove((*edges())[(MeshView*)edge->getAt(this, 0).containingView()][edge->getAt(this, 0).id()], edge);
  sd::utils::remove((*edges())[(MeshView*)edge->getAt(this, 1).containingView()][edge->getAt(this, 1).id()], edge);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::signalVertexAdded(MeshView* containingView, const vertex_id& vertexId) {
  if ((*faces())[containingView].size() <= vertexId) {
    (*faces())[containingView].resize(vertexId+1, std::vector<sd::core::FaceFromMeshView<T, N>* >());
  }
  
  if ((*edges())[containingView].size() <= vertexId) {
    (*edges())[containingView].resize(vertexId+1, std::vector<sd::core::EdgeFromMeshView<T>* >());
  }
  
  sd::core::MeshView::vertex_id lastId = 0;
  if (m_verticesIds.find(containingView) != m_verticesIds.end()
      &&  m_verticesIds[containingView].size() > 0)
    lastId = m_verticesIds[containingView].back().second;

  if (m_verticesIds[containingView].size() > 0 && lastId == vertexId)
    ++(m_verticesIds[containingView].back().second);
  else
    m_verticesIds[containingView].push_back(std::pair<vertex_id, vertex_id>(vertexId, vertexId+1));
  
  assert(m_verticesIds[containingView].size() > 0);
  
  std::vector<ViewNode*>::const_iterator it = getNextViewNodes().begin();
  const std::vector<ViewNode*>::const_iterator end = getNextViewNodes().end();
  for (; it != end; ++it) {
    if ((*it)->getSDType() == this->getSDType()
	&& static_cast<MeshView*>(*it)->getVertexDataType() == this->getVertexDataType()) {
      if (static_cast<MeshView*>(*it)->getFaceDegree() == 3)
	static_cast<MeshView_<T, 3>* >(*it)->signalVertexAdded(containingView, vertexId);
      else
	static_cast<MeshView_<T, 4>* >(*it)->signalVertexAdded(containingView, vertexId);
    }
  }
}

template<typename T, unsigned int N>
sd::core::FaceFromMeshView<T, N>*
sd::core::MeshView_<T, N>::signalFaceAdded(const std::vector<VertexFromMeshView<T> > f, bool isFirstCall) {
  FaceFromMeshView<T, N>* face = NULL;
  if (isFirstCall || m_isMeshRoot) {
    for (unsigned int i=0; i<f.size(); ++i)
      if (f[i].id() >= (*faces())[(MeshView*)f[i].containingView()].size())
	(*faces())[(MeshView*)f[i].containingView()].resize(f[i].id()+1,
							    std::vector<sd::core::FaceFromMeshView<T, N>*>());
    
     //check if face is already created
    std::map<sd::core::FaceFromMeshView<T, N>*, unsigned int> intersection;
    for (unsigned int i=0; i<f.size(); ++i) {
      typename std::vector<sd::core::FaceFromMeshView<T, N>*>::const_iterator it = (*faces())[(MeshView*)f[i].containingView()][f[i].id()].begin();
      const typename std::vector<sd::core::FaceFromMeshView<T, N>*>::const_iterator end = (*faces())[(MeshView*)f[i].containingView()][f[i].id()].end();
      for (; it!=end; ++it) {
	++intersection[*it];
	if (intersection[*it] == N) {
	  return *it;
	}
      }
    }
    
    if (N == 3)
      face = newFace(this, f[0], f[1], f[2]);
    else
      face = newFace(this, f[0], f[1], f[2], f[3]);
    
    for (unsigned int i=0; i<f.size(); ++i)
      (*faces())[(MeshView*)f[i].containingView()][f[i].id()].push_back(face);
  }
  std::vector<ViewNode*>::const_iterator it = getNextViewNodes().begin();
  const std::vector<ViewNode*>::const_iterator end = getNextViewNodes().end();
  for (; it != end; ++it) {
    if ((*it)->getSDType() == this->getSDType()
	&& static_cast<MeshView*>(*it)->getVertexDataType() == this->getVertexDataType()
	&& static_cast<MeshView*>(*it)->getFaceDegree() == this->getFaceDegree())
      static_cast<MeshView_<T, N>* >(*it)->signalFaceAdded(f, false);
  }
  return face;
}

template<typename T, unsigned int N>
sd::core::EdgeFromMeshView<T>*
sd::core::MeshView_<T, N>::signalEdgeAdded(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2,
					   bool isFirstCall) {
  assert(v1 != v2);
  EdgeFromMeshView<T>* edge = NULL;
  if (isFirstCall || m_isMeshRoot) {
    if (v1.id() >= (*edges())[(MeshView*)v1.containingView()].size())
      (*edges())[(MeshView*)v1.containingView()].resize(v1.id()+1,
							std::vector<sd::core::EdgeFromMeshView<T>*>());
    if (v2.id() >= (*edges())[(MeshView*)v2.containingView()].size())
      (*edges())[(MeshView*)v2.containingView()].resize(v2.id()+1,
							std::vector<sd::core::EdgeFromMeshView<T>*>());
    
    std::map<sd::core::EdgeFromMeshView<T>*,unsigned int> intersection;
    {
      typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator it = (*edges())[(MeshView*)v1.containingView()][v1.id()].begin();
      const typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator end = (*edges())[(MeshView*)v1.containingView()][v1.id()].end();
      for (; it!=end; ++it) {
	++intersection[*it];
	if (intersection[*it] == 2)
	  return NULL;
      }
    }
    {
      typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator it = (*edges())[(MeshView*)v2.containingView()][v2.id()].begin();
      const typename std::vector<sd::core::EdgeFromMeshView<T>*>::const_iterator end = (*edges())[(MeshView*)v2.containingView()][v2.id()].end();
      for (; it!=end; ++it) {
	++intersection[*it];
	if (intersection[*it] == 2)
	  return NULL;
      }
    }
    
    edge = new EdgeFromMeshView<T>(this, v1, v2);
    (*edges())[(MeshView*)v1.containingView()][v1.id()].push_back(edge);
    (*edges())[(MeshView*)v2.containingView()][v2.id()].push_back(edge);
  }
  std::vector<ViewNode*>::const_iterator it = getNextViewNodes().begin();
  const std::vector<ViewNode*>::const_iterator end = getNextViewNodes().end();
  for (; it != end; ++it) {
    static_cast<MeshView_<T, N>* >(*it)->signalEdgeAdded(v1, v2, false);
  }
  return edge;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::map_faces*
sd::core::MeshView_<T, N>::faces() {
  // return (/*m_isMeshRoot*/m_faces != NULL) ? m_faces : ((askableParent()) ? askableParent()->faces() : NULL);
  return m_faces.data();
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::map_faces_attributes*
sd::core::MeshView_<T, N>::facesAttributes() {
  // return (/*m_isMeshRoot*/m_facesAttributes != NULL) ? m_facesAttributes : ((askableParent()) ? askableParent()->facesAttributes() : NULL);
  return &m_facesAttributes;
}

template<typename T, unsigned int N>
const typename sd::core::MeshView_<T, N>::map_faces_attributes*
sd::core::MeshView_<T, N>::facesAttributes() const {
  // return (/*m_isMeshRoot*/m_facesAttributes != NULL) ? m_facesAttributes : ((askableParent()) ? askableParent()->facesAttributes() : NULL);
  return &m_facesAttributes;
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::map_edges*
sd::core::MeshView_<T, N>::edges() {
  // return (/*m_isMeshRoot*/m_edges != NULL) ? m_edges : ((askableParent()) ? askableParent()->edges() : NULL);
  return m_edges.data();
}

template<typename T, unsigned int N>
typename sd::core::MeshView_<T, N>::map_edges_attributes*
sd::core::MeshView_<T, N>::edgesAttributes() {
  // return (/*m_isMeshRoot*/m_edgesAttributes != NULL) ? m_edgesAttributes : ((askableParent()) ? askableParent()->edgesAttributes() : NULL);
  return &m_edgesAttributes;
}

template<typename T, unsigned int N>
const typename sd::core::MeshView_<T, N>::map_edges_attributes*
sd::core::MeshView_<T, N>::edgesAttributes() const {
  // return (/*m_isMeshRoot*/m_edgesAttributes != NULL) ? m_edgesAttributes : ((askableParent()) ? askableParent()->edgesAttributes() : NULL);
  return &m_edgesAttributes;
}

template<typename T, unsigned int N>
unsigned int
sd::core::MeshView_<T, N>::globalIndexOf(const VertexFromMeshView<T>& vertex) {
  MeshView::vertex_id gIndex = 0;
  std::map<MeshView*,
	   std::list<std::pair<MeshView::vertex_id,
			       MeshView::vertex_id> > >::const_iterator mapIt = m_verticesIds.begin();
  const std::map<MeshView*,
		 std::list<std::pair<MeshView::vertex_id,
				     MeshView::vertex_id> > >::const_iterator mapEnd = m_verticesIds.end();
  
  const MeshView* containingView = vertex.containingView();
  MeshView::vertex_id id = vertex.id();
  
  for (; mapIt != mapEnd; ++mapIt) {
    if (containingView != (*mapIt).first) {
      std::list<std::pair<MeshView::vertex_id,
			  MeshView::vertex_id> >::const_iterator intervalIt = (*mapIt).second.begin();
      const std::list<std::pair<MeshView::vertex_id,
				MeshView::vertex_id> >::const_iterator intervalEnd = (*mapIt).second.end();
      for (; intervalIt != intervalEnd; ++intervalIt) {
	gIndex += (*intervalIt).second - (*intervalIt).first;
      }
    } else {
      std::list<std::pair<MeshView::vertex_id,
			  MeshView::vertex_id> >::const_iterator intervalIt = (*mapIt).second.begin();
      const std::list<std::pair<MeshView::vertex_id,
				MeshView::vertex_id> >::const_iterator intervalEnd = (*mapIt).second.end();
      for (; intervalIt != intervalEnd; ++intervalIt) {
	if ((*intervalIt).second > id && (*intervalIt).first <= id) {
	  gIndex += (id - (*intervalIt).first);
	  return gIndex;
	} else {
	  gIndex += (*intervalIt).second - (*intervalIt).first;
	}
      }
    }
  }
  return gIndex;
 }

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>*
sd::core::MeshView_<T, N>::askableParent() {
  std::vector<sd::core::MeshView*> supportedParents = filterNonSupportedSDType(getPreviousViewNodes(),
									       (*this).getSDType());
  
  return (supportedParents.size() == 1
	  && supportedParents[0]->getVertexDataType() == this->getVertexDataType()
	  && supportedParents[0]->getFaceDegree() == this->getFaceDegree())
    ? ((sd::core::MeshView_<T, N>*)supportedParents[0]) : NULL;
}

template<typename T, unsigned int N>
const sd::core::MeshView_<T, N>*
sd::core::MeshView_<T, N>::askableParent() const {
  std::vector<sd::core::MeshView*> supportedParents = filterNonSupportedSDType(getPreviousViewNodes(),
									       (*this).getSDType());
  return (supportedParents.size() == 1
	  && supportedParents[0]->getVertexDataType() == this->getVertexDataType()
	  && supportedParents[0]->getFaceDegree() == this->getFaceDegree())
    ? static_cast<MeshView_<T, N>* >(supportedParents[0]) : NULL;
}

// +++================== Vertices Attributes Management ==================+++ {

template<typename T, unsigned int N>
std::map<sd::core::MeshView*, sd::core::AttributeCollection<unsigned int>*>&
sd::core::MeshView_<T, N>::getVerticesAttribute(const sd::core::AttributeKey& akey) {
  assert(akey.id() != 0);
  if (m_verticesAttributes.find(akey.id()) != m_verticesAttributes.end())
    return m_verticesAttributes.find(akey.id())->second;
  else if (!askableParent())
    assert(false && "Attribute is not defined !!!");
  return askableParent()->getVerticesAttribute(akey);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setVerticesAttribute(const sd::core::AttributeKey& akey,
						sd::core::MeshView* mv,
						sd::core::AttributeCollection<unsigned int>* attribute) {
  assert(akey.id() != 0);
  delete m_verticesAttributes[akey.id()][mv];
  m_verticesAttributes[akey.id()][mv] = attribute;
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::importParentVerticesAttribute(const sd::core::AttributeKey& akey) {
  assert(askableParent());
  {
    std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator it = m_verticesAttributes[akey.id()].begin();
    const std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator end = m_verticesAttributes[akey.id()].end();
    for (; it != end; ++it)
      delete (*it).second;
  }
  {
    m_verticesAttributes[akey.id()] = askableParent()->getVerticesAttribute(akey);
    std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator it = m_verticesAttributes[akey.id()].begin();
    const std::map<MeshView*, AttributeCollection<unsigned int>* >::iterator end = m_verticesAttributes[akey.id()].end();
    for (; it != end; ++it)
      (*it).second = (*it).second->clone();
  }
}

template<typename T, unsigned int N>
std::set<sd::core::AttributeKey>
sd::core::MeshView_<T, N>::getVertexAttributesKeys(const sd::core::VertexFromMeshView<T>& vertex) const {
  sd::core::MeshView* contView = (sd::core::MeshView*) vertex.containingView();
  assert(*contView <= *this);
  std::set<AttributeKey> result;
  if (askableParent() != NULL
      && *contView < *this) {
    result = ((sd::core::MeshView_<T, N>*)askableParent())->getVertexAttributesKeys(vertex);
  }
  map_vertices_attributes::const_iterator it = m_verticesAttributes.begin();
  const map_vertices_attributes::const_iterator end = m_verticesAttributes.end();
  for (; it!=end; ++it) {
    AttributeKey akey = AttributeKey::getKey((*it).first);
    if ((*it).second.find(contView) != (*it).second.end()
	&& (*(*it).second.find(contView)).second != NULL
	&& vertex.id() < static_cast<typename sd::core::AttributeCollection<unsigned int>*>((*(*it).second.find(contView)).second)->size())
      result.insert(akey);
  }
  return result;
}

template<typename T, unsigned int N>
sd::core::AttributeType
sd::core::MeshView_<T, N>::getVertexAttributeDataTypeAtKey(const sd::core::VertexFromMeshView<T>& vertex,
							   const AttributeKey& akey) const {
  assert(akey.id() != 0);
  sd::core::MeshView* contView = (sd::core::MeshView*) vertex.containingView();
  assert(*contView <= *this);
  if ((m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end()
       || m_verticesAttributes.find(akey.id())->second.find(contView) == m_verticesAttributes.find(akey.id())->second.end())
      && askableParent() != NULL
      && *contView < *this) {
    return askableParent()->getVertexAttributeDataTypeAtKey(vertex, akey);
  } else {
    assert(m_verticesAttributes.find(akey.id()) != m_verticesAttributes.end());
    assert(m_verticesAttributes.find(akey.id())->second.find(contView) != m_verticesAttributes.find(akey.id())->second.end());
    assert(m_verticesAttributes.find(akey.id())->second.find(contView)->second != NULL);
    assert(vertex.id() < m_verticesAttributes.find(akey.id())->second.find(contView)->second->size());
    return m_verticesAttributes.find(akey.id())->second.find(contView)->second->getAttributeType();
  }
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::isAttributeSet(const sd::core::VertexFromMeshView<T>& vertex,
					  const sd::core::AttributeKey& akey) const {
  assert(akey.id() != 0);
  sd::core::MeshView* contView = (sd::core::MeshView*) vertex.containingView();
  assert(*contView <= *this);
  if ((m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end()
       || m_verticesAttributes.find(akey.id())->second.find(contView) == m_verticesAttributes.find(akey.id())->second.end())
      && askableParent() != NULL
      && *contView < *this) {
    return askableParent()->isAttributeSet(vertex, akey);
  } else {
    return (m_verticesAttributes.find(akey.id()) != m_verticesAttributes.end()
	    && m_verticesAttributes.find(akey.id())->second.find(contView) != m_verticesAttributes.find(akey.id())->second.end()
	    && m_verticesAttributes.find(akey.id())->second.find(contView)->second != NULL
	    && vertex.id() < (*static_cast<sd::core::AttributeCollection<unsigned int>*>(m_verticesAttributes.find(akey.id())->second.find(contView)->second)).size());
  }
}

template<typename T, unsigned int N>
template<typename A>
const A&
sd::core::MeshView_<T, N>::getAttribute(const sd::core::VertexFromMeshView<T>& vertex,
					const sd::core::AttributeKey& akey) {
  assert(akey.id() != 0);
  sd::core::MeshView* contView = (sd::core::MeshView*) vertex.containingView();
  assert(*contView <= *this);
  if ((m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end()
       || m_verticesAttributes[akey.id()].find(contView) == m_verticesAttributes[akey.id()].end())
      && askableParent() != NULL
      && *contView < *this) {
    return askableParent()->template getAttribute<A>(vertex, akey);
  } else {
#ifdef DEBUG
    if (m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end()
	|| m_verticesAttributes[akey.id()].find(contView) == m_verticesAttributes[akey.id()].end()) {
      std::cerr << askableParent() << " && " << contView->getId() << " < " << this->getId() << " ? " << (*contView < *this) << " askAtt : " << akey.name() << std::endl;
      printTree();
    }
    assert(m_verticesAttributes.find(akey.id()) != m_verticesAttributes.end());
    assert(*contView <= *this);
    assert(askableParent() == NULL || *contView <= *this);
    assert(m_verticesAttributes[akey.id()].find(contView) != m_verticesAttributes[akey.id()].end());
    assert((*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][contView])).size() > vertex.id());
    assert(m_verticesAttributes[akey.id()][contView]->getAttributeType() == DataType_<A>());
#endif // DEBUG
    //if((*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][contView])).size() <= vertex.id())
    //(*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][contView])).getData().resize(vertex.id()+1);
    return (*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][contView]))[vertex.id()];
  }
}

template<typename T, unsigned int N>
template<typename A>
void
sd::core::MeshView_<T, N>::setAttribute(const sd::core::VertexFromMeshView<T>& vertex,
					const sd::core::AttributeKey& akey,
					const A& val) {
  assert(akey.id() != 0);
  sd::core::MeshView* view = (sd::core::MeshView*) vertex.containingView();
  if (m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end()
       || m_verticesAttributes[akey.id()].find(view) == m_verticesAttributes[akey.id()].end()) {
    if (this->isAttributeSet(vertex, akey)) {
      this->importParentVerticesAttribute(akey);
    } else {
      if (m_verticesAttributes.find(akey.id()) == m_verticesAttributes.end())
	m_verticesAttributes[akey.id()] = std::map<MeshView*, AttributeCollection<unsigned int>* >();
      m_verticesAttributes[akey.id()][view] = new sd::core::AttributeVect<A>();
    }
  }
  assert(m_verticesAttributes[akey.id()][view]->getAttributeType() == DataType_<A>());
  if ((*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][view])).size() <= vertex.id())
    (*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][view])).getData().resize(vertex.id()+1);
  (*static_cast<typename sd::core::AttributeVect<A>*>(m_verticesAttributes[akey.id()][view]))[vertex.id()] = val;
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::importAttributes(sd::core::VertexFromMeshView<T>& v1,
					    const sd::core::VertexFromMeshView<T>& v2) {
  std::set<sd::core::AttributeKey> keys = getVertexAttributesKeys(v2);
  MeshView* containingView1 = (sd::core::MeshView*) v1.containingView();
  MeshView* containingView2 = (sd::core::MeshView*) v2.containingView();
  bool result = true;
  std::set<sd::core::AttributeKey>::const_iterator keyIt = keys.begin();
  const std::set<sd::core::AttributeKey>::const_iterator endKey = keys.end();
  for (; keyIt!=endKey; ++keyIt) {
    if (m_verticesAttributes.find((*keyIt).id()) == m_verticesAttributes.end())
      importParentVerticesAttribute(*keyIt);
    map_vertices_attributes::iterator itAtts = m_verticesAttributes.find((*keyIt).id());
    assert(itAtts != m_verticesAttributes.end());
    assert((*itAtts).second.find(containingView2) != (*itAtts).second.end());
    if ((*itAtts).second.find(containingView1) == (*itAtts).second.end()) {
      AttributeCollection<unsigned int>* tmp = (*(*itAtts).second.find(containingView2)).second->clone();
      tmp->clear();
      (*itAtts).second[containingView1] = tmp;
    }
    result &= (*itAtts).second[containingView1]->import(v1.id(), (*itAtts).second[containingView2], v2.id());
    assert((*itAtts).second[containingView1]->size() > 0);
  }
  return result;
}

// ---================== Vertices Attributes Management ==================--- }

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::isAttributeSet(const sd::core::FaceFromMeshView<T, N>* face,
					  const sd::core::AttributeKey& akey) const {
  return facesAttributes()->isAttributeSet(face, akey, this);
}

template<typename T, unsigned int N>
template<typename A>
const A&
sd::core::MeshView_<T, N>::getAttribute(const sd::core::FaceFromMeshView<T, N>* face,
					const AttributeKey& akey) {
  return facesAttributes()->template getAttribute<A>(face, akey, this);
}

template<typename T, unsigned int N>
template<typename A>
void
sd::core::MeshView_<T, N>::setAttribute(const sd::core::FaceFromMeshView<T, N>* face,
					const AttributeKey& akey,
					const A& val) {
  return facesAttributes()->template setAttribute<A>(face, akey, val, this);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::importAttributes(sd::core::FaceFromMeshView<T, N>* f1,
					    const sd::core::FaceFromMeshView<T, N>* f2) {
  return facesAttributes()->importAttributes(f1, f2, this);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::importAttributes(sd::core::EdgeFromMeshView<T>* e1,
					    const sd::core::EdgeFromMeshView<T>* e2) {
  return edgesAttributes()->importAttributes(e1, e2, this);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::isNormalSet(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  return isAttributeSet(vertex, normalKey);
}

template<typename T, unsigned int N>
const sd::core::Normal<T>&
sd::core::MeshView_<T, N>::getNormal(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  return getAttribute<typename sd::core::Normal<T> >(vertex, normalKey);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setNormal(const VertexFromMeshView<T>& vertex,
				     const sd::core::Normal<T>& normal) {
  const sd::core::AttributeKey& normalKey = sd::core::AttributeKey::getKey("Normal");
  setAttribute<typename sd::core::Normal<T> >(vertex, normalKey, normal);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::isRGBCoordianteSet(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  return isAttributeSet(vertex, colorKey);
}

template<typename T, unsigned int N>  
const sd::core::RGBCoordinates<unsigned char>&
sd::core::MeshView_<T, N>::getRGBCoordinates(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  return getAttribute<typename sd::core::RGBCoordinates<unsigned char> >(vertex, colorKey);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setRGBCoordinates(const VertexFromMeshView<T>& vertex,
					    const sd::core::RGBCoordinates<unsigned char>& rgb) {
  assert(this == vertex.askingView());
  const sd::core::AttributeKey& colorKey = sd::core::AttributeKey::getKey("Color");
  setAttribute<typename sd::core::RGBCoordinates<unsigned char> >(vertex, colorKey, rgb);
}

template<typename T, unsigned int N>
bool
sd::core::MeshView_<T, N>::isTextureSet(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  return isAttributeSet(vertex, textureKey);
}

template<typename T, unsigned int N>
const sd::core::TextureCoordinates<double>&
sd::core::MeshView_<T, N>::getTexture(const VertexFromMeshView<T>& vertex) {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  return getAttribute<typename sd::core::TextureCoordinates<double> >(vertex, textureKey);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::setTexture(const VertexFromMeshView<T>& vertex,
				      const sd::core::TextureCoordinates<double>& texture) {
  const sd::core::AttributeKey& textureKey = sd::core::AttributeKey::getKey("Texture");
  setAttribute<typename sd::core::TextureCoordinates<double> >(vertex, textureKey, texture);
}

#ifdef DEBUG
template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::printTree() {
  for (unsigned int i=0; i<getPreviousViewNodes().size(); ++i)
    static_cast<MeshView_<T, N>* >(getPreviousViewNodes()[i])->printParent(0);
  std::cerr << "I'm : " << this << " id : " << getId().toString() << std::endl;
  for (unsigned int i=0; i<getNextViewNodes().size(); ++i)
    static_cast<MeshView_<T, N>* >(getNextViewNodes()[i])->printChild(0);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::printParent(int l) {
  std::cerr << "Parent l:" << l << " : " << this << " id : " << getId().toString() << std::endl;
  for (unsigned int i=0; i<getPreviousViewNodes().size(); ++i)
    static_cast<MeshView_<T, N>* >(getPreviousViewNodes()[i])->printParent(l+1);
}

template<typename T, unsigned int N>
void
sd::core::MeshView_<T, N>::printChild(int l) {
  std::cerr << "Child l:" << l << " : " << this << " id : " << getId().toString() << std::endl;
  for (unsigned int i=0; i<getNextViewNodes().size(); ++i)
    static_cast<MeshView_<T, N>* >(getNextViewNodes()[i])->printParent(l+1);
}
#endif // DEBUG

#endif /*! SD_CORE_MESH_VIEW__HXX */
