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

#ifndef SD_CORE_SLICE_ARRAY_HXX
#define SD_CORE_SLICE_ARRAY_HXX

#include "iterators/SliceIterator.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    void
    init(std::vector<T*>& vec, size_t& index, T* data, const Slice& s, size_t level, size_t startPos) {
      size_t nbDims = s.nbDims();
      if (level > 0) {
	for (size_t i = 0; i < s.size(nbDims-1-level); ++i) {
	  init(vec, index, data, s, level-1, startPos);
	  startPos += s.stride(nbDims-1-level);
	}
      }
      else {
	for (size_t i = 0; i < s.size(nbDims-1-level); ++i)
	  vec[index++] = &(data[startPos + i * s.stride(nbDims-1-level)]);
      }
    }

    template<typename T>
    SliceArray<T>::SliceArray(T* data, const Slice& s)
      : m_subset(s.size()), m_slice(s) {
      size_t index = 0;
      init<T>(m_subset, index, data, s, s.nbDims()-1, s.start());
    }

    template<typename T>
    SliceArray<T>::SliceArray(const SliceArray<T>& rhs)
      : m_subset(rhs.m_subset), m_slice(rhs.m_slice)
    {}

    template<typename T>
    SliceArray<T>::~SliceArray()
    {}

    template<typename T>
    SliceArray<T>&
    SliceArray<T>::operator=(const SliceArray<T>& rhs) {
      if (&rhs != this) {
	m_subset = rhs.m_subset;
	m_slice = rhs.m_slice;
      }
      return *this;
    }

    // iterators
    template<typename T>
    typename SliceArray<T>::const_iterator
    SliceArray<T>::begin() const {
      return const_iterator(new SliceIterator<T>(m_subset));
    }

    template<typename T>
    typename SliceArray<T>::const_iterator
    SliceArray<T>::end() const {
      return const_iterator(new SliceIterator<T>(std::vector<T*>()));
    }

    template<typename T>
    typename SliceArray<T>::const_reverse_iterator
    SliceArray<T>::rbegin() const {
      return const_reverse_iterator(end());
    }

    template<typename T>
    typename SliceArray<T>::const_reverse_iterator
    SliceArray<T>::rend() const {
      return const_reverse_iterator(begin());
    }

    template<typename T>
    typename SliceArray<T>::iterator
    SliceArray<T>::begin() {
      return iterator(new SliceIterator<T>(m_subset));
    }

    template<typename T>
    typename SliceArray<T>::iterator
    SliceArray<T>::end() {
      return iterator(new SliceIterator<T>(std::vector<T*>()));
    }

    template<typename T>
    typename SliceArray<T>::reverse_iterator
    SliceArray<T>::rbegin() {
      return reverse_iterator(end());
    }

    template<typename T>
    typename SliceArray<T>::reverse_iterator
    SliceArray<T>::rend() {
      return reverse_iterator(begin());
    }

  }
  
}

#endif /*! SD_CORE_SLICE_ARRAY_HXX */
