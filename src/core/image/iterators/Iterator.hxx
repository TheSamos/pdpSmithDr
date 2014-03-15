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

#ifndef SD_CORE_ITERATORS_ITERATOR_HXX
#define SD_CORE_ITERATORS_ITERATOR_HXX

namespace sd {
  namespace core {

    template<typename T>
    Iterator<T>::Iterator(Impl* impl)
      : m_impl(impl)
    {
    }

    template<typename T>
    Iterator<T>::Iterator(const Iterator<T>& r)
      : m_impl(NULL)
    {
      m_impl = (r.m_impl ? dynamic_cast<Impl*>(r.m_impl->clone()) : NULL);
    }

    template<typename T>
    Iterator<T>::~Iterator()
    {
      if (m_impl)
	delete m_impl;
    }

    template<typename T>
    Iterator<T>&
    Iterator<T>::operator=(const Iterator<T>& r)
    {
      if (&r != this) {
	if (m_impl)
	  delete m_impl;
	m_impl = (r.m_impl ? dynamic_cast<Impl*>(r.m_impl->clone()) : NULL);
      }
      return *this;
    }

    template<typename T>
    Iterator<T>&
    Iterator<T>::operator++()
    {
      ++(*m_impl);
      return *this;
    }

    template<typename T>
    Iterator<T>&
    Iterator<T>::operator--()
    {
      --(*m_impl);
      return *this;
    }

    template<typename T>
    Iterator<T>
    Iterator<T>::operator++(int)
    {
      Iterator it = *this;
      ++(*this);
      return it;
    }

    template<typename T>
    Iterator<T>
    Iterator<T>::operator--(int)
    {
      Iterator it = *this;
      --(*this);
      return it;
    }

    template<typename T>
    Iterator<T>
    Iterator<T>::operator+(const difference_type& n) const
    {
      Iterator it = *this;
      it.m_impl->operator+=(n);
      return it;
    }

    template<typename T>
    Iterator<T>
    Iterator<T>::operator-(const difference_type& n) const
    {
      Iterator it = *this;
      it.m_impl->operator-=(n);
      return it;
    }

    template<typename T>
    Iterator<T>&
    Iterator<T>::operator+=(const difference_type& n)
    {
      (*m_impl) += n;
      return *this;
    }

    template<typename T>
    Iterator<T>&
    Iterator<T>::operator-=(const difference_type& n)
    {
      (*m_impl) -= n;
      return *this;
    }

    template<typename T>
    typename Iterator<T>::reference
    Iterator<T>::operator*() const
    {
      return *(*m_impl);
    }

    template<typename T>
    typename Iterator<T>::pointer
    Iterator<T>::operator->() const
    {
      return m_impl->operator->();
    }

    template<typename T>
    typename Iterator<T>::reference
    Iterator<T>::operator()(const difference_type& n) const
    {
      return (*m_impl)(n);
    }

    template<typename T>
    typename Iterator<T>::reference
    Iterator<T>::operator[](const difference_type& n) const
    {
      return (*m_impl)[n];
    }

    template<typename U>
    inline bool
    operator==(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) == *(i2.m_impl));
    }

    template<typename U>
    inline  bool
    operator!=(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) != *(i2.m_impl));
    }

    template<typename U>
    inline bool
    operator<(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) < *(i2.m_impl));
    }

    template<typename U>
    bool
    inline operator>(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) > *(i2.m_impl));
    }

    template<typename U>
    inline bool
    operator<=(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) <= *(i2.m_impl));
    }

    template<typename U>
    inline bool
    operator>=(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return (*(i1.m_impl) >= *(i2.m_impl));
    }

    template<typename U>
    inline typename Iterator<U>::difference_type
    operator-(const Iterator<U>& i1, const Iterator<U>& i2)
    {
      return *(i1.m_impl) - *(i2.m_impl);
    }

    template<typename U>
    inline Iterator<U>
    operator+(typename Iterator<U>::difference_type n, const Iterator<U>& i1)
    {
      Iterator<U> it = i1;
      return it + n;
    }

  }
}

#endif /*! SD_CORE_ITERATORS_ITERATOR_HXX */
