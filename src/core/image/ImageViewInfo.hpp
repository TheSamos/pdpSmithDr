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

#ifndef SD_CORE_IMAGE_VIEW_INFO_HPP
#define SD_CORE_IMAGE_VIEW_INFO_HPP

#include <core/Size.hpp>
#include <core/Point.hpp>

#include <string>
#include <ostream>

namespace sd {
  
  namespace core {

    class ImageViewInfo {

    public:

      ImageViewInfo(ColorSpace cSpace = GRAYLEVEL,
		    UINT8 nbChannels = 1,
		    Orientation orient = Z_AXIS,
		    Size s = Size(),
		    Resolution resol = Resolution(),
		    std::string sUnit = "",
		    std::string tUnit = "");

      // Deduce nbChannels from color space. Provided for convenience.
      ImageViewInfo(ColorSpace cSpace,
		    Orientation orient = Z_AXIS,
		    Size s = Size(),
		    Resolution resol = Resolution(),
		    std::string sUnit = "",
		    std::string tUnit = "");

      // Color space is set "user defined". Provided for convenience.
      ImageViewInfo(UINT8 nbChannels,
		    Orientation orient = Z_AXIS,
		    Size s = Size(),
		    Resolution resol = Resolution(),
		    std::string sUnit = "",
		    std::string tUnit = "");

      ImageViewInfo(const ImageViewInfo& info);
      ~ImageViewInfo();

      ImageViewInfo& operator=(const ImageViewInfo& info);

      inline UINT8 nbChannels() const;
      inline ColorSpace colorSpace() const;
      inline Orientation orientation() const;

      inline sd::Size size() const;
      inline Resolution resolution() const;

      inline const std::string& spaceUnit() const;
      inline const std::string& timeUnit() const;

      friend bool operator==(const ImageViewInfo& info1, const ImageViewInfo& info2);
      friend bool operator!=(const ImageViewInfo& info1, const ImageViewInfo& info2);

      friend std::ostream& operator<<(std::ostream& os, const ImageViewInfo& info);

    private:

      UINT8 m_nbChannels;
      ColorSpace m_colorSpace;
      Orientation m_orientation;

      Size m_size;
      Resolution m_resolution;

      std::string m_spaceUnit;
      std::string m_timeUnit;

    };

  }
}

#include "ImageViewInfo.hxx"

#endif /*! SD_CORE_IMAGE_VIEW_INFO_HPP */
