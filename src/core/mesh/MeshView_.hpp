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

#ifndef SD_CORE_MESH_VIEW__HPP
#define SD_CORE_MESH_VIEW__HPP

#include "MeshView.hpp"

#include <list>
#include <set>
#include <vector>
#include <utility>

#include "../BoundingBox.hpp"

#include "Normal.hpp"
#include "AttributesManager.hpp"

#include "iterators/VertexFromMeshViewIterator.hpp"
#include "iterators/VertexFromMeshViewConstGeometryIterator.hpp"
#include "iterators/VertexFromMeshViewConstIterator.hpp"
#include "iterators/FaceFromMeshViewIterator.hpp"
#include "iterators/EdgeFromMeshViewIterator.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class VertexFromMeshView;
    
    template<typename T, unsigned int N>
    class FaceFromMeshView;
    
    template<typename T>
    class EdgeFromMeshView;
    
    template<typename T, unsigned int N>
    class MeshView_;
    
    template<typename T, unsigned int N>
    class MeshView_ : public MeshView {
      
    public:
      
      friend class VertexFromMeshView<T>;
      friend class VertexBasedFromMeshView<T, N>;
      friend class VertexBasedFromMeshView<T, 2>;
      // friend class FaceFromMeshView<T, N>;
      // friend class EdgeFromMeshView<T>;
      friend class MeshView_<T, 3>;
      friend class MeshView_<T, 4>;
      
      friend class AttributesManager<MeshView_<T, N>, FaceFromMeshView<T, N> >;
      friend class AttributesManager<MeshView_<T, N>, EdgeFromMeshView<T> >;
      
      typedef T DataType;
      static const unsigned int FACE_CARD = N;
      
      typedef VertexFromMeshViewIterator<T> vertices_modifier_iterator;
      typedef VertexFromMeshViewConstGeometryIterator<T> vertices_const_geometry_iterator;
      typedef VertexFromMeshViewConstIterator<T> vertices_const_iterator;
      typedef FaceFromMeshViewIterator<T, N> faces_iterator;
      typedef FaceFromMeshViewIterator<T, N> faces_const_iterator;
      typedef EdgeFromMeshViewIterator<T> edges_iterator;
      typedef EdgeFromMeshViewIterator<T> edges_const_iterator;

      enum MeshStructuration {VF=0, VEF=1};
      
    private:
      
      typedef std::list<std::pair<vertex_id, vertex_id> > list_vertices_id;
      typedef std::map<MeshView*, list_vertices_id> map_vertices_id;
      typedef std::map<attribute_key_id, std::map<MeshView*, AttributeCollection<unsigned int>* > > map_vertices_attributes;
      
      typedef typename std::map<MeshView*, std::vector<std::vector<FaceFromMeshView<T, N>* > > > map_faces;
      typedef typename sd::core::AttributesManager<MeshView_<T, N>, FaceFromMeshView<T, N> > map_faces_attributes;
      
      typedef typename std::map<MeshView*, std::vector<std::vector<EdgeFromMeshView<T>* > > > map_edges;
      typedef typename sd::core::AttributesManager<MeshView_<T, N>, EdgeFromMeshView<T> > map_edges_attributes;
      
      
    public:
      
      MeshView_(const std::string& name,
		const sd::core::Vector<T>& vertices,
		const sd::core::Vector<unsigned int>& faces = sd::core::Vector<unsigned int>(),
		const sd::core::Vector<unsigned int>& edges = sd::core::Vector<unsigned int>(),
		const sd::core::Vector<typename Normal<T>::type>& verticesNormals = sd::core::Vector<typename Normal<T>::type>(),
		//const sd::core::Vector<Normal::type>& facesNormals = sd::core::Vector<Normal::type>(),
		const sd::core::Vector<unsigned char>& verticesColors = sd::core::Vector<unsigned char>(),
		//const sd::core::Vector<unsigned char>& facesColors = sd::core::Vector<unsigned char>(),
		const sd::core::Vector<double>& verticesTextures = sd::core::Vector<double>());
      
      MeshView_(const std::string& name = "");
      
      MeshView_(ViewNode* previousViewNode, const std::string& name = "");
      
      MeshView_(const std::vector<ViewNode*>& previousViewNodes,
		const std::string& name);
      
      virtual ~MeshView_();
      
      virtual VertexDataType getVertexDataType() const;
      virtual unsigned int getFaceDegree() const;
      //virtual RawMesh* getRawMesh();
      
      virtual void* getVertexRawData();
      
      // Run it first before all if the default constructor has been used (for example in a plugins...)
      virtual void build();
      
      void setStructurationMode(MeshStructuration s);
      
      VertexFromMeshView<T> addVertex(const T& x, const T& y, const T& z);
      VertexFromMeshView<T> addVertex(const Point3d<T>& p);
      void removeVertex(const VertexFromMeshView<T>& vertex);
      
      EdgeFromMeshView<T>* addEdge(std::vector<VertexFromMeshView<T> > vs);
      EdgeFromMeshView<T>* addEdge(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2);
      void removeEdge(EdgeFromMeshView<T>* edge);
      
      FaceFromMeshView<T, N>* addFace(std::vector<VertexFromMeshView<T> > vs);
      FaceFromMeshView<T, N>* addFace(const Point3d<T>& p1, const Point3d<T>& p2,
				      const Point3d<T>& p3);
      FaceFromMeshView<T, N>* addFace(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2,
				      const VertexFromMeshView<T>& v3);
      FaceFromMeshView<T, N>* addFace(const Point3d<T>& p1, const Point3d<T>& p2,
				      const Point3d<T>& p3, const Point3d<T>& p4);
      FaceFromMeshView<T, N>* addFace(const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2,
				      const VertexFromMeshView<T>& v3, const VertexFromMeshView<T>& v4);
      void removeFace(FaceFromMeshView<T, N>* face);
      
      void setFusionDistance(float distance); // negative value to disable fusion...
      
      unsigned int nbVertices() const;
      unsigned int nbFaces();
      unsigned int nbEdges();
      
      VertexFromMeshView<T> getVertexAt(unsigned int i);
      
      FaceFromMeshView<T, N>* getFaceAt(unsigned int i);
      
      EdgeFromMeshView<T>* getEdgeAt(unsigned int i);
      
      inline vertices_const_geometry_iterator beginVerticesConstGeometry();
      inline vertices_const_geometry_iterator endVerticesConstGeometry();
      
      inline vertices_const_iterator beginVertices() const;
      inline vertices_const_iterator endVertices() const;
      
      inline vertices_modifier_iterator beginVerticesModifier();
      inline vertices_modifier_iterator endVerticesModifier();
      
      inline faces_iterator beginFaces();
      inline faces_iterator endFaces();
      
      inline edges_iterator beginEdges();
      inline edges_iterator endEdges();
      
      inline const sd::core::BoundingBox<T>& boundingBox();
      
      inline void updateBoundingBox();
      
      inline void getNeighborhood(const VertexFromMeshView<T>& vertex,
				  std::vector<VertexFromMeshView<T> >& result);
      
      inline void getNeighboringFaces(const VertexFromMeshView<T>& vertex,
				      std::vector<sd::core::FaceFromMeshView<T, N>* >& result);
      inline void getNeighboringFaces(const sd::core::VertexFromMeshView<T>& v1,
				      const sd::core::VertexFromMeshView<T>& v2,
				      std::vector<sd::core::FaceFromMeshView<T, N>* >& result);

      inline void getNeighboringEdges(const VertexFromMeshView<T>& vertex,
				      std::vector<sd::core::EdgeFromMeshView<T>* >& result);
      
      inline void computeVerticesNormals();
      
      inline std::set<AttributeKey> getVerticesAttributesKeys() const;
      inline AttributeType getVerticesAttributeDataTypeAtKey(const AttributeKey& akey) const;
      inline bool verticesAttributeContainsKey(const AttributeKey& akey) const;
      inline bool removeVerticesAttributeKey(const AttributeKey& akey);
      inline bool removeVerticesSelection();
      
      inline std::set<AttributeKey> getFacesAttributesKeys() const;
      inline AttributeType getFacesAttributeDataTypeAtKey(const AttributeKey& akey) const;
      inline bool facesAttributeContainsKey(const AttributeKey& akey) const;
      inline bool removeFacesAttributeKey(const AttributeKey& akey);
      inline bool removeFacesSelection();
      
      inline std::set<AttributeKey> getEdgesAttributesKeys() const;
      inline AttributeType getEdgesAttributeDataTypeAtKey(const AttributeKey& akey) const;
      inline bool edgesAttributeContainsKey(const AttributeKey& akey) const;
      inline bool removeEdgesAttributeKey(const AttributeKey& akey);
      inline bool removeEdgesSelection();
      
      inline sd::core::Normal<T> computeVertexNormal(const sd::core::VertexFromMeshView<T>& vertex);
      
    private:
      
      inline void init();
      
      inline void clear();
      
      inline void unlinkFace(FaceFromMeshView<T, N>* face);
      inline void unlinkEdge(EdgeFromMeshView<T>* edge);
      
      inline void signalVertexAdded(MeshView* containingView, const vertex_id& vertexId);
      
      inline FaceFromMeshView<T, N>* signalFaceAdded(const std::vector<VertexFromMeshView<T> > f, bool isFirstCall);
      inline EdgeFromMeshView<T>* signalEdgeAdded(const VertexFromMeshView<T>& v1,
						  const VertexFromMeshView<T>& v2, bool isFirstCall);
      
      map_faces* faces();
      map_faces_attributes* facesAttributes();
      const map_faces_attributes* facesAttributes() const;
      map_edges* edges();
      map_edges_attributes* edgesAttributes();
      const map_edges_attributes* edgesAttributes() const;
      
      inline unsigned int globalIndexOf(const VertexFromMeshView<T>& vertex);
      
      inline MeshView_<T, N>* askableParent();
      inline const MeshView_<T, N>* askableParent() const;
      
      // +++================== Vertices Attributes Management ==================+++ {
      
      inline std::map<MeshView*, AttributeCollection<unsigned int>*>& getVerticesAttribute(const AttributeKey& akey);
      inline void setVerticesAttribute(const AttributeKey& akey, MeshView* mw,
				       AttributeCollection<unsigned int>* attribute);
      inline void importParentVerticesAttribute(const sd::core::AttributeKey& akey);

      inline std::set<AttributeKey> getVertexAttributesKeys(const sd::core::VertexFromMeshView<T>& vertex) const;
      inline AttributeType getVertexAttributeDataTypeAtKey(const sd::core::VertexFromMeshView<T>& vertex,
							   const AttributeKey& akey) const;
      
      inline bool isAttributeSet(const sd::core::VertexFromMeshView<T>& vertex,
				 const sd::core::AttributeKey& akey) const;
      template<typename A>
      inline const A& getAttribute(const sd::core::VertexFromMeshView<T>& vertex,
				   const AttributeKey& akey);
      template<typename A>
      inline void setAttribute(const sd::core::VertexFromMeshView<T>& vertex,
			       const AttributeKey& akey,
			       const A& val);
      inline bool importAttributes(sd::core::VertexFromMeshView<T>& v1,
				   const sd::core::VertexFromMeshView<T>& v2);
      
      // ---================== Vertices Attributes Management ==================--- }
      
      inline bool isAttributeSet(const sd::core::FaceFromMeshView<T, N>* face,
				 const sd::core::AttributeKey& akey) const;
      template<typename A>
      inline const A& getAttribute(const sd::core::FaceFromMeshView<T, N>* face,
				   const AttributeKey& akey);
      template<typename A>
      inline void setAttribute(const sd::core::FaceFromMeshView<T, N>* face,
			       const AttributeKey& akey,
			       const A& val);
      inline bool importAttributes(sd::core::FaceFromMeshView<T, N>* f1,
				   const sd::core::FaceFromMeshView<T, N>* f2);
      
      inline bool isAttributeSet(const sd::core::EdgeFromMeshView<T>* edge,
				 const sd::core::AttributeKey& akey) const;
      template<typename A>
      inline const A& getAttribute(const sd::core::EdgeFromMeshView<T>* edge,
				   const AttributeKey& akey);
      template<typename A>
      inline void setAttribute(const sd::core::EdgeFromMeshView<T>* edge,
			       const AttributeKey& akey,
			       const A& val);
      inline bool importAttributes(sd::core::EdgeFromMeshView<T>* e1,
       				   const sd::core::EdgeFromMeshView<T>* e2);
      
      inline bool isNormalSet(const VertexFromMeshView<T>& vertex);
      inline const Normal<T>& getNormal(const VertexFromMeshView<T>& vertex);
      inline void setNormal(const VertexFromMeshView<T>& vertex, const Normal<T>& normal);

      inline bool isRGBCoordianteSet(const VertexFromMeshView<T>& vertex);
      inline const RGBCoordinates<unsigned char>& getRGBCoordinates(const VertexFromMeshView<T>& vertex);
      inline void setRGBCoordinates(const VertexFromMeshView<T>& vertex, const RGBCoordinates<unsigned char>& rgb);
      
      inline bool isTextureSet(const VertexFromMeshView<T>& vertex);
      inline const TextureCoordinates<double>& getTexture(const VertexFromMeshView<T>& vertex);
      inline void setTexture(const VertexFromMeshView<T>& vertex, const TextureCoordinates<double>& texture);
            
    private:
      
      MeshView_(const MeshView_<T, N>& rhs) = delete;

      MeshView_<T, N>& operator=(const MeshView_<T, N>& rhs) = delete;
      
    private:
      
#ifdef DEBUG
      inline void printTree();
      
      inline void printParent(int l);

      inline void printChild(int l);
#endif // DEBUG      

    private:
      
      bool m_isMeshRoot;
      
      // *** Vertices ***
      sd::core::Vector<T> m_vertices;
      map_vertices_id m_verticesIds;
      map_vertices_attributes m_verticesAttributes;
      // sd::core::Vector<typename Normal<T>::type> m_verticesNormalsData;
      // sd::core::Vector<unsigned char> m_verticesColorsData;
      // sd::core::Vector<double> m_verticesTexturesData;
      // *** Faces ***
      sd::core::SharedObject<map_faces> m_faces; // first is containingView, second is the vertex_id
      map_faces_attributes m_facesAttributes;
      // ^- Only use in a mesh root view !!! Warning can not be use directely !!!

      // *** Edges ***
      sd::core::SharedObject<map_edges> m_edges; // first  is containingView, second is the vertex_id
      map_edges_attributes m_edgesAttributes;
      // ^- Only use in a mesh root view !!! Warning can not be use directely !!!
      
      MeshStructuration m_meshStruct;
      
      BoundingBox<T> m_bb;
      bool m_bbIsUpdated;
      
      double m_fusionDistance;
      
    };
    
  }
}

#include "MeshView_.hxx"

#endif /*! SD_CORE_MESH_VIEW__HPP */
