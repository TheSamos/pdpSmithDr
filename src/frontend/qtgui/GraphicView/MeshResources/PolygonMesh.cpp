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

#include <core/mesh/MeshView_.hpp>
#include <core/mesh/VertexFromMeshView.hpp>
#include "PolygonMesh.hpp"
#include <SmartPointers.hpp>

#include <cassert>
#include <cmath>
#include <vector>

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {
      
      namespace mesh_resources {

	/*
	 * PolygonMesh
	 */
	PolygonMesh::PolygonMesh(const frontend::MeshInfoPtr mesh,
				 const sd::core::RGBCoordinates<float>& defaultColor)
	  : MeshViewResource(PolygonMeshType, mesh),
	    m_vertices(),
	    m_vertexColors(),
	    m_vertexNormals(),
	    m_faces(),
	    m_faceNormals(),
	    m_image(NULL),
	    m_faceDegree(0),
	    m_defaultColor(defaultColor) {
	  init();
	}
	
	PolygonMesh::~PolygonMesh()
	{}
	
	template<typename T, unsigned int N>
	void
	PolygonMesh::buildObject(sd::core::MeshView* view) {
	  sd::core::MeshView_<T, N>* mesh = dynamic_cast<sd::core::MeshView_<T, N>*>(view);
	  if (!mesh)
	    return;
	  
	  unsigned int nbVertices = mesh->nbVertices();
	  unsigned int nbFaces = mesh->nbFaces();
	  
	  m_faceDegree = N;
	  m_vertices.resize(nbVertices*3);
	  m_vertexColors.resize(nbVertices*4, 1.);
	  m_vertexNormals.resize(nbVertices*3);
	  m_vertexTextures.resize(nbVertices*2, 0.);
	  m_faces.resize(nbFaces*N);
	  m_faceNormals.resize(nbFaces*3);

	  {
	    const sd::core::AttributeVect<sd::core::ImageView*>* images = NULL;
	    if (sd::core::AttributeKey::containsKey("ImagesTextures"))
	      images = static_cast<const sd::core::AttributeVect<sd::core::ImageView*>*>(((sd::core::MeshView*) mesh)->getAttribute(sd::core::AttributeKey::getKey("ImagesTextures")));
	    if (images != NULL && images->size() > 0
		&& images->getData()[0]->channelType() == sd::core::DataType_<UINT8>()
		&& images->getData()[0]->colorSpace() == sd::RGB)
	      m_image = static_cast<sd::core::ImageView_<UINT8>*>(images->getData()[0]);
	  }
	  
	  const sd::core::BoundingBox<T>& bb = mesh->boundingBox();
	  sd::core::Point3d<T> offset = bb.origin();
	  
	  T normalizeFactor = 1./std::max(std::max(bb.width(), bb.height()), bb.depth())*2.;
	  float cx = (2*offset.x()+bb.width())/2.;
	  float cy = (2*offset.y()+bb.height())/2.;
	  float cz = (2*offset.z()+bb.depth())/2.;
	  
	  {
	    size_t vertexId = 0;
	    typename sd::core::MeshView_<T, N>::vertices_const_iterator it = mesh->beginVertices();
	    const typename sd::core::MeshView_<T, N>::vertices_const_iterator end = mesh->endVertices();
	    for (; it!=end; ++it, ++vertexId) {
	      assert((*it).globalIndex() == vertexId);
	      m_vertices[vertexId*3] = ((*it).x()-cx)*normalizeFactor;
	      m_vertices[vertexId*3+1] = ((*it).y()-cy)*normalizeFactor;
	      m_vertices[vertexId*3+2] = ((*it).z()-cz)*normalizeFactor;
	      
	      if ((*it).isRGBCoordinatesSet()) {
		const sd::core::RGBCoordinates<unsigned char> rgbCoord((*it).getRGBCoordinates());
		m_vertexColors[vertexId*4] = (float)rgbCoord.r()/255.;
		m_vertexColors[vertexId*4+1] = (float)rgbCoord.g()/255.;
		m_vertexColors[vertexId*4+2] = (float)rgbCoord.b()/255.;
		m_vertexColors[vertexId*4+3] = (float)rgbCoord.a()/255.;
	      } else {
		m_vertexColors[vertexId*4] = m_defaultColor.r();
		m_vertexColors[vertexId*4+1] = m_defaultColor.g();
		m_vertexColors[vertexId*4+2] = m_defaultColor.b();
		m_vertexColors[vertexId*4+3] = 1.;
	      }
	      if ((*it).isNormalSet()) {
		const sd::core::Normal<T> normal((*it).getNormal());
		m_vertexNormals[vertexId*3] = normal.x();
		m_vertexNormals[vertexId*3+1] = normal.y();
		m_vertexNormals[vertexId*3+2] = normal.z();
	      } else {
		const sd::core::Normal<T> normal(mesh->computeVertexNormal(*it));
		m_vertexNormals[vertexId*3] = normal.x();
		m_vertexNormals[vertexId*3+1] = normal.y();
		m_vertexNormals[vertexId*3+2] = normal.z();
	      }
	      if ((*it).isTextureCoordinatesSet()) {
		const sd::core::TextureCoordinates<double> texture((*it).getTextureCoordinates());
		m_vertexTextures[vertexId*2] = texture.u();
		m_vertexTextures[vertexId*2+1] = texture.v();
	      }
	    }
	  }
	  
	  {
	    size_t faceId = 0;
	    typename sd::core::MeshView_<T, N>::faces_const_iterator it =  mesh->beginFaces();
	    const typename sd::core::MeshView_<T, N>::faces_const_iterator end =  mesh->endFaces();
	    for (; it!=end; ++it, ++faceId) {
	      for (unsigned int i=0; i<N; ++i) {
		const sd::core::VertexFromMeshView<T> vertex = (*it)->getAt(mesh, i);
		m_faces[faceId*N+i] = vertex.globalIndex();
	      }
	      if ((*it)->isNormalSet(mesh)) {
		const sd::core::Normal<T>& normal = (*it)->getNormal(mesh);
		m_faceNormals[faceId*3] = normal.x();
		m_faceNormals[faceId*3+1] = normal.y();
		m_faceNormals[faceId*3+2] = normal.z();
	      }
	    }
	  }
	}
	
	void
	PolygonMesh::getVNCData(std::vector<float>& vertices,
				unsigned int& cardCoord,
				unsigned int& cardNormal,
				unsigned int& cardColor) {
	  cardCoord = 3;
	  cardNormal = 3;
	  cardColor = 4;
	  const unsigned int factor = cardCoord * cardNormal * cardColor;
	  vertices.resize(m_faces.size() * factor);
	  for (unsigned int i=0; i < m_faces.size(); ++i) {
	    for (unsigned int k=0; k < cardCoord; ++k)
	      vertices[i*factor+k] = m_vertices[m_faces[i]*cardCoord+k];
	    for (unsigned int k=0; k < cardNormal; ++k)
	      vertices[i*factor+cardCoord+k] = m_faceNormals[i/m_faceDegree*cardNormal+k];
	      //vertices[i*factor+cardCoord+k] = m_vertexNormals[m_faces[i]*cardNormal+k];
	    for (unsigned int k=0; k < cardColor; ++k)
	      vertices[i*factor+cardCoord+cardNormal+k] = m_vertexColors[m_faces[i]*cardColor+k];
	  }
	}
	
	
#define CALL_OBJECT_BUILDER(Type)					\
	if (dataType.isOfType<Type>()) {				\
	  if (degree == 3) {						\
	    buildObject<Type, 3>(mesh);					\
	  } else if (degree == 4) {					\
	    buildObject<Type, 4>(mesh);					\
	  } else {							\
	    assert(!"Should never get here!");				\
	    return;							\
	  }								\
	}
	
	void
	PolygonMesh::init() {
	  sd::core::MeshView* mesh = m_meshInfo->meshPtr.get();
	  sd::core::VertexDataType dataType = mesh->getVertexDataType();
	  int degree = mesh->getFaceDegree();
	  
	  CALL_OBJECT_BUILDER(long double)
	  else CALL_OBJECT_BUILDER(double)
	    else CALL_OBJECT_BUILDER(float)
	      else CALL_OBJECT_BUILDER(int)
		else {
		  assert(!"Should never get here!");
		  return;
		}
	}
	
#undef CALL_OBJECT_BUILDER
		
      }
      
    }
    
  }
  
}
