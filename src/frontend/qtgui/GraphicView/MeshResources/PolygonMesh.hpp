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

#ifndef SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCES_POLYGONMESH_HPP
#define SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCES_POLYGONMESH_HPP

#include <map>
#include <vector>

#include <GL/glew.h>

#include "MeshViewResource.hpp"
#include <frontend/lib/TypesInfo.hpp>
#include <core/RGBCoordinates.hpp>
#include <core/image/ImageView_.hpp>

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {

      class GraphicsResourceManager;

      namespace mesh_resources {

	class PolygonMesh : public MeshViewResource {

	public:

	  virtual ~PolygonMesh();
	  
	  virtual inline std::vector<float>& getVertices() {return m_vertices;}
	  virtual inline std::vector<float>& getVertexColorsRGBA() {return m_vertexColors;}
	  virtual inline std::vector<float>& getVertexNormals() {return m_vertexNormals;}
	  virtual inline std::vector<float>& getVertexTextures() {return m_vertexTextures;}
	  virtual inline unsigned int getFacesDegree() const {return m_faceDegree;}
	  virtual inline std::vector<unsigned int>& getFaces() {return m_faces;}
	  virtual inline std::vector<float>& getFaceNormals() {return m_faceNormals;}

	  virtual void getVNCData(std::vector<float>& result,
				  unsigned int& cardCoord,
				  unsigned int& cardNormal,
				  unsigned int& cardColor);
	  
	  virtual void init();
	  
	private:

	  friend class sd::qtgui::graphicView::GraphicsResourceManager;
	  PolygonMesh(frontend::MeshInfoPtr mesh,
		      const sd::core::RGBCoordinates<float>& defaultColor);
	  
	  template<typename T, unsigned int N>
	  void buildObject(sd::core::MeshView* view);
	  
	private:
	  
	  std::vector<float> m_vertices;
	  std::vector<float> m_vertexColors;
	  std::vector<float> m_vertexNormals;
	  std::vector<float> m_vertexTextures;
	  std::vector<unsigned int> m_faces;
	  std::vector<float> m_faceNormals;
	  // :TODO:J:21/06:12
	  // std::vector<float> m_image; + W & H
	  sd::core::ImageView_<UINT8>* m_image;
	  
	  unsigned int m_faceDegree;
	  
	  sd::core::RGBCoordinates<float> m_defaultColor;
	};

      }
      
    }
    
  }
  
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_MESHRESOURCES_POLYGONMESH_HPP */
