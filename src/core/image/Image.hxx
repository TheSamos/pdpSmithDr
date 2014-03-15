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

#ifndef SD_CORE_IMAGE_HXX
#define SD_CORE_IMAGE_HXX

#include <cassert>
#include <algorithm>
#include "iterators/PointerIterator.hpp"

namespace sd {
  namespace core {

    template<typename T>
    Image<T>::Image(const ImageViewInfo& imageInfo, const std::string& name)
      : ImageView_<T>(name),
	m_info(imageInfo),
	m_data(),
	m_min(T()),
	m_max(T()),
	m_isMinMaxComputed(false) {
      m_data.resize((this)->size().dataSize() * (this)->nbChannels());
    }
    
    template<typename T, typename U>
    void
    initF(const ImageView_<U>* iv, T*& data, unsigned int nbChannels,
	  const Point& offset, const Size& size, Point& point,
	  const Size& destSize, const std::vector<unsigned int>& channels,
	  const T& min, const T& mean,
	  const size_t dimTarget, const size_t dimDest, const size_t dimSrc,
	  const size_t dataSize) {
      if (dimSrc == 0) {
	for (unsigned int i=offset[dimSrc]; i<size[dimSrc]; ++i) {
	  point.setAt(dimSrc, i);
	  const Pixel<U> p = iv->getAt(point);
	  for (unsigned int j=0; j<channels.size(); ++j) {
	    *data = (T)((p[channels[j]] - min) / mean);
	    ++data;
	  }
	}
      } else {
	for (unsigned int i=offset[dimSrc]; i<size[dimSrc]; ++i) {
	  point.setAt(dimSrc, i);
	  initF(iv, data, nbChannels,
		offset, size, point,
		destSize, channels,
		min, mean,
		dimTarget, (dimTarget==dimSrc) ? dimDest : (dimDest == 0) ? 0 : dimDest-1, dimSrc-1,
		(dimTarget==dimSrc || dimDest == 0) ? dataSize : dataSize/destSize[dimDest]);
	}
      }
      if (dimTarget!=dimSrc)
	data += ((destSize[dimDest]-(size[dimSrc]-offset[dimSrc]))*(dataSize/destSize[dimDest]));
    }
    
    template<typename T, typename U>
    void
    init(const ImageView_<U>* iv, T*& data, unsigned int nbChannels,
	 const Point& offset, const Size& size, Point& point,
	 const Size& destSize, const std::vector<unsigned int>& channels,
	 const size_t dimTarget, const size_t dimDest, const size_t dimSrc,
	 const size_t dataSize) {
      if (dimSrc == 0) {
	for (unsigned int i=offset[dimSrc]; i<size[dimSrc]; ++i) {
	  point.setAt(dimSrc, i);
	  const Pixel<U> p = iv->getAt(point);
	  for (unsigned int j=0; j<channels.size(); ++j) {
	    *data = (T)p[channels[j]];
	    ++data;
	  }
	}
      } else {
	for (unsigned int i=offset[dimSrc]; i<size[dimSrc]; ++i) {
	  point.setAt(dimSrc, i);
	  init(iv, data, nbChannels,
	       offset, size, point,
	       destSize, channels,
	       dimTarget, (dimTarget==dimSrc) ? dimDest : (dimDest == 0) ? 0 : dimDest-1, dimSrc-1,
	       (dimTarget==dimSrc || dimDest == 0) ? dataSize : dataSize/destSize[dimDest]);
	}
      }
      if (dimTarget!=dimSrc)
	data += ((destSize[dimDest]-(size[dimSrc]-offset[dimSrc]))*(dataSize/destSize[dimDest]));
    }
    
