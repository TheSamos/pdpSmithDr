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

#ifndef SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_HPP

#include <utility>
#include <vector>

#include <Types.hpp>

#include <SmartPointers.hpp>

namespace sd {
  
  namespace core {
   
    class MeshView;
    
  }
  namespace qtgui {
    
    namespace graphicView {

      class MeshResource;
      typedef sd::intrusive_ptr<MeshResource> MeshResourcePtr;

      class MeshResource {

      public:
	
	virtual ~MeshResource();
	
	virtual std::string getName() const = 0;
	
	virtual std::vector<float>& getVertices() = 0;
	virtual std::vector<float>& getVertexColorsRGBA() = 0;
	virtual std::vector<float>& getVertexNormals() = 0;
	virtual unsigned int getFacesDegree() const = 0;
	virtual std::vector<unsigned int>& getFaces() = 0;
	virtual std::vector<float>& getFaceNormals() = 0;
	
	virtual void getVNCData(std::vector<float>& vertices,
				unsigned int& cardCoord,
				unsigned int& cardNormal,
				unsigned int& cardColor) = 0;
	
	virtual const sd::core::MeshView* getData() const = 0;
	
	virtual void init() = 0;
	
      protected:
	
	MeshResource();
	
      private:
	
	friend inline void
	intrusive_ptr_add_ref(MeshResource* self) {
	  ++self->m_refCounter;
	}
	
	friend inline void
	intrusive_ptr_release(MeshResource* self) {
	  if (!--self->m_refCounter)
	    delete self;
	}
	
      private:
	
	size_t m_refCounter;
	
      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCE_HPP */
