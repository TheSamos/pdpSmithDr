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

#ifndef SD_IMAGE_PROCESSING_FILTER_HXX
#define SD_IMAGE_PROCESSING_FILTER_HXX

#include <algorithm>
#include <typeinfo>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    Filter<T>::Filter(const double* kern, size_t w, size_t h, size_t d)
      : m_kernel(), m_width(w), m_height(h), m_depth(d)
    {
      size_t n = m_width * m_height * m_depth;
      // :TODO: check if dimensions > 0
      if (kern)
	m_kernel.assign(kern, kern+n);
      else
	m_kernel.resize(n);
    }

    template<typename T>
    Filter<T>::~Filter()
    {
    }

    template<typename T>
    inline bool
    Filter<T>::hasNegativeValues() const
    {
      return std::count_if(m_kernel.begin(), m_kernel.end(), [&](const double& d) { return d<0; });
    }

    template<typename T>
    bool
    isUnsignedType()
    {
      return (typeid(T) == typeid(sd::UINT8) ||
	      typeid(T) == typeid(sd::UINT16) ||
	      typeid(T) == typeid(sd::UINT32)
	      );
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_FILTER_HXX */
