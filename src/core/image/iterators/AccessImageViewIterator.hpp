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

#ifndef SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HPP
#define SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HPP

#include "IteratorBase.hpp"
#include <vector>

namespace sd {
  
  namespace core {
    
    template<typename T>
    class AccessImageViewIterator : public IteratorBase<T> {
      
    private:
      
      typedef IteratorBase<T> _Parent;
      typedef AccessImageViewIterator<T> _Self;
      
    public:
      
      typedef typename _Parent::iterator_category iterator_category;
      typedef typename _Parent::value_type value_type;
      typedef typename _Parent::difference_type difference_type;
      typedef typename _Parent::reference reference;
      typedef typename _Parent::pointer pointer;
      
      AccessImageViewIterator(const sd::core::ImageView_<T>* iv, bool goToEnd = false);
      virtual ~AccessImageViewIterator();
      
      _Self* clone() const;
      
      virtual _Self& operator++();
      virtual _Self& operator--();
      
      virtual _Self operator++(int);
      virtual _Self operator--(int);
      
      virtual _Self operator+(const difference_type& n) const;
      virtual _Self operator-(const difference_type& n) const;
      
      virtual _Self& operator+=(const difference_type& n);
      virtual _Self& operator-=(const difference_type& n);
      
      virtual reference operator*() const;
      virtual pointer operator->() const;
      
      virtual reference operator[](const difference_type& n) const;
      
    protected:
      
      AccessImageViewIterator() = delete;
      AccessImageViewIterator(const _Self& x);
      AccessImageViewIterator& operator=(const _Self& x);
      
      virtual bool equal(const _Parent& x) const;
      virtual bool greaterThan(const _Parent& x) const;
      virtual difference_type subtract(const _Parent& x) const;
      
    private:

      void updateValue() {
	Point_<unsigned int> point;
	if (!(m_index < m_size.dataSize())) return;
	m_size.getCoordinatesOf(m_index, point);
	m_iv->getAt(point).copyTo(m_buff);
      }
      
    private:
      
      const sd::core::ImageView_<T>* m_iv;
      sd::core::Size_<unsigned int> m_size;
      size_t m_index;
      T* m_buff;
      
    };
    
  }
  
}

#include "AccessImageViewIterator.hxx"

#endif /*! SD_CORE_ITERATORS_ACCESS_IMAGE_VIEW_ITERATOR_HPP */
