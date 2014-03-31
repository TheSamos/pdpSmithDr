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

#ifndef SD_CORE_EDGE_FROM_MESH_VIEW_HPP
#define SD_CORE_EDGE_FROM_MESH_VIEW_HPP

#include "VertexBasedFromMeshView.hpp"
#include "MeshView.hpp"

namespace sd {
  namespace core {

    template<typename T>
    class EdgeFromMeshView : public VertexBasedFromMeshView<T, 2> {

    public:

      EdgeFromMeshView(MeshView* creationView);

      EdgeFromMeshView(MeshView* creationView,
		       const sd::core::VertexFromMeshView<T>& v0,
		       const sd::core::VertexFromMeshView<T>& v1);

      const VertexFromMeshView<T> first(MeshView* askingView) const;
      const VertexFromMeshView<T> second(MeshView* askingView) const;


      inline bool isNormalSet(sd::core::MeshView* askingView) const;
      inline Normal<T> getNormal(sd::core::MeshView* askingView) const;
      inline void setNormal(const Normal<T>& normal, sd::core::MeshView* askingView);
      
      inline bool isRGBCoordinatesSet(sd::core::MeshView* askingView) const;
      inline const RGBCoordinates<unsigned char>& getRGBCoordinates(sd::core::MeshView* askingView) const;
      inline void setRGBCoordinates(const RGBCoordinates<unsigned char>& rgb,
				    sd::core::MeshView* askingView);
      
      inline bool isAttributeSet(const sd::core::AttributeKey& akey,
				 sd::core::MeshView* askingView) const;
      template<typename A>
      inline const A& getAttribute(const AttributeKey& akey,
				   sd::core::MeshView* askingView);
      template<typename A>
      inline void setAttribute(const AttributeKey& akey, const A& val,
			       sd::core::MeshView* askingView);
      
    };

  }
}

#include "EdgeFromMeshView.hxx"

#endif /* ! SD_CORE_EDGE_FROM_MESH_VIEW_HPP */


    
