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

#include "ImageViewInfo.hpp"

#include <cassert>

using namespace sd::core;

ImageViewInfo::ImageViewInfo(ColorSpace cSpace,
			     UINT8 nbChans,
			     Orientation orient,
			     Size s,
			     Resolution resol,
			     std::string sUnit,
			     std::string tUnit)
  : m_nbChannels(nbChans),
    m_colorSpace(cSpace),
    m_orientation(orient),
    m_size(s),
    m_resolution(resol),
    m_spaceUnit(sUnit),
    m_timeUnit(tUnit)
{
  assert(m_nbChannels > 0);
}

ImageViewInfo::ImageViewInfo(ColorSpace cSpace,
			     Orientation orient,
			     Size s,
			     Resolution resol,
			     std::string sUnit,
			     std::string tUnit)
  : m_nbChannels(0),
    m_colorSpace(cSpace),
    m_orientation(orient),
    m_size(s),
    m_resolution(resol),
    m_spaceUnit(sUnit),
    m_timeUnit(tUnit)
{
  if (m_colorSpace == sd::BITMAP || m_colorSpace == sd::GRAYLEVEL)
    m_nbChannels = 1;
  else if (m_colorSpace == sd::RGB)
    m_nbChannels = 3;
  else if (m_colorSpace == sd::RGBA)
    m_nbChannels = 4;

  assert(m_nbChannels > 0);
}

ImageViewInfo::ImageViewInfo(UINT8 nbChans,
			     Orientation orient,
			     Size s,
			     Resolution resol,
			     std::string sUnit,
			     std::string tUnit)
  : m_nbChannels(nbChans),
    m_colorSpace(sd::USER_DEFINED),
    m_orientation(orient),
    m_size(s),
    m_resolution(resol),
    m_spaceUnit(sUnit),
    m_timeUnit(tUnit)
{
  assert(m_nbChannels > 0);
}

ImageViewInfo::ImageViewInfo(const ImageViewInfo& info)
  : m_nbChannels(info.m_nbChannels),
    m_colorSpace(info.m_colorSpace),
    m_orientation(info.m_orientation),
    m_size(info.m_size),
    m_resolution(info.m_resolution),
    m_spaceUnit(info.m_spaceUnit),
    m_timeUnit(info.m_timeUnit)
{
}

ImageViewInfo::~ImageViewInfo()
{
}

ImageViewInfo&
ImageViewInfo::operator=(const ImageViewInfo& info)
{
  if (&info != this) {
    m_nbChannels = info.m_nbChannels;
    m_colorSpace = info.m_colorSpace;
    m_orientation = info.m_orientation;
    m_size = info.m_size;
    m_resolution = info.m_resolution;
    m_spaceUnit = info.m_spaceUnit;
    m_timeUnit = info.m_timeUnit;
  }
  return *this;
}

namespace sd {
  namespace core {

    bool
    operator==(const ImageViewInfo& info1, const ImageViewInfo& info2)
    {
      return (info1.m_nbChannels == info2.m_nbChannels &&
	      info1.m_colorSpace == info2.m_colorSpace &&
	      info1.m_orientation == info2.m_orientation &&
	      info1.m_size == info2.m_size &&
	      info1.m_resolution == info2.m_resolution &&
	      info1.m_spaceUnit == info2.m_spaceUnit &&
	      info1.m_timeUnit == info2.m_timeUnit);
    }

    bool
    operator!=(const ImageViewInfo& info1, const ImageViewInfo& info2)
    {
      return !(info1 == info2);
    }

    std::ostream&
    operator<<(std::ostream& os, const ImageViewInfo& info)
    {
      os << "Image View Info:\n";
      os << "\tNb channels: " << (int) info.m_nbChannels << std::endl;
      os << "\tColor space: " << info.m_colorSpace << std::endl;
      os << "\tOrientation: " << info.m_orientation << std::endl;
      os << "\tSize: " << info.m_size << std::endl;
      os << "\tResolution: " << info.m_resolution << std::endl;
      os << "\tSpace unit: " << info.m_spaceUnit << std::endl;
      os << "\tTime unit: " << info.m_timeUnit << std::endl;
      return os;
    }

  }
}
