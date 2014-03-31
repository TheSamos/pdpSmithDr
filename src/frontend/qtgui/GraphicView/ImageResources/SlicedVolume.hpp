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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCES_SLICEDVOLUME_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCES_SLICEDVOLUME_HPP

#include <map>

#include <GL/glew.h>

#include "ImageViewResource.hpp"
#include <frontend/lib/TypesInfo.hpp>

namespace sd {
  
  namespace qtgui {
    
    namespace graphicView {

      class GraphicsResourceManager;

      namespace image_resources {

	class SlicedVolumeSlice;
	typedef sd::intrusive_ptr<SlicedVolumeSlice> SlicedVolumeSlicePtr;

	/*
	 * SlicedVolume
	 */
	class SlicedVolume : public ImageViewResource {

	public:

	  friend class SlicedVolumeSlice;

	public:

	  virtual ImageSlicePtr getSlice(TextureResourceDesc desc);

	  UINT16 bindSizeX() const;
	  UINT16 bindSizeY() const;
	  UINT16 bindSizeZ() const;

	private:

	  friend class sd::qtgui::graphicView::GraphicsResourceManager;
	  SlicedVolume(frontend::ImageInfoPtr image);

	  void removeSlice(SliceDesc desc);

	private:

	  UINT16 m_bindSizeX, m_bindSizeY, m_bindSizeZ;
	  double m_scaleX, m_scaleY, m_scaleZ;

	  std::map<SliceDesc, SlicedVolumeSlicePtr> m_slices;

	};


	/*
	 * SlicedVolumeSlice
	 */
	class SlicedVolumeSlice : public ImageViewSlice {

	public:

	  SlicedVolumeSlice(SlicedVolume* res, TextureResourceDesc desc);
	  ~SlicedVolumeSlice();

	  virtual bool use() const;
	  virtual void bindTexCoord(double x, double y) const;

	private:

	  GLuint m_glTexObject;

	};

      }
      
    }
    
  }
  
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGERESOURCES_SLICEDVOLUME_HPP */
