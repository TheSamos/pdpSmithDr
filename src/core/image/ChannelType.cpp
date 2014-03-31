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

#include "ChannelType.hpp"

namespace sd {
  namespace core {

    unsigned int ChannelType_<INT8>::_instanceId = getNewId();
    unsigned int ChannelType_<UINT8>::_instanceId = getNewId();
    unsigned int ChannelType_<INT16>::_instanceId = getNewId();
    unsigned int ChannelType_<UINT16>::_instanceId = getNewId();
    unsigned int ChannelType_<UINT>::_instanceId = getNewId();
    unsigned int ChannelType_<INT>::_instanceId = getNewId();
    unsigned int ChannelType_<float>::_instanceId = getNewId();
    unsigned int ChannelType_<double>::_instanceId = getNewId();

    ChannelType::ChannelType(unsigned int id)
      : DataType(id)
    {
    }

  }
}
