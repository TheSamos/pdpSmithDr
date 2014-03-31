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

#include <core/image/Image.hpp>
#include "SlicedVolume.hpp"
#include <SmartPointers.hpp>

#include <cassert>
#include <cmath>
#include <vector>

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace image_resources {

	/*
	 * SlicedVolume
	 */
	SlicedVolume::SlicedVolume(const frontend::ImageInfoPtr image)
	  : ImageViewResource(SlicedVolumeType, image) {
	  UINT16 sizeX = getSizeX();
	  UINT16 sizeY = getSizeY();
	  UINT16 sizeZ = getSizeZ();
	  m_bindSizeX = (UINT16) pow(2, ceil(log2(sizeX)));
	  m_bindSizeY = (UINT16) pow(2, ceil(log2(sizeY)));
	  m_bindSizeZ = (UINT16) pow(2, ceil(log2(sizeZ)));
	  m_scaleX = sizeX / (float) m_bindSizeX;
	  m_scaleY = sizeY / (float) m_bindSizeY;
	  m_scaleZ = sizeZ / (float) m_bindSizeZ;
	}

	UINT16
	SlicedVolume::bindSizeX() const {
	  return m_bindSizeX;
	}

	UINT16
	SlicedVolume::bindSizeY() const {
	  return m_bindSizeY;
	}

	UINT16
	SlicedVolume::bindSizeZ() const {
	  return m_bindSizeZ;
	}

	ImageSlicePtr
	SlicedVolume::getSlice(TextureResourceDesc desc) {
	  auto it = m_slices.find(desc.first);
	  if (it != m_slices.end()) {
	    return it->second;
	  }
	  else {
	    // do not increment in the manager to avoid memory leak
	    SlicedVolumeSlicePtr ret(new SlicedVolumeSlice(this, desc), false);
	    m_slices[desc.first] = ret;
	    return ret;
	  }
	}

	void
	SlicedVolume::removeSlice(SliceDesc desc) {
	  m_slices.erase(desc);
	}


	/*
	 * SlicedVolumeSlice
	 */

	// typename V is used to handle the case of double
	// (because glTexImage2D does not accept double)
	// migh imply precision loss (double->float conversion)
	template<typename T, typename V>
	void
	setTexture(core::ImageView_<T>* iv, SliceDesc desc, size_t /*xMax*/,
		   size_t bindSzX, size_t bindSzY, GLenum type) {
	  bool isRGB = (iv->colorSpace() == sd::RGB);
	  bool isRGBA = (iv->colorSpace() == sd::RGBA);
	  std::vector<unsigned int> channels = {0, 1, 2, 3};
	  if (!(isRGBA))
	    channels.resize(3);
	  if (!(isRGB || isRGBA))
	    channels.resize(1);
	  sd::core::Image<V> slice("Slice", iv, desc.first, desc.second-1, channels, Size(bindSzX, bindSzY));
	  
	  // :TODO: consider visualization of user defined types (4d visualization along channels)
	  const V* newData = slice.getData();
	  // :TODO: should consider other formats (third and seventh parameters)
	  if (isRGB) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			 bindSzX, bindSzY,
			 0, GL_RGB, type,
			 newData);
	  }
	  else if (isRGBA) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			 bindSzX, bindSzY,
			 0, GL_RGBA, type,
			 newData);
	  }
	  else {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
			 bindSzX, bindSzY,
			 0, GL_LUMINANCE, type,
			 newData);
	    }
	  //delete slice;
	}

	template<typename T, typename V>
	void
	initSliceTexture(SlicedVolume* res, SliceDesc desc, GLenum type) {
	  auto image = res->imageInfo->imagePtr;
	  auto iv = dynamic_cast<core::ImageView_<T>*>(image.get());

	  switch (desc.first) {
	  case sd::X_AXIS:
	    setTexture<T,V>(iv, desc, iv->size().height(), res->bindSizeY(), res->bindSizeZ(), type);
	    break;
	  case sd::Y_AXIS:
	    setTexture<T,V>(iv, desc, iv->size().width(), res->bindSizeX(), res->bindSizeZ(), type);
	    break;
	  case sd::Z_AXIS:
	    setTexture<T,V>(iv, desc, iv->size().width(), res->bindSizeX(), res->bindSizeY(), type);
	    break;
	  case sd::NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  };
	}

	// SubstType: substitution type for double (not handle by
	// glTexImage2D; using float instead)
	// #define CALL_INIT_SLICE_TEXTURE_IN_OUT(Type, GlType, SubstType)
