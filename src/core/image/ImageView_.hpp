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

#ifndef SD_CORE_IMAGE_VIEW__HPP
#define SD_CORE_IMAGE_VIEW__HPP

#include "ImageView.hpp"

#include <string>
#include <vector>

#include <Types.hpp>
#include <core/Pixel.hpp>
#include <core/Point.hpp>
#include "iterators/Iterator.hpp"
#include "iterators/ConstIterator.hpp"
#include "SliceArray.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    class ImageView_ : public ImageView {

    public:

      typedef ConstIterator<T> const_iterator;
      typedef Iterator<T> iterator;

      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;

      virtual inline ~ImageView_();

      virtual DataType channelType() const;

      virtual const Pixel<T> getAt(const Point& p) const = 0;
      virtual Pixel<T> getAt(const Point& p) = 0;
      virtual T getMin() const = 0;
      virtual T getMax() const = 0;
      
      virtual void setAt(const Point& p, const Pixel<T>& val) = 0;
      virtual void setMinMax(T min, T max) = 0;
      
      virtual inline bool isReadOnly() const = 0;

      // iterators
      virtual const_iterator begin() const = 0;
      virtual const_iterator end() const = 0;

      virtual const_reverse_iterator rbegin() const = 0;
      virtual const_reverse_iterator rend() const = 0;

      virtual iterator begin() = 0;
      virtual iterator end() = 0;

      virtual reverse_iterator rbegin() = 0;
      virtual reverse_iterator rend() = 0;


      // slice selectors
      template<typename U = T>
      SliceArray<U>* getSlice(const Slice& s,
			      const std::vector<unsigned int>& channels = std::vector<unsigned int>(),
			      const Size& destSize = Size());
      template<typename U = T>
      SliceArray<U>* getSlice(Orientation, size_t n,
			      const std::vector<unsigned int>& channels = std::vector<unsigned int>(),
			      const Size& destSize = Size());

    protected:
      
      ImageView_(const std::string& name = "");
      ImageView_(ViewNode* previousViewNode, const std::string& name = "");
      ImageView_(const std::vector<ViewNode*>& previousViewNodes, const std::string& name);
      
    private:

      ImageView_(const ImageView_<T>&) = delete;
      ImageView_& operator=(const ImageView_<T>&) = delete;

    };

  }
  
}

#include "ImageView_.hxx"

#endif
