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

#ifndef SD_CORE_NORMAL_HPP
#define SD_CORE_NORMAL_HPP

#include <core/Vector3d.hpp>

namespace sd {

  namespace core {
    
    template<typename T>
    class Normal : public Vector3d<typename sd::core::DataType_<T>::work_type> {
      
    public:
      
      typedef typename sd::core::DataType_<T>::work_type type;
      
    public:
      
      virtual ~Normal() {}

      inline Normal(sd::core::Vector<type>* data, unsigned int index, bool deleteOnRemove = false)
	: Vector3d<type>(data, index, deleteOnRemove) {}

      inline Normal(const type& px=0, const type& py=0, const type& pz=0)
	: Vector3d<type>(px, py, pz) {}

      inline Normal(const Normal& n) 
	: Vector3d<type>((sd::core::Vector<type>*)NULL, 0, false) {
	Coordinates<type>::nocopy(n);
      }
      
      inline Normal(const Vector3d<type>& v) : Vector3d<type>(v)
      {}
      
      inline const Normal& operator=(const Normal& n) {
	Coordinates<type>::nocopy(n);
	return *this;
      }
    };

  }

}

#endif /*! SD_CORE_NORMAL_HPP */
