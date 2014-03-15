
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

#ifndef SD_CORE_IMAGE_HPP
#define SD_CORE_IMAGE_HPP

// C++
#include <string>
// Project
#include "ImageView_.hpp"
#include <Types.hpp>
#include "ImageViewInfo.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Image : public ImageView_<T> {

    public:

      class Slice {
	
      public:
	
	Slice() : m_dim(0), m_sliceNumber(0) {}
	Slice(size_t dim, size_t sliceNumber) : m_dim(dim), m_sliceNumber(sliceNumber) {}
	Slice(const Slice& s) : m_dim(s.m_dim), m_sliceNumber(s.m_sliceNumber) {}
	~Slice() {}
	
	inline size_t dim() const {return m_dim;}
	inline size_t sliceNumber() const {return m_sliceNumber;}
	
      private:
	
	size_t m_dim;
	size_t m_sliceNumber;

      };
      
    public:

      typedef typename ImageView_<T>::const_iterator const_iterator;
      typedef typename ImageView_<T>::iterator iterator;

      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;

    public:

      Image(const ImageViewInfo& info = ImageViewInfo(), const std::string& name="Image");

      template<typename U = T>
      Image(const std::string& name, const ImageView_<U>* iv, const Image::Slice& s,
	    const std::vector<unsigned int>& channels = std::vector<unsigned int>(),
	    const Size& destSize = Size());
      
      template<typename U = T>
      Image(const std::string& name, const ImageView_<U>* iv, Orientation, size_t n,
	    const std::vector<unsigned int>& channels = std::vector<unsigned int>(),
	    const Size& destSize = Size());
      
      virtual ~Image();

      virtual inline const ImageViewInfo& information() const;
      inline void init(const ImageViewInfo& Info);
      
      virtual inline T* getData();
      virtual inline const T* getData() const;
      virtual inline Pixel<T> getAt(const Point& p);
      virtual inline const Pixel<T> getAt(const Point& p) const;
      virtual inline T getMin() const;
      virtual inline T getMax() const;
      
      virtual inline void setData(const T* data);
      virtual inline void setAt(const Point& p, const Pixel<T> & val);
      virtual inline void setMinMax(T min, T max);

      virtual inline bool isReadOnly() const;

      // iterators over image elements (pixels), not over image channels
      // Example: Let I be a RGB image, and itr an iterator on I.
      //          itr[k] points on the k-th pixel
      //          itr[k][1] is the second component (here the Green channel) of the k-th pixel.
      virtual const_iterator begin() const;
      virtual const_iterator end() const;

      virtual const_reverse_iterator rbegin() const;
      virtual const_reverse_iterator rend() const;

      virtual iterator begin();
      virtual iterator end();

      virtual reverse_iterator rbegin();
      virtual reverse_iterator rend();
            
      // slice selectors
      virtual SliceArray<T> getSlice(const sd::core::Slice& s);
      virtual SliceArray<T> getSlice(Orientation, size_t n);
      
    private:
      
      Image(const Image&) = delete;
      Image& operator=(const Image&) = delete;
      
      void computeMinMax() const;

      ImageViewInfo m_info;

      sd::core::Vector<T> m_data;
      mutable T m_min;
      mutable T m_max;
      mutable bool m_isMinMaxComputed;

    };

  }
  
}

#include "Image.hxx"

#endif /*! SD_CORE_IMAGE_HPP */
