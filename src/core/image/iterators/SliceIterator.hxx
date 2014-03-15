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

#ifndef SD_CORE_ITERATORS_SLICE_ITERATOR_HXX
#define SD_CORE_ITERATORS_SLICE_ITERATOR_HXX

#include "SliceIterator.hpp"

#include <iostream>

namespace sd {
  namespace core {

    template<typename T>
    SliceIterator<T>::SliceIterator(const std::vector<pointer>& subset)
      : _Parent(), m_subset(subset), m_index(0)
    {
    }

    template<typename T>
    SliceIterator<T>::SliceIterator(const _Self& x)
      : _Parent(), m_subset(x.m_subset), m_index(x.m_index)
    {
    }

    template<typename T>
    SliceIterator<T>::~SliceIterator()
    {
    }

    template<typename T>
    SliceIterator<T>&
    SliceIterator<T>::operator=(const _Self& x)
    {
      if (&x != this) {
	m_subset = x.m_subset;
	m_index = x.m_index;
      }
      return *this;
    }

    template<typename T>
    SliceIterator<T>*
    SliceIterator<T>::clone() const
    {
      return new _Self(*this);
    }

    template<typename T>
    SliceIterator<T>&
    SliceIterator<T>::operator++()
    {
      if (m_index < m_subset.size())
	++ m_index;
      return *this;
    }

    template<typename T>
    SliceIterator<T>&
    SliceIterator<T>::operator--()
    {
      -- m_index;
      return *this;
    }

    template<typename T>
    SliceIterator<T>
    SliceIterator<T>::operator++(int)
    {
      SliceIterator<T> sit(*this);

      if (m_index < m_subset.size())
	++ m_index;
      return sit;
    }

    template<typename T>
    SliceIterator<T>
    SliceIterator<T>::operator--(int)
    {
      SliceIterator<T> sit(*this);

      -- m_index;
      return sit;
    }

    template<typename T>
    SliceIterator<T>
    SliceIterator<T>::operator+(const difference_type& n) const
    {
      SliceIterator<T> sit(*this);
      if (sit.m_index+n <= sit.m_subset.size())
	sit.m_index += n;
      return sit;
    }

    template<typename T>
    SliceIterator<T>&
    SliceIterator<T>::operator+=(const difference_type& n)
    {
      if (m_index+n <= m_subset.size())
	m_index += n;
      return *this;
    }

    template<typename T>
    SliceIterator<T>
    SliceIterator<T>::operator-(const difference_type& n) const
    {
      SliceIterator<T> sit(*this);
      if (sit.m_index >= n)
	sit.m_index -= n;
      return sit;
    }

    template<typename T>
    SliceIterator<T>&
    SliceIterator<T>::operator-=(const difference_type& n)
    {
      if ((difference_type) m_index >= n)
	m_index -= n;
      return *this;
    }

    template<typename T>
    typename SliceIterator<T>::reference
    SliceIterator<T>::operator*() const
    {
      return *(m_subset[m_index]);
    }

    template<typename T>
    typename SliceIterator<T>::pointer
    SliceIterator<T>::operator->() const
    {
      return m_subset[m_index];
    }

    template<typename T>
    typename SliceIterator<T>::reference
    SliceIterator<T>::operator[](const difference_type& n) const
    {
      return *(m_subset[m_index+n]);
    }

    template<typename T>
    bool
    SliceIterator<T>::equal(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);

      bool atEnd = (m_index >= m_subset.size()); // true id m_subset is empty
      bool x2AtEnd = (x2.m_index >= x2.m_subset.size()); // true id x2.m_subset is empty

      if (atEnd && x2AtEnd)
	return true;

      if (atEnd || x2AtEnd)
	return false;

      return (m_subset[m_index] == x2.m_subset[x2.m_index]);
    }

    template<typename T>
    bool
    SliceIterator<T>::greaterThan(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);

      bool atEnd = (m_index >= m_subset.size()); // true id m_subset is empty
      bool x2AtEnd = (x2.m_index >= x2.m_subset.size()); // true id x2.m_subset is empty

      if (atEnd && x2AtEnd)
	return false;

      if (atEnd || x2AtEnd)
	return atEnd;

      return (m_subset[m_index] > x2.m_subset[x2.m_index]);
    }

    template<typename T>
    typename SliceIterator<T>::difference_type
    SliceIterator<T>::subtract(const _Parent& x) const
    {
      const _Self& x2 = static_cast<const _Self&>(x);
      return m_subset.begin() - x2.m_subset.begin();
    }

  }
}

#endif /*! SD_CORE_ITERATORS_SLICE_ITERATOR_HXX */
