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

#ifndef SD_CORE_ITERATORS_POINTER_ITERATOR_HPP
#define SD_CORE_ITERATORS_POINTER_ITERATOR_HPP

#include "IteratorBase.hpp"

namespace sd {
  namespace core {

    template<typename T>
    class PointerIterator : public IteratorBase<T>
    {

    private:

      typedef IteratorBase<T> _Parent;
      typedef PointerIterator<T> _Self;

    public:

      typedef typename _Parent::iterator_category iterator_category;
      typedef typename _Parent::value_type value_type;
      typedef typename _Parent::difference_type difference_type;
      typedef typename _Parent::reference reference;
      typedef typename _Parent::pointer pointer;

      PointerIterator(pointer pData, size_t stride = 1);
      virtual ~PointerIterator();

      inline _Self* clone() const;

      inline _Self& operator++();
      inline _Self& operator--();

      inline _Self operator++(int);
      inline _Self operator--(int);

      inline _Self operator+(const difference_type& n) const;
      inline _Self operator-(const difference_type& n) const;

      inline _Self& operator+=(const difference_type& n);
      inline _Self& operator-=(const difference_type& n);

      inline reference operator*() const;
      inline pointer operator->() const;

      // operator() does not consider stride; operator[] does.
      inline reference operator()(const difference_type& n) const;
      inline reference operator[](const difference_type& n) const;

    protected:

      virtual bool equal(const _Parent& x) const;
      virtual bool greaterThan(const _Parent& x) const;
      virtual difference_type subtract(const _Parent& x) const;

    private:

      PointerIterator() = delete;
      PointerIterator(const _Self& x);
      PointerIterator& operator=(const _Self& x);

    private:

      T* m_pData;
      size_t m_stride;

    };

  }
}

#include "PointerIterator.hxx"

#endif /*! SD_CORE_ITERATORS_POINTER_ITERATOR_HPP */
