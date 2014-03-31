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

#ifndef SD_CORE_IMAGE_VIEW__HXX
#define SD_CORE_IMAGE_VIEW__HXX

#include <typeinfo>

namespace sd {
  
  namespace core {
    
    template<typename T>
    inline ImageView_<T>::ImageView_(const std::string& name)
      : ImageView(NULL, name)
    {}

    template<typename T>
    inline ImageView_<T>::ImageView_(ViewNode* previousView, const std::string& name)
      : ImageView(previousView, name)
    {}

    template<typename T>
    inline ImageView_<T>::ImageView_(const std::vector<ViewNode*>& previousViews, const std::string& name)
      : ImageView(previousViews, name)
    {}
      
    template<typename T>
    inline ImageView_<T>::~ImageView_() {}
    
    template<typename T>
    inline DataType
    ImageView_<T>::channelType() const {
      return DataType_<T>();
    }

    // slice selectors
    template<typename T>
    template<typename U>
    SliceArray<U>*
    ImageView_<T>::getSlice(const Slice& s,
			    const std::vector<unsigned int>& channels,
			    const Size& destSize) {
      return new SliceArray<U>(*this, s, channels, destSize);
    }

    // slice selectors
    template<typename T>
    template<typename U>
    SliceArray<U>*
    ImageView_<T>::getSlice(Orientation ort, size_t n,
			    const std::vector<unsigned int>& channels,
			    const Size& destSize) {
      switch (ort) {
      case X_AXIS:
	return new SliceArray<U>(*this, Slice(0, n), channels, destSize);
	break;
      case Y_AXIS:
	return new SliceArray<U>(*this, Slice(1, n), channels, destSize);
	break;
      case Z_AXIS:
	return new SliceArray<U>(*this, Slice(2, n), channels, destSize);
	break;
      case NO_AXIS:
      default:
	assert(!"Should never get here!");
      }

      assert(!"Should never get here!");
      return new SliceArray<U>(*this, Slice(), channels);
    }

  }
  
}

#endif /*! SD_CORE_IMAGE_VIEW__HXX */
