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

#ifndef SD_CORE_ITERATORS_CONST_ITERATOR_HXX
#define SD_CORE_ITERATORS_CONST_ITERATOR_HXX

namespace sd {
  namespace core {

    template<typename T>
    ConstIterator<T>::ConstIterator(typename Iterator<T>::Impl* impl)
      : Iterator<T>(impl)
    {
    }

    template<typename T>
    ConstIterator<T>::ConstIterator(const ConstIterator<T>& cit)
      : Iterator<T>(cit)
    {
    }

    template<typename T>
    ConstIterator<T>::ConstIterator(const Iterator<T>& it)
      : Iterator<T>(it)
    {
    }

    template<typename T>
    ConstIterator<T>::~ConstIterator()
    {
    }

    template<typename T>
    ConstIterator<T>&
    ConstIterator<T>::operator=(const ConstIterator<T>& cit)
    {
      Iterator<T>::operator=(cit);
      return *this;
    }

  }
}

#endif /*! SD_CORE_ITERATORS_CONST_ITERATOR_HXX */