#define CALL_INIT_SLICE_TEXTURE(Type, GlType, SubstType)	\
	if (image->channelType().isOfType<Type>()) {		\
	  initSliceTexture<Type, SubstType>(res, desc, GlType); \
	}
	/*

#define CALL_INIT_SLICE_TEXTURE(Type, GlType, SubstType)		\
	if (image->channelType().isOfType<Type>()) {		\
	  CALL_INIT_SLICE_TEXTURE_IN_OUT(UINT8, Type, GlType, SubstType) \
	  else CALL_INIT_SLICE_TEXTURE_IN_OUT(INT8, Type, GlType, SubstType) \
	    else CALL_INIT_SLICE_TEXTURE_IN_OUT(UINT16, Type, GlType, SubstType) \
	      else CALL_INIT_SLICE_TEXTURE_IN_OUT(INT16, Type, GlType, SubstType) \
		else CALL_INIT_SLICE_TEXTURE_IN_OUT(UINT32, Type, GlType, SubstType) \
		  else CALL_INIT_SLICE_TEXTURE_IN_OUT(INT32, Type, GlType, SubstType) \
		    else CALL_INIT_SLICE_TEXTURE_IN_OUT(float, Type, GlType, SubstType) \
		      else CALL_INIT_SLICE_TEXTURE_IN_OUT(double, Type, GlType, SubstType) \
			     }
	*/

	SlicedVolumeSlice::SlicedVolumeSlice(SlicedVolume* res, TextureResourceDesc texDesc)
	  : ImageViewSlice(res, texDesc) {
	  SliceDesc desc = texDesc.first;
	  
	  glEnable(GL_TEXTURE_2D);
	  glGenTextures(1, &m_glTexObject);
	  glBindTexture(GL_TEXTURE_2D, m_glTexObject);

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	  float maxAniso;
	  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	  auto image = res->imageInfo->imagePtr;

	  CALL_INIT_SLICE_TEXTURE(UINT8, GL_UNSIGNED_BYTE, UINT8)
	  else CALL_INIT_SLICE_TEXTURE(INT8, GL_BYTE, INT8)
	  else CALL_INIT_SLICE_TEXTURE(UINT16, GL_UNSIGNED_SHORT, UINT16)
	  else CALL_INIT_SLICE_TEXTURE(INT16, GL_SHORT, INT16)
	  else CALL_INIT_SLICE_TEXTURE(UINT32, GL_UNSIGNED_INT, UINT32)
	  else CALL_INIT_SLICE_TEXTURE(INT32, GL_INT, INT32)
	  else CALL_INIT_SLICE_TEXTURE(float, GL_FLOAT, float)
	  //else CALL_INIT_SLICE_TEXTURE(double, GL_DOUBLE, double)
	  else CALL_INIT_SLICE_TEXTURE(double, GL_FLOAT, float)

	  glBindTexture(GL_TEXTURE_2D, 0);
	}

#undef CALL_INIT_SLICE_TEXTURE
#undef CALL_INIT_SLICE_TEXTURE_IN_OUT

	SlicedVolumeSlice::~SlicedVolumeSlice() {
	  glDeleteTextures(1, &m_glTexObject);
	  ((SlicedVolume*)imageResource.get())->removeSlice(description.first);
	}

	bool
	SlicedVolumeSlice::use() const {
	  glColor3f(1.0f, 1.0f, 1.0f);   // avoid colorisation of texture
	  glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, m_glTexObject);
	  return true;
	}

	void
	SlicedVolumeSlice::bindTexCoord(double x, double y) const {
	  double coords[2] = {0};
	  SlicedVolume* res = static_pointer_cast<SlicedVolume>(imageResource).get();
	  switch (description.first.first) {
	  case X_AXIS: {
	    coords[0] = res->m_scaleY * x;
	    coords[1] = res->m_scaleZ * y;
	  }
	    break;
	  case Y_AXIS: {
	    coords[0] = res->m_scaleX * x;
	    // Subtract 1/max to map inside [0, 1[, not ]0, 1], while inverting
	    //coords[1] = m_scaleY * (1.0f - coord - 1.0f/max);
	    coords[1] = res->m_scaleZ * y;
	  }
	    break;
	  case Z_AXIS: {
	    coords[0] = res->m_scaleX * x;
	    coords[1] = res->m_scaleY * y;
	    //coords[2] = m_scaleZ * coord;
	  }
	    break;
	  case NO_AXIS:
	  default:
	    assert(!"Should never get here!");
	  }
	  glTexCoord2dv(coords);
	}

      }
    }
  }
}
