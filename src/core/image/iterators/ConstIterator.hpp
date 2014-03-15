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

#ifndef SD_CORE_ITERATORS_CONST_ITERATOR_HPP
#define SD_CORE_ITERATORS_CONST_ITERATOR_HPP

#include "Iterator.hpp"

namespace sd {
  namespace core {

    template<typename T>
    class ConstIterator : public Iterator<T>
    {

    public:

      ConstIterator(typename Iterator<T>::Impl* impl = NULL);

      ConstIterator(const ConstIterator<T>& cit);
      ConstIterator(const Iterator<T>& it);

      ~ConstIterator();

      ConstIterator& operator=(const ConstIterator<T>& cit);

    };

  }
}

#include "ConstIterator.hxx"

#endif /*! SD_CORE_ITERATORS_CONST_ITERATOR_HPP */
