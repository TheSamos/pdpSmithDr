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

#ifndef SD_CORE_ITERATORS_ITERATOR_BASE_HPP
#define SD_CORE_ITERATORS_ITERATOR_BASE_HPP

#include <cstddef>

namespace sd {
  namespace core {

    template<typename T>
    class IteratorBase
    {

    public:

      typedef std::random_access_iterator_tag iterator_category;
      typedef T value_type;
      typedef ptrdiff_t difference_type;
      typedef T& reference;
      typedef T* pointer;

    public:

      virtual ~IteratorBase() {}

      virtual IteratorBase<T>* clone() const = 0;

      virtual IteratorBase& operator++() = 0;
      virtual IteratorBase& operator--() = 0;

      virtual IteratorBase& operator+=(const difference_type& n) = 0;
      virtual IteratorBase& operator-=(const difference_type& n) = 0;

      virtual reference operator*() const = 0;
      virtual pointer operator->() const = 0;

      virtual reference operator()(const difference_type& n) const;
      virtual reference operator[](const difference_type& n) const = 0;

      template<typename U>
      friend bool operator==(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend bool operator!=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend bool operator<(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend bool operator>(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend bool operator<=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend bool operator>=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

      template<typename U>
      friend typename IteratorBase<U>::difference_type operator-(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2);

    protected:

      IteratorBase();
      virtual bool equal(const IteratorBase& x) const = 0;
      virtual bool greaterThan(const IteratorBase& x) const = 0;
      virtual difference_type subtract(const IteratorBase& x) const = 0;

    };

  }
}

#include "IteratorBase.hxx"

#endif /*! SD_CORE_ITERATORS_ITERATOR_BASE_HPP */
