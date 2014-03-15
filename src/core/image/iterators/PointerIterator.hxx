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

#ifndef SD_CORE_ITERATORS_POINTER_ITERATOR_HXX
#define SD_CORE_ITERATORS_POINTER_ITERATOR_HXX

#include "PointerIterator.hpp"

namespace sd {
  namespace core {

    template<typename T>
    PointerIterator<T>::PointerIterator(const _Self& x)
      : _Parent(), m_pData(x.m_pData), m_stride(x.m_stride)
    {
    }

    template<typename T>
    PointerIterator<T>::PointerIterator(pointer pData, size_t stride)
      : _Parent(), m_pData(pData), m_stride(stride)
    {
    }

    template<typename T>
    PointerIterator<T>::~PointerIterator()
    {
    }

    template<typename T>
    PointerIterator<T>&
    PointerIterator<T>::operator=(const _Self& x)
    {
      if (&x != this) {
	m_pData = x.m_pData;
	m_stride = x.m_stride;
      }
      return *this;
    }

    template<typename T>
    PointerIterator<T>*
    PointerIterator<T>::clone() const
    {
      return new _Self(*this);
    }

    template<typename T>
    PointerIterator<T>&
    PointerIterator<T>::operator++()
    {
      m_pData += m_stride;
      return *this;
    }

    template<typename T>
    PointerIterator<T>&
    PointerIterator<T>::operator--()
    {
      m_pData -= m_stride;
      return *this;
    }

    template<typename T>
    PointerIterator<T>
    PointerIterator<T>::operator++(int)
    {
      return _Self(m_pData+=m_stride);
    }

    template<typename T>
    PointerIterator<T>
    PointerIterator<T>::operator--(int)
    {
      return _Self(m_pData-=m_stride);
    }

    template<typename T>
    PointerIterator<T>
    PointerIterator<T>::operator+(const difference_type& n) const
    {
      return _Self(m_pData + m_stride*n);
    }

    template<typename T>
    PointerIterator<T>&
    PointerIterator<T>::operator+=(const difference_type& n)
    {
      m_pData += m_stride*n;
      return *this;
    }

    template<typename T>
    PointerIterator<T>
    PointerIterator<T>::operator-(const difference_type& n) const
    {
      return _Self(pointer(m_pData - m_stride*n));
    }

    template<typename T>
    PointerIterator<T>&
    PointerIterator<T>::operator-=(const difference_type& n)
    {
      m_pData -= m_stride*n;
      return *this;
    }

    template<typename T>
    typename PointerIterator<T>::reference
    PointerIterator<T>::operator*() const
    {
      return *m_pData;
    }

    template<typename T>
    typename PointerIterator<T>::pointer
    PointerIterator<T>::operator->() const
    {
      return m_pData;
    }

    template<typename T>
    typename PointerIterator<T>::reference
    PointerIterator<T>::operator()(const difference_type& n) const
    {
      return m_pData[n];
    }

    template<typename T>
    typename PointerIterator<T>::reference
    PointerIterator<T>::operator[](const difference_type& n) const
    {
      return m_pData[m_stride*n];
    }

    template<typename T>
    bool
    PointerIterator<T>::equal(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);
      return (m_pData == x2.m_pData);
    }

    template<typename T>
    bool
    PointerIterator<T>::greaterThan(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);
      return (m_pData > x2.m_pData);
    }

    template<typename T>
    typename PointerIterator<T>::difference_type
    PointerIterator<T>::subtract(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);
      return m_pData - x2.m_pData;
    }

  }
}

#endif /*! SD_CORE_ITERATORS_POINTER_ITERATOR_HXX */
