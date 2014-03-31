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

#ifndef SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_MESHVIEWRESOURCE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_MESHVIEWRESOURCE_HPP

#include "../MeshResource.hpp"
#include <frontend/lib/TypesInfo.hpp>

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {
      
      namespace mesh_resources {

	class MeshViewResource : public MeshResource {

	public:

	  enum Type { PolygonMeshType };
	  
	public:
	  
	  virtual ~MeshViewResource();
	  
	  virtual std::string getName() const;
	  
	  virtual inline const sd::core::MeshView* getData() const {
	    return m_meshInfo->meshPtr.get();
	  }
	  
	  const frontend::MeshInfoPtr& meshInfo() const {
	    return m_meshInfo;
	  }
	  
	protected:

	  MeshViewResource(MeshViewResource::Type t, frontend::MeshInfoPtr mesh);
	  
	protected:

	  const Type m_type;
	  const frontend::MeshInfoPtr m_meshInfo;

	};

	typedef sd::intrusive_ptr<MeshViewResource> MeshViewResourcePtr;

      }
      
    }
    
  }
  
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_MESHVIEWRESOURCE_HPP */
