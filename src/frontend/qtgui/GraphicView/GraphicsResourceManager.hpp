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

#ifndef SD_QTGUI_GRAPHIC_VIEW_GRAPHICSRESOURCEMANAGER_HPP
#define SD_QTGUI_GRAPHIC_VIEW_GRAPHICSRESOURCEMANAGER_HPP

#include <map>

#include <frontend/lib/TypesInfo.hpp>
#include "ImageResources/ImageViewResource.hpp"
//#include "ImageResources/DynamicImageResource.hpp"

#include "MeshResources/MeshViewResource.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class GraphicsResourceManager
      {

      public:

	static inline GraphicsResourceManager&
	instance()
	{
	  static GraphicsResourceManager inst;
	  return inst;
	}

	image_resources::ImageViewResourcePtr getResourceFor(frontend::ImageInfo* imageInfo);
	image_resources::ImageViewResourcePtr getResourceFor(frontend::ImageInfoPtr imageInfo);

	mesh_resources::MeshViewResourcePtr getResourceFor(frontend::MeshInfo* meshInfo);
	mesh_resources::MeshViewResourcePtr getResourceFor(frontend::MeshInfoPtr meshInfo);

      private:

	GraphicsResourceManager();
	~GraphicsResourceManager();
	GraphicsResourceManager(const GraphicsResourceManager&) = delete;
	GraphicsResourceManager& operator=(const GraphicsResourceManager&) = delete;

	friend class image_resources::ImageViewResource;
	void removeRes(image_resources::ImageViewResource* self);

	friend class mesh_resources::MeshViewResource;
	void removeRes(mesh_resources::MeshViewResource* self);

      private:

	typedef std::map<frontend::ImageInfo*, image_resources::ImageViewResource*> ImageInfoToResourceMap;
	typedef std::map<frontend::MeshInfo*, mesh_resources::MeshViewResource*> MeshInfoToResourceMap;

	ImageInfoToResourceMap m_imageResources;
	MeshInfoToResourceMap m_meshResources;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_GRAPHICSRESOURCEMANAGER_HPP */
