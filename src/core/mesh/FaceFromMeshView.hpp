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

#ifndef SD_CORE_FACE_FROM_MESH_VIEW_HPP
#define SD_CORE_FACE_FROM_MESH_VIEW_HPP

#include "VertexBasedFromMeshView.hpp"

#include "MeshView.hpp"

#include <vector>

namespace sd {
  namespace core {

    template<typename T, unsigned int N>
    class MeshView_;

    template<typename T, unsigned int N>
    class FaceFromMeshView : public VertexBasedFromMeshView<T, N> {

    public:

      // friend class MeshView_<T, 3>;
      friend class MeshView_<T, N>;
      
      inline bool isNormalSet(sd::core::MeshView_<T, N>* askingView) const;
      inline Normal<T> getNormal(sd::core::MeshView_<T, N>* askingView) const;
      inline void setNormal(const Normal<T>& normal, sd::core::MeshView_<T, N>* askingView);
      
      inline bool isRGBCoordinatesSet(sd::core::MeshView_<T, N>* askingView) const;
      inline const RGBCoordinates<unsigned char>& getRGBCoordinates(sd::core::MeshView_<T, N>* askingView) const;
      inline void setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb,
				    sd::core::MeshView_<T, N>* askingView);
      
      inline bool isAttributeSet(const sd::core::AttributeKey& akey,
				 sd::core::MeshView_<T, N>* askingView) const;
      template<typename A>
      inline const A& getAttribute(const AttributeKey& akey,
				   sd::core::MeshView_<T, N>* askingView);
      template<typename A>
      inline void setAttribute(const AttributeKey& akey, const A& val,
			       sd::core::MeshView_<T, N>* askingView);
      
    protected:

      FaceFromMeshView(MeshView_<T, N>* creationView);

      virtual ~FaceFromMeshView();
      
    };
    
    template<typename T>
    class TriangularFaceFromMeshView : public FaceFromMeshView<T, 3> {

    public:

      TriangularFaceFromMeshView(MeshView_<T, 3>* creationView);
      
      TriangularFaceFromMeshView(MeshView_<T, 3>* creationView,
				 const sd::core::VertexFromMeshView<T>& v0,
				 const sd::core::VertexFromMeshView<T>& v1,
				 const sd::core::VertexFromMeshView<T>& v2);
      
    private:
      
      virtual ~TriangularFaceFromMeshView() {}
      
    };

    template<typename T>
    class QuadrangularFaceFromMeshView : public FaceFromMeshView<T, 4> {

    public:

      QuadrangularFaceFromMeshView(MeshView_<T, 4>* creationView);

      QuadrangularFaceFromMeshView(MeshView_<T, 4>* creationView,
				   const sd::core::VertexFromMeshView<T>& v0,
				   const sd::core::VertexFromMeshView<T>& v1,
				   const sd::core::VertexFromMeshView<T>& v2,
				   const sd::core::VertexFromMeshView<T>& v3);
      
    private:
      
      virtual ~QuadrangularFaceFromMeshView() {}
      
    };
    
    template<typename T, unsigned int N>
    inline FaceFromMeshView<T, N>* clone(sd::core::MeshView_<T, N>* creationView,
					 FaceFromMeshView<T, N>* face);
    
    template<typename T>
    inline FaceFromMeshView<T, 3>* newFace(sd::core::MeshView_<T, 3>* creationView,
					   const sd::core::VertexFromMeshView<T>& v0,
					   const sd::core::VertexFromMeshView<T>& v1,
					   const sd::core::VertexFromMeshView<T>& v2);
    template<typename T>
    inline FaceFromMeshView<T, 4>* newFace(sd::core::MeshView_<T, 4>* creationView,
					   const sd::core::VertexFromMeshView<T>& v0,
					   const sd::core::VertexFromMeshView<T>& v1,
					   const sd::core::VertexFromMeshView<T>& v2,
					   const sd::core::VertexFromMeshView<T>& v3);
    // Do not use : abort
    template<typename T>
    inline FaceFromMeshView<T, 4>* newFace(sd::core::MeshView_<T, 4>* creationView,
					   const sd::core::VertexFromMeshView<T>& v0,
					   const sd::core::VertexFromMeshView<T>& v1,
					   const sd::core::VertexFromMeshView<T>& v2);
    // Do not use : abort
    template<typename T>
    inline FaceFromMeshView<T, 3>* newFace(sd::core::MeshView_<T, 3>* creationView,
					   const sd::core::VertexFromMeshView<T>& v0,
					   const sd::core::VertexFromMeshView<T>& v1,
					   const sd::core::VertexFromMeshView<T>& v2,
					   const sd::core::VertexFromMeshView<T>& v3);
  }
  
}

#include "FaceFromMeshView.hxx"

#endif /* ! SD_CORE_FACE_FROM_MESH_VIEW_HPP */
