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

#ifndef SD_CORE_ITERATORS_ITERATOR_HPP
#define SD_CORE_ITERATORS_ITERATOR_HPP

#include "IteratorBase.hpp"

namespace sd {
  namespace core {

    template<typename T>
    class Iterator
    {

    protected:

      typedef IteratorBase<T> Impl;

    public:

      typedef std::random_access_iterator_tag iterator_category;
      typedef T value_type;
      typedef ptrdiff_t difference_type;
      typedef T& reference;
      typedef T* pointer;

      Iterator(Impl* impl = NULL);

      Iterator(const Iterator<T>& r);

      virtual ~Iterator();

      Iterator& operator=(const Iterator<T>& r);

      inline Iterator& operator++();
      inline Iterator& operator--();

      // :GLITCH: Use carefully: these operators are very time- and memory-expensive! (not recommanded in loops!). Use pre-increment and pre-decrement operators if possible.
      inline Iterator operator++(int);
      inline Iterator operator--(int);

      inline Iterator operator+(const difference_type& n) const;
      inline Iterator operator-(const difference_type& n) const;

      inline Iterator& operator+=(const difference_type& n);
      inline Iterator& operator-=(const difference_type& n);

      inline reference operator*() const;
      inline pointer operator->() const;

      inline reference operator()(const difference_type& n) const;
      inline reference operator[](const difference_type& n) const;

      template<typename U>
      friend bool operator==(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend bool operator!=(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend bool operator<(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend bool operator>(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend bool operator<=(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend bool operator>=(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend typename Iterator<U>::difference_type
      operator-(const Iterator<U>& i1, const Iterator<U>& i2);

      template<typename U>
      friend Iterator<U>
      operator+(typename Iterator<U>::difference_type n, const Iterator<U>& i1);

    private:

      Impl* m_impl;

    };

  }
}

#include "Iterator.hxx"

#endif /*! SD_CORE_ITERATORS_ITERATOR_HPP */
