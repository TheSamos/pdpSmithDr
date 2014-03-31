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

#ifndef SD_CORE_IMAGE_VIEW_INFO_HXX
#define SD_CORE_IMAGE_VIEW_INFO_HXX

#include <Types.hpp>

#include <string>
#include <ostream>

namespace sd {
  namespace core {

    inline UINT8
    ImageViewInfo::nbChannels() const
    {
      return m_nbChannels;
    }

    inline ColorSpace
    ImageViewInfo::colorSpace() const
    {
      return m_colorSpace;
    }

    inline Orientation
    ImageViewInfo::orientation() const
    {
      return m_orientation;
    }

    inline Size
    ImageViewInfo::size() const
    {
      return m_size;
    }

    inline Resolution
    ImageViewInfo::resolution() const
    {
      return m_resolution;
    }

    inline const std::string&
    ImageViewInfo::spaceUnit() const
    {
      return m_spaceUnit;
    }

    inline const std::string&
    ImageViewInfo::timeUnit() const
    {
      return m_timeUnit;
    }

  }
}

#endif /*! SD_CORE_IMAGE_VIEW_INFO_HXX */
