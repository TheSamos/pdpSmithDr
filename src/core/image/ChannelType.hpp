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

#ifndef SD_CORE_CHANNEL_TYPE_HPP
#define SD_CORE_CHANNEL_TYPE_HPP

#include "../DataType.hpp"

namespace sd {
  namespace core {

    class ChannelType
      : public DataType
    {

    public:

      template<typename T> bool isOfType() const;

    protected:

      ChannelType(unsigned int id);

    };

    bool operator==(const ChannelType& t1, const ChannelType& t2);
    bool operator!=(const ChannelType& t1, const ChannelType& t2);

  }
}

#include "ChannelType.hxx"

#endif
