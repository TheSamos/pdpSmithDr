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

#include "SliceArray.hpp"
#include <algorithm>

namespace sd {
  
  namespace core {

    Slice::Slice()
      : m_start(0), m_size(), m_stride()
    {}

    Slice::Slice(size_t startIni, std::initializer_list<size_t> sizeIni, std::initializer_list<size_t> strideIni)
      : m_start(startIni), m_size(sizeIni), m_stride(strideIni)
    {}

    Slice::Slice(size_t startIni, size_t sizeIni, size_t strideIni)
      : m_start(startIni), m_size({sizeIni}), m_stride({strideIni})
    {}

    Slice::Slice(const Slice& rhs)
      : m_start(rhs.m_start), m_size(rhs.m_size), m_stride(rhs.m_stride)
    {}

    Slice::~Slice()
    {}

    const Slice&
    Slice::operator=(const Slice& rhs) {
      if (&rhs != this) {
	m_start = rhs.m_start;
	m_size = rhs.m_size;
	m_stride = rhs.m_stride;
      }
      return *this;
    }

    size_t
    Slice::size() const {
      if (m_size.empty())
	return 0;

      size_t s = 1;
      std::for_each(m_size.begin(), m_size.end(), [&s](size_t n) { s *= n; });

      return s;
    }

  }
  
}
