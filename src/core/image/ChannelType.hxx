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

#ifndef SD_CORE_CHANNEL_TYPE_HXX
#define SD_CORE_CHANNEL_TYPE_HXX

#include <Types.hpp>

namespace sd {
  namespace core {

    template<typename T>
    class ChannelType_;

#define SD_DEF_CHANTYPE( x, y )				\
    template<>						\
    class ChannelType_<x>				\
      : public ChannelType				\
    {							\
							\
    public:						\
							\
      typedef x type;					\
      typedef y work_type;				\
      ChannelType_(): ChannelType( _instanceId ) {   }	\
							\
    private:						\
							\
      static unsigned int _instanceId;			\
							\
    };

    SD_DEF_CHANTYPE(   INT8,    INT )
    SD_DEF_CHANTYPE(  UINT8,   UINT )
    SD_DEF_CHANTYPE(  INT16,    INT )
    SD_DEF_CHANTYPE( UINT16,   UINT )
    SD_DEF_CHANTYPE(  INT32,    INT )
    SD_DEF_CHANTYPE( UINT32,   UINT )
    SD_DEF_CHANTYPE(  float, double )
    SD_DEF_CHANTYPE( double, double )

#undef SD_DEF_CHANTYPE

    inline bool
    operator==(const ChannelType& t1, const ChannelType& t2)
    {
      return (const DataType &)t1 == (const DataType &)t2;
    }

    inline bool
    operator!=( const ChannelType & t1, const ChannelType & t2 )
    {
      return (const DataType &)t1 != (const DataType &)t2;
    }

    template<typename T>
    bool
    ChannelType::isOfType() const
    {
      return *this == ChannelType_<T>();
    }

  }
}

#endif /*! SD_CORE_CHANNEL_TYPE_HXX */

