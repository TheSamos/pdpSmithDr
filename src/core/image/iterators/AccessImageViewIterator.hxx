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

#ifndef SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HXX
#define SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HXX

#include "AccessImageViewIterator.hpp"

#include <iostream>

namespace sd {
  namespace core {

    template<typename T>
    AccessImageViewIterator<T>::AccessImageViewIterator(const sd::core::ImageView_<T>* iv,
							bool goToEnd)
      : _Parent(), m_iv(iv), m_size(iv->size()), m_index(goToEnd ? m_size.dataSize() : 0), m_buff(NULL)
    {m_buff = new T[m_iv->nbChannels()];
      updateValue();}

    template<typename T>
    AccessImageViewIterator<T>::AccessImageViewIterator(const _Self& x)
      : _Parent(), m_iv(x.m_iv), m_size(x.m_size), m_index(x.m_index), m_buff(NULL)
    {m_buff = new T[m_iv->nbChannels()];
      updateValue();}

    template<typename T>
    AccessImageViewIterator<T>::~AccessImageViewIterator()
    {delete [] m_buff;}

    template<typename T>
    AccessImageViewIterator<T>&
    AccessImageViewIterator<T>::operator=(const _Self& x) {
      if (&x != this) {
	delete [] m_buff;
	m_iv = x.m_iv;
	m_size = x.m_size;
	m_index = x.m_index;
	m_buff = new T[m_iv->nbChannels()];
	updateValue();
      }
      return *this;
    }

    template<typename T>
    AccessImageViewIterator<T>*
    AccessImageViewIterator<T>::clone() const {
      return new _Self(*this);
    }

    template<typename T>
    AccessImageViewIterator<T>&
    AccessImageViewIterator<T>::operator++() {
      ++m_index;
      updateValue();
      return *this;
    }

    template<typename T>
    AccessImageViewIterator<T>&
    AccessImageViewIterator<T>::operator--() {
      if (m_index>0)
	--m_index;
      updateValue();
      return *this;
    }

    template<typename T>
    AccessImageViewIterator<T>
    AccessImageViewIterator<T>::operator++(int) {
      AccessImageViewIterator<T> sit(*this);
      ++m_index;
      updateValue();
      return sit;
    }

    template<typename T>
    AccessImageViewIterator<T>
    AccessImageViewIterator<T>::operator--(int) {
      AccessImageViewIterator<T> sit(*this);
      if (m_index>0)
	--m_index;
      updateValue();
      return sit;
    }

    template<typename T>
    AccessImageViewIterator<T>
    AccessImageViewIterator<T>::operator+(const difference_type& n) const {
      AccessImageViewIterator<T> sit(*this);
      sit.m_index += n;
      sit.updateValue();
      return sit;
    }

    template<typename T>
    AccessImageViewIterator<T>&
    AccessImageViewIterator<T>::operator+=(const difference_type& n) {
      m_index += n;
      updateValue();
      return *this;
    }

    template<typename T>
    AccessImageViewIterator<T>
    AccessImageViewIterator<T>::operator-(const difference_type& n) const {
      AccessImageViewIterator<T> sit(*this);
      if ((difference_type) sit.m_index >=n)
	sit.m_index -= n;
      sit.updateValue();
      return sit;
    }

    template<typename T>
    AccessImageViewIterator<T>&
    AccessImageViewIterator<T>::operator-=(const difference_type& n) {
      if ((difference_type) m_index >= n)
	m_index -= n;
      updateValue();
      return *this;
    }

    template<typename T>
    typename AccessImageViewIterator<T>::reference
    AccessImageViewIterator<T>::operator*() const {
      /*Pixel<T> pixel;
      Point_<unsigned int> point;
      assert(m_index < m_size.dataSize());
      m_size.getCoordinatesOf(m_index, point);*/
      return *m_buff;//m_iv->getAt(point);
    }

    template<typename T>
    typename AccessImageViewIterator<T>::pointer
    AccessImageViewIterator<T>::operator->() const {
      /*Pixel<T> pixel;
      Point_<unsigned int> point;
      assert(m_index < m_size.dataSize());
      m_size.getCoordinatesOf(m_index, point);*/
      return m_buff;//m_iv->getAt(point);
    }

    template<typename T>
    typename AccessImageViewIterator<T>::reference
    AccessImageViewIterator<T>::operator[](const difference_type& n) const {
      /*Pixel<T> pixel;
      Point_<unsigned int> point;
      assert(m_index < m_size.dataSize());
      m_size.getCoordinatesOf(m_index, point);*/
      return *(m_buff+n);//m_iv->getAt(point)[n];
    }

    template<typename T>
    bool
    AccessImageViewIterator<T>::equal(const _Parent& x) const {
      const _Self& x2 = static_cast<const _Self&>(x);
      assert(m_iv == x2.m_iv);
      return (m_iv == x2.m_iv
	      && (m_index == x2.m_index || (m_index >= m_size.dataSize()
					   && x2.m_index >= m_size.dataSize())));
    }

    template<typename T>
    bool
    AccessImageViewIterator<T>::greaterThan(const _Parent& x) const {
      const _Self& x2 = static_cast<const _Self&>(x);
      assert(m_iv == x2.m_iv);
      return (m_iv == x2.m_iv && m_index > x2.m_index);
    }

    template<typename T>
    typename AccessImageViewIterator<T>::difference_type
    AccessImageViewIterator<T>::subtract(const _Parent& x) const {
      const _Self& x2 = static_cast<const _Self&>(x);
      assert(m_iv == x2.m_iv);
      return m_index - x2.m_index;
    }

  }
}

#endif /*! SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HXX */
