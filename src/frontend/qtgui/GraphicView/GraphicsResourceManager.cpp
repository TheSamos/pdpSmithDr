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

#include "GraphicsResourceManager.hpp"

#include <limits>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
RESTORE_CONTEXT()

//#include "ImageResources/Volume.hpp"
#include "ImageResources/SlicedVolume.hpp"

#include "MeshResources/PolygonMesh.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {

      GraphicsResourceManager::GraphicsResourceManager()
      {}

      GraphicsResourceManager::~GraphicsResourceManager() {
	// Avoid a race condition when the destructor of GraphicsResourceManager
	// m_xxxResources instances ares called before every xxxResourcePtr
	// are released by their owner, causing segfaults
	// Thank you Qt threads for causing this mess... :/
	for (auto it = m_imageResources.begin(), end = m_imageResources.end(); it != end; ++it) {
	  intrusive_ptr_add_ref(it->second);
	}
	for (auto it = m_meshResources.begin(), end = m_meshResources.end(); it != end; ++it) {
	  intrusive_ptr_add_ref(it->second);
	}
      }

      image_resources::ImageViewResourcePtr
      GraphicsResourceManager::getResourceFor(frontend::ImageInfo* imageInfo) {
	if (!imageInfo)
	  return image_resources::ImageViewResourcePtr();

	auto it = m_imageResources.find(imageInfo);
	if (it != m_imageResources.end())
	  return image_resources::ImageViewResourcePtr(it->second);

	// potentially complex cg memory management goes here
	// ...

	//ImageResource* ptr = new image_resources::Volume(imageInfo.image);
	image_resources::ImageViewResource* ptr = new image_resources::SlicedVolume(frontend::ImageInfoPtr(imageInfo));
	m_imageResources[imageInfo] = ptr;
	return image_resources::ImageViewResourcePtr(ptr);
      }

      image_resources::ImageViewResourcePtr
      GraphicsResourceManager::getResourceFor(frontend::ImageInfoPtr imageInfo) {
	return getResourceFor(imageInfo.get());
      }

      void
      GraphicsResourceManager::removeRes(image_resources::ImageViewResource* res) {
	m_imageResources.erase(res->imageInfo.get());
      }

      mesh_resources::MeshViewResourcePtr
      GraphicsResourceManager::getResourceFor(frontend::MeshInfo* meshInfo) {
	if (!meshInfo)
	  return mesh_resources::MeshViewResourcePtr();

	auto it = m_meshResources.find(meshInfo);
	if (it != m_meshResources.end())
	  return mesh_resources::MeshViewResourcePtr(it->second);

	mesh_resources::MeshViewResource* ptr
	  = new mesh_resources::PolygonMesh(frontend::MeshInfoPtr(meshInfo),
					    sd::core::RGBCoordinates<float>(//1., 1., 0.
									    0./256., 166./256., 227./256. /*defaultOuside*/
									    //138./256., 226./256., 52./256. /*defaultInside*/,
									    //193./256., 12./256., 3./256. /*selection*/
									    ));
	m_meshResources[meshInfo] = ptr;
	return mesh_resources::MeshViewResourcePtr(ptr);
      }

      mesh_resources::MeshViewResourcePtr
      GraphicsResourceManager::getResourceFor(frontend::MeshInfoPtr meshInfo)
      {
	return getResourceFor(meshInfo.get());
      }

      void
      GraphicsResourceManager::removeRes(mesh_resources::MeshViewResource* res)
      {
	m_meshResources.erase(res->meshInfo().get());
      }

    }
  }
}
