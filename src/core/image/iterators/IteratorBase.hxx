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

#ifndef SD_CORE_ITERATORS_ITERATOR_BASE_HXX
#define SD_CORE_ITERATORS_ITERATOR_BASE_HXX

#include <cassert>
#include <typeinfo>

namespace sd {
  namespace core {

    template<typename T>
    IteratorBase<T>::IteratorBase()
    {
    }

    template<typename T>
    typename IteratorBase<T>::reference
    IteratorBase<T>::operator()(const difference_type& n) const
    {
      return this->operator[](n);
    }

    template<typename U>
    inline bool
    operator==(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      // check for type compatibility before calling comparision function
      assert(typeid(ib1) == typeid(ib2));
      return (/*typeid(ib1) == typeid(ib2) && */ib1.equal(ib2));
    }
    
    template<typename U>
    inline bool
    operator!=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      return !(ib1 == ib2);
    }
    
    template<typename U>
    inline bool
    operator<(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      return !(ib1 >= ib2);
    }
    
    template<typename U>
    inline bool
    operator>(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      // check for type compatibility before calling comparision function
      assert(typeid(ib1) == typeid(ib2));
      return (/*typeid(ib1) == typeid(ib2) && */ib1.greaterThan(ib2));
    }
    
    template<typename U>
    inline bool
    operator<=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      // check for type compatibility before calling comparision function
      assert(typeid(ib1) == typeid(ib2));
      return (/*typeid(ib1) == typeid(ib2) && */!(ib1.greaterThan(ib2)));
    }
    
    template<typename U>
    inline bool
    operator>=(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      // check for type compatibility before calling comparision function
      assert(typeid(ib1) == typeid(ib2));
      return (/*typeid(ib1) == typeid(ib2) && */(ib1.greaterThan(ib2) || ib1.equal(ib2)));
    }
    
    template<typename U>
    inline typename IteratorBase<U>::difference_type
    operator-(const IteratorBase<U>& ib1, const IteratorBase<U>& ib2)
    {
      // check for type compatibility before calling subtract function
      assert(typeid(ib1) == typeid(ib2));
      return (/*typeid(ib1) == typeid(ib2) ? */ib1.subtract(ib2)/* : 0*/);
    }

  }
}

#endif /*! SD_CORE_ITERATORS_ITERATOR_BASE_HXX */
