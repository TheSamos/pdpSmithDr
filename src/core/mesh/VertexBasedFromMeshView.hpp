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

#ifndef SD_CORE_VERTEX_BASED_FROM_MESH_HPP
#define SD_CORE_VERTEX_BASED_FROM_MESH_HPP

#include "VertexFromMeshView.hpp"

namespace sd {
  
  namespace core {

    class MeshView;

    template<typename T, unsigned int N>
    class VertexBasedFromMeshView {

    protected:

      inline void setAt(unsigned int index, const VertexFromMeshView<T>& vertex);

    public:

      VertexBasedFromMeshView(MeshView* creationView);

      // shallow copy
      VertexBasedFromMeshView(const VertexBasedFromMeshView<T,N>&);

      virtual ~VertexBasedFromMeshView();

      // shallow copy
      const VertexBasedFromMeshView<T,N>& operator=(const VertexBasedFromMeshView<T,N>&);

      inline VertexFromMeshView<T> getAt(MeshView* askingView, unsigned int index);

      inline const VertexFromMeshView<T> getAt(MeshView* askingView, unsigned int index) const;

      inline unsigned int find(const VertexFromMeshView<T>& v) const;
      
      inline unsigned int size() const;
      
      inline bool isValidAtView(MeshView* View) const;
      
      inline void removeAt(MeshView* view);
      
      inline void isNoMore(MeshView* view);
      
      inline bool isDeletable() const;
      
    protected:
      
      // friend class MeshView_<T, 3>;
      // friend class MeshView_<T, 4>;
      
      template<typename U>
      inline bool isNormalSet(sd::core::MeshView_<T, N>* askingView) const;
      template<typename U>
      inline Normal<T> getNormal(sd::core::MeshView_<T, N>* askingView) const;
      template<typename U>
      inline void setNormal(const Normal<T>& normal, sd::core::MeshView_<T, N>* askingView);
      
      template<typename U>
      inline bool isRGBCoordinatesSet(sd::core::MeshView_<T, N>* askingView) const;
      template<typename U>
      inline const RGBCoordinates<unsigned char>& getRGBCoordinates(sd::core::MeshView_<T, N>* askingView) const;
      template<typename U>
      inline void setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb,
				    sd::core::MeshView_<T, N>* askingView);
      
      template<typename U>
      inline bool isAttributeSet(const sd::core::AttributeKey& akey,
				 sd::core::MeshView_<T, N>* askingView) const;
      template<typename U, typename A>
      inline const A& getAttribute(const AttributeKey& akey,
				   sd::core::MeshView_<T, N>* askingView);
      template<typename U, typename A>
      inline void setAttribute(const AttributeKey& akey, const A& val,
			       sd::core::MeshView_<T, N>* askingView);
      
    private:

      VertexBasedFromMeshView();

    private:
      
      std::pair<MeshView*, MeshView::vertex_id> m_data[N];
      MeshView const* m_creationView;
      std::vector<MeshView*> m_removeViews;
      
    };

  }
  
}

#include "VertexBasedFromMeshView.hxx"

#endif /* ! SD_CORE_VERTEX_BASED_FROM_MESH_HPP */
