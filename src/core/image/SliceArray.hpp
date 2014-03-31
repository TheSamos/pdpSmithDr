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

#ifndef SD_CORE_SLICE_ARRAY_HPP
#define SD_CORE_SLICE_ARRAY_HPP

#include <cstdio>
#include <vector>
#include "iterators/Iterator.hpp"
#include "iterators/ConstIterator.hpp"

namespace sd {
  
  namespace core {

    template<typename T> class Image;

    /*
     * Slice
     */
    class Slice
    {

    public:

      Slice();
      Slice(size_t, size_t, size_t); // start, size, stride
      Slice(size_t, std::initializer_list<size_t>, std::initializer_list<size_t>); // start, sizes, strides
      Slice(const Slice&);
      ~Slice();

      const Slice& operator=(const Slice&);

      inline size_t start() const { return m_start; }
      size_t size() const;
      inline size_t size(size_t n) const { return m_size[n]; }
      inline size_t stride(size_t n) const { return m_stride[n]; }

      inline size_t nbDims() const { return m_size.size(); }

    private:

      size_t m_start;
      std::vector<size_t> m_size;
      std::vector<size_t> m_stride;

    };


    /*
     * SliceArray
     */
    template<typename T>
    class SliceArray {

    public:
      
      typedef ConstIterator<T> const_iterator;
      typedef Iterator<T> iterator;
      
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      
      SliceArray(const SliceArray<T>&);
      virtual ~SliceArray();
      
      SliceArray<T>& operator=(const SliceArray<T>&);
      
      // iterators
      virtual const_iterator begin() const;
      virtual const_iterator end() const;
      
      virtual const_reverse_iterator rbegin() const;
      virtual const_reverse_iterator rend() const;
      
      virtual iterator begin();
      virtual iterator end();
      
      virtual reverse_iterator rbegin();
      virtual reverse_iterator rend();
      
    private:
      
      SliceArray() = delete;
      
      friend class Image<T>;
      SliceArray(T*, const Slice&);
      
    private:
      
      std::vector<T*> m_subset; // store pointers to values
      Slice m_slice;
      
    };
    
  }
  
}

#include "SliceArray.hxx"

#endif /*! SD_CORE_SLICE_ARRAY_HPP */
