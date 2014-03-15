
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

#ifndef SD_CORE_VERTEX_FROM_MESH_VIEW_HPP
#define SD_CORE_VERTEX_FROM_MESH_VIEW_HPP

#include "MeshView.hpp"
#include <core/RGBCoordinates.hpp>
#include "TextureCoordinates.hpp"
#include "Normal.hpp"

namespace sd {
  namespace core {
    
    template<typename T, unsigned int N>
    class FaceFromMeshView;
    
    template<typename T>
    class EdgeFromMeshView;
    
    template<typename T, unsigned int N>
    class MeshView_;
    
    template<typename T>
    class VertexFromMeshView : public Point3d<T> {

    public:

      VertexFromMeshView();

      VertexFromMeshView(MeshView* askingView,
			 MeshView* containingView,
			 MeshView::vertex_id id);

      VertexFromMeshView(const VertexFromMeshView<T>&);

      virtual ~VertexFromMeshView();

      inline bool isNull() const;

      inline VertexFromMeshView<T> set(const T& px, const T& py, const T& pz);

      inline const T& setX(const T& px);
      inline const T& setY(const T& py);
      inline const T& setZ(const T& pz);

      inline const MeshView* askingView() const;
      inline const MeshView* containingView() const;

      inline MeshView::vertex_id id() const;
      
      inline unsigned int globalIndex() const;

      void getNeighborhood(std::vector<sd::core::VertexFromMeshView<T> >& neighbors) const;
      
      void getNeighborhood(std::vector<std::vector<sd::core::VertexFromMeshView<T> > >& neighbors,
			   unsigned int nbRing) const;
      
      template<unsigned int N>
      void getNeighboringFaces(std::vector<sd::core::FaceFromMeshView<T, N>* >& result);
      
      void getNeighboringEdges(std::vector<sd::core::EdgeFromMeshView<T>* >& result);
      
      inline bool isAttributeSet(const AttributeKey& akey) const;
      template<typename A>
      inline const A& getAttribute(const AttributeKey& akey) const;
      template<typename A>
      inline void setAttribute(const AttributeKey& akey,
			       const A& val);
      
      inline bool isSelected() const;
      inline void setSelected(bool selected);
      
      inline bool isNormalSet() const;
      inline Normal<T> getNormal() const;
      inline void setNormal(const Normal<T>& normal);
      
      inline bool isRGBCoordinatesSet() const;
      inline const RGBCoordinates<unsigned char>& getRGBCoordinates() const;
      inline void setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb);
      
      inline bool isTextureCoordinatesSet() const;
      inline const TextureCoordinates<double>& getTextureCoordinates() const;
      inline void setTextureCoordinates(const TextureCoordinates<double>& uvw);
      
      inline const VertexFromMeshView<T>& operator=(const VertexFromMeshView<T>& v);

      inline VertexFromMeshView<T>& operator+=(const Vector3d<T>& v);
      
      inline bool operator<(const sd::core::VertexFromMeshView<T>& v) const; // Used to sort in a container
      inline bool operator>(const sd::core::VertexFromMeshView<T>& v) const;
            
    private:
      
      template<unsigned int N>
      inline void swapNeighborhood(sd::core::MeshView_<T, N>* askingView,
				   sd::core::VertexFromMeshView<T>& futur);
      
      template<unsigned int N>
      inline bool importAttributes(const sd::core::VertexFromMeshView<T>& v);
      
    private:

      MeshView* m_askingView;
      MeshView* m_containingView;
      MeshView::vertex_id m_id;

    };
    
    template<typename T>
    inline bool operator== (const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2);
    template<typename T>
    inline bool operator!= (const VertexFromMeshView<T>& v1, const VertexFromMeshView<T>& v2);
    
  }
}

#include "VertexFromMeshView.hxx"

#endif /* ! SD_CORE_VERTEX_FROM_MESH_VIEW_HPP */