    template<typename T, typename U>
    void
    loadSlice(Image<T>& image, const ImageView_<U>* iv, const typename Image<T>::Slice& s,
	      const std::vector<unsigned int>& channels,
	      const Size& destSize) {
      Size size(iv->size());
      Size newSize;
      if (destSize.nbDims() != 0) {
	newSize = destSize;
      } else {
	for (unsigned int i=0; i<size.nbDims(); ++i)
	  if (i != s.dim())
	    newSize.setAt(newSize.nbDims(), size[i]);
      }
      
      ImageViewInfo info = iv->information();
      size.setAt(s.dim(), s.sliceNumber()+1);
      std::vector<unsigned int> usedChannels(channels);
      
      if (usedChannels.size() == 0) {
	usedChannels.resize(info.nbChannels());
	for (unsigned int i=0; i<usedChannels.size(); ++i)
	  usedChannels[i] = i;
      }
      image.init(ImageViewInfo(info.colorSpace(),
			       usedChannels.size(),
			       info.orientation(),
			       newSize,
			       info.resolution(),
			       info.spaceUnit(),
			       info.timeUnit()));
      Point offset;
      if (s.dim()<iv->size().nbDims() || s.sliceNumber() == 0)
	offset.setAt(s.dim(), s.sliceNumber());
      Point point(offset);
      unsigned int nbChannels = info.nbChannels();
      T* data = image.getData();
      
      if (DataType_<T>() == DataType_<float>() || DataType_<T>() == DataType_<double>())
	initF<T, U>(iv, data, nbChannels, offset, size, point,
		    newSize, usedChannels, iv->getMin(), iv->getMax()-iv->getMin(),
		    s.dim(), size.nbDims()-1, size.nbDims()-1, newSize.dataSize()*usedChannels.size());
      else
	init<T, U>(iv, data, nbChannels, offset, size, point,
		   newSize, usedChannels,
		   s.dim(), newSize.nbDims()-1, size.nbDims()-1, newSize.dataSize()*usedChannels.size());
    }
    
    // slice selectors
    template<typename T>
    template<typename U>
    Image<T>::Image(const std::string& name, const ImageView_<U>* iv, const Image<T>::Slice& s,
		    const std::vector<unsigned int>& channels,
		    const Size& destSize)
      : ImageView_<T>((ViewNode*)iv, name),
	m_info(),
	m_data(),
	m_min(T()),
	m_max(T()),
	m_isMinMaxComputed(false) {
      loadSlice<T, U>(*this, &iv, s, channels, destSize);
    }

    // slice selectors
    template<typename T>
    template<typename U>
    sd::core::Image<T>::Image(const std::string& name, const ImageView_<U>* iv,
			      Orientation ort, size_t n,
			      const std::vector<unsigned int>& channels,
			      const Size& destSize)
      : ImageView_<T>((ViewNode*)iv, name),
	m_info(),
	m_data(),
	m_min(T()),
	m_max(T()),
	m_isMinMaxComputed(false) {
      switch (ort) {
      case X_AXIS:
	loadSlice<T, U>(*this, iv, Image<T>::Slice(0, n), channels, destSize);
	return;
	break;
      case Y_AXIS:
	loadSlice<T, U>(*this, iv, Image<T>::Slice(1, n), channels, destSize);
	return;
	break;
      case Z_AXIS:
	loadSlice<T, U>(*this, iv, Image<T>::Slice(2, n), channels, destSize);
	return;
	break;
      case NO_AXIS:
      default:
	assert(!"Should never get here!");
      }
      assert(!"Should never get here!");
    }

    template<typename T>
    Image<T>::~Image()
    {}

    template<typename T>
    const ImageViewInfo&
    Image<T>::information() const {
      return m_info;
    }
    
    template<typename T>
    void
    Image<T>::init(const ImageViewInfo& info) {
      m_info = info;
      m_min = T();
      m_max = T();
      m_isMinMaxComputed = false;
      m_data.resize((this)->size().dataSize() * (this)->nbChannels());
    }
    
    template<typename T>
    T*
    Image<T>::getData() {
      return m_data.data();
    }

    template<typename T>
    const T*
    Image<T>::getData() const {
      return m_data.data();
    }
    
    template<typename T>
    Pixel<T>
    Image<T>::getAt(const Point& p) {
#ifdef DEBUG
      const Size sz = (*this).size();
      for (unsigned int i=sz.nbDims(); i<p.nbDims(); ++i)
	assert(p[i] == 0);
#endif
      return Pixel<T>((sd::core::Vector<T>*)&m_data,
		      (*this).size().getIndexOf(p) * (*this).nbChannels(),
		      (unsigned int) (*this).nbChannels());
    }

    template<typename T>
    const Pixel<T>
    Image<T>::getAt(const Point& p) const {
#ifdef DEBUG
      const Size sz = (*this).size();
      for (unsigned int i=sz.nbDims(); i<p.nbDims(); ++i)
	assert(p[i] == 0);
#endif
      return Pixel<T>((sd::core::Vector<T>*)&m_data,
		      (*this).size().getIndexOf(p) * (*this).nbChannels(),
		      (unsigned int) (*this).nbChannels());
    }
    
    template<typename T>
    T
    Image<T>::getMin() const {
      if (!m_isMinMaxComputed)
	computeMinMax();
      return m_min;
    }

