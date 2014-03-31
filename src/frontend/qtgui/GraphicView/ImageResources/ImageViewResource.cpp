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

#include <limits>

#include "ImageViewResource.hpp"

#include "../GraphicsResourceManager.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace image_resources {

	ImageViewResource::ImageViewResource(ImageViewResource::Type t, frontend::ImageInfoPtr image)
	  : type(t),
	  imageInfo(image)
	{
	  assert(image);
	}

	ImageViewResource::~ImageViewResource()
	{
	  GraphicsResourceManager::instance().removeRes(this);
	}

	UINT16
	ImageViewResource::getSizeX() const
	{
	  return imageInfo->imagePtr->size().width();
	}

	UINT16
	ImageViewResource::getSizeY() const
	{
	  return imageInfo->imagePtr->size().height();
	}

	UINT16
	ImageViewResource::getSizeZ() const
	{
	  return imageInfo->imagePtr->size().depth();
	}

	float
	ImageViewResource::getResolX() const
	{
	  return imageInfo->imagePtr->resolution().width();
	}

	float
	ImageViewResource::getResolY() const
	{
	  return imageInfo->imagePtr->resolution().height();
	}

	float
	ImageViewResource::getResolZ() const
	{
	  return imageInfo->imagePtr->resolution().depth();
	}

	std::string
	ImageViewResource::getName() const
	{
	  return imageInfo->getName();
	}

      }
    }
  }
}
