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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCE_HPP

#include <utility>

#include <Types.hpp>

#include <SmartPointers.hpp>

namespace sd {
  namespace qtgui {
    namespace graphicView {

      // orientation + slice number
      typedef std::pair<Orientation, UINT16> SliceDesc;

      // window width + window level in percent [0,1] of the image min/max stats
      typedef std::pair<float, float> ViewingWindowDesc;

      typedef std::pair<SliceDesc, ViewingWindowDesc> TextureResourceDesc;

      class ImageResource;
      typedef sd::intrusive_ptr<ImageResource> ImageResourcePtr;

      /*
       * ImageSlice
       */
      class ImageSlice
      {

      public:

	const ImageResourcePtr imageResource;
	const TextureResourceDesc description;

      public:

	virtual ~ImageSlice();

	virtual bool use() const = 0;
	virtual void bindTexCoord(double x, double y) const = 0;

      protected:

	ImageSlice(const ImageResourcePtr& myRes, TextureResourceDesc desc);

      private:

	friend inline void
	intrusive_ptr_add_ref(ImageSlice* self)
	{
	  ++self->m_refCounter;
	}

	friend inline void
	intrusive_ptr_release(ImageSlice* self)
	{
	  if (!--self->m_refCounter)
	    delete self;
	}

      private:

	size_t m_refCounter;

      };

      typedef sd::intrusive_ptr<ImageSlice> ImageSlicePtr;

      /*
       * ImageResource
       */
      class ImageResource
      {

      public:

	virtual ~ImageResource();

	virtual ImageSlicePtr getSlice(TextureResourceDesc desc) = 0;

	virtual UINT16 getSizeX() const = 0;
	virtual UINT16 getSizeY() const = 0;
	virtual UINT16 getSizeZ() const = 0;

	virtual float getResolX() const = 0;
	virtual float getResolY() const = 0;
	virtual float getResolZ() const = 0;

	virtual std::string getName() const = 0;

      protected:

	ImageResource();

      private:

	friend inline void
	intrusive_ptr_add_ref(ImageResource* self)
	{
	  ++self->m_refCounter;
	}

	friend inline void
	intrusive_ptr_release(ImageResource* self)
	{
	  if (!--self->m_refCounter)
	    delete self;
	}

      private:

	size_t m_refCounter;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCE_HPP */
