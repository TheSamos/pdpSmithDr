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

#ifndef SD_CORE_IMAGE_DATA_TYPE_HXX
#define SD_CORE_IMAGE_DATA_TYPE_HXX

namespace sd {
  namespace core {

    ImageDataType::ImageDataType(DataType chanType,
				 UINT8 nbChan,
				 ColorSpace cSpace)
      : m_channelType(chanType),
	m_nbChannel(nbChan),
	m_colorSpace(cSpace)
    {
    }

    const DataType&
    ImageDataType::channelType()
    {
      return m_channelType;
    }

    UINT8
    ImageDataType::nbChannel()
    {
      return m_nbChannel;
    }

    ColorSpace
    ImageDataType::colorSpace()
    {
      return m_colorSpace;
    }

    bool
    operator==(ImageDataType itdp1, ImageDataType itdp2)
    {
      return (itdp1.colorSpace() != USER_DEFINED && 
	      itdp2.colorSpace() != USER_DEFINED &&
	      itdp1.channelType() == itdp2.channelType() &&
	      itdp1.nbChannel() == itdp2.nbChannel() && 
	      itdp1.colorSpace() == itdp2.colorSpace());
    }

    bool
    operator!=(ImageDataType itdp1, ImageDataType itdp2)
    {
      return itdp1 != itdp2;
    }

  }
}

#endif /*! SD_CORE_IMAGE_DATA_TYPE_HXX */