    template<typename T>
    T
    Image<T>::getMax() const {
      if (!m_isMinMaxComputed)
	computeMinMax();
      return m_max;
    }

    template<typename T>
    void
    Image<T>::setData(const T* data) {
      Size sz = this->size();
      m_data.resize(sz.dataSize() * (this)->nbChannels());
      memcpy(m_data.data(), data, m_data.size() * sizeof(T));
      computeMinMax();
    }

    template<typename T>
    void
    Image<T>::setAt(const Point& p, const Pixel<T>& val) {
#ifdef DEBUG
      const Size sz = (*this).size();
      for (unsigned int i=sz.nbDims(); i<p.nbDims(); ++i)
	assert(p[i] == 0);
      assert(val.nbChannels() <= (*this).nbChannels());
#endif
      val.copyTo(&m_data[(*this).size().getIndexOf(p) * (*this).nbChannels()]);
    }

    template<typename T>
    void
    Image<T>::computeMinMax() const {
      if (m_data.size() > 0) {
	auto itBeg = m_data.begin();
	auto itEnd = m_data.end();
	m_min = *std::min_element(itBeg, itEnd);
	m_max = *std::max_element(itBeg, itEnd);
	m_isMinMaxComputed = true;
      }
      else
	m_min = m_max = T();
    }

    template<typename T>
    void
    Image<T>::setMinMax(T min, T max) {
      m_min = min;
      m_max = max;
      m_isMinMaxComputed = true;
    }
    
    template<typename T>
    bool
    Image<T>::isReadOnly() const {
      return false;
    }
    
    // iterators
    template<typename T>
    typename Image<T>::const_iterator
    Image<T>::begin() const {
      return const_iterator(new PointerIterator<T>((T*)m_data.data(), this->nbChannels()));
    }

    template<typename T>
    typename Image<T>::const_iterator
    Image<T>::end() const {
      return const_iterator(new PointerIterator<T>((T*)m_data.data() + this->size().dataSize()*this->nbChannels(),
						   this->nbChannels()));
    }

    template<typename T>
    typename Image<T>::const_reverse_iterator
    Image<T>::rbegin() const {
      return const_reverse_iterator(end());
    }

    template<typename T>
    typename Image<T>::const_reverse_iterator
    Image<T>::rend() const {
      return const_reverse_iterator(begin());
    }

    template<typename T>
    typename Image<T>::iterator
    Image<T>::begin() {
      return iterator(new PointerIterator<T>(m_data.data(), this->nbChannels()));
    }

    template<typename T>
    typename Image<T>::iterator
    Image<T>::end() {
      return iterator(new PointerIterator<T>(m_data.data() + this->size().dataSize()*this->nbChannels(),
					     this->nbChannels()));
    }

    template<typename T>
    typename Image<T>::reverse_iterator
    Image<T>::rbegin() {
      return reverse_iterator(end());
    }

    template<typename T>
    typename Image<T>::reverse_iterator
    Image<T>::rend() {
      return reverse_iterator(begin());
    }
    
    // slice selectors
    template<typename T>
    SliceArray<T>
    Image<T>::getSlice(const sd::core::Slice& s) {
      return SliceArray<T>(m_data.data(), s);
    }

    // slice selectors
    template<typename T>
    SliceArray<T>
    Image<T>::getSlice(Orientation ort, size_t n) {
      size_t sx = this->size().width();
      size_t sy = this->size().height();
      size_t sz = this->size().depth();
      UINT8 nchan = this->nbChannels();

      switch (ort) {
      case X_AXIS:
	if (nchan == 1)
	  return SliceArray<T>(m_data.data(), sd::core::Slice(n, {sz,sy}, {sx*sy,sx}));
	else
	  return SliceArray<T>(m_data.data(), sd::core::Slice(n*nchan, {sz,sy,nchan}, {sx*sy*nchan,sx*nchan,1}));
	break;
      case Y_AXIS:
	return SliceArray<T>(m_data.data(), sd::core::Slice(n*sx*nchan, {sz,sx*nchan}, {sx*sy*nchan,1}));
	break;
      case Z_AXIS:
	return SliceArray<T>(m_data.data(), sd::core::Slice(sx*sy*n*nchan, sx*sy*nchan, 1));
	break;
      case NO_AXIS:
      default:
	assert(!"Should never get here!");
      }

      assert(!"Should never get here!");
      return SliceArray<T>(NULL, sd::core::Slice());
    }
    
  }
}

#endif /*! SD_CORE_IMAGE_HXX */
