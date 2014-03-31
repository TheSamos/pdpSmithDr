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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGE_RESOURCE_IMAGE_VIEW_RESOURCE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGE_RESOURCE_IMAGE_VIEW_RESOURCE_HPP

#include "../ImageResource.hpp"
#include <frontend/lib/TypesInfo.hpp>

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {
      
      namespace image_resources {

	/*
	 * ImageViewResource
	 */
	class ImageViewResource : public ImageResource {
	  
	public:
	  
	  enum Type { SlicedVolumeType, VolumeType };
	  
	public:
	  
	  const Type type;
	  const frontend::ImageInfoPtr imageInfo;
	  
	public:
	  
	  virtual ~ImageViewResource();
	  
	  virtual ImageSlicePtr getSlice(TextureResourceDesc desc) = 0;
	  
	  virtual UINT16 getSizeX() const;
	  virtual UINT16 getSizeY() const;
	  virtual UINT16 getSizeZ() const;
	  
	  virtual float getResolX() const;
	  virtual float getResolY() const;
	  virtual float getResolZ() const;
	  
	  virtual std::string getName() const;
	  
	protected:
	  
	  ImageViewResource(ImageViewResource::Type t, frontend::ImageInfoPtr image);

	};
	
	typedef sd::intrusive_ptr<ImageViewResource> ImageViewResourcePtr;
	
	
	/*
	 * ImageViewSlice
	 */
	class ImageViewSlice : public ImageSlice {
	  
	public:
	  
	  ImageViewSlice(ImageViewResource* myRes, TextureResourceDesc desc)
	    : ImageSlice(myRes, desc) {}
	  
	};

	typedef sd::intrusive_ptr<ImageViewSlice> ImageViewSlicePtr;

      }
      
    }
    
  }
  
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGE_RESOURCE_IMAGE_VIEW_RESOURCE_HPP */
