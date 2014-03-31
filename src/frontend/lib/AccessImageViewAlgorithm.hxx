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

#ifndef SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HXX
#define SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HXX

#include <core/image/iterators/AccessImageViewIterator.hpp>

namespace sd {
  
  namespace frontend {

    /*template<typename T>
    AccessImageViewAlgorithm<T>::AccessImageViewAlgorithm(const std::string& algoName,
							  const AlgorithmInfo& in,
							  const ParameterList& parameters)
      : core::ImageView_<T>(algoName),
	Algorithm(make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
								   sd::core::ObjectDescription::ANY_CARD,
								   sd::core::ObjectDescription::ANY_CARD)),
		  in,
		  parameters),
	m_information(),
	m_min(0),
	m_max(0),
	m_isComputedMinMax(false)
    {}*/

  template<typename T>
    AccessImageViewAlgorithm<T>::AccessImageViewAlgorithm(const std::string& algoName,
                const AlgorithmInfo& in,
                const std::string& parameters)
      : core::ImageView_<T>(algoName),
  Algorithm(make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
                   sd::core::ObjectDescription::ANY_CARD,
                   sd::core::ObjectDescription::ANY_CARD)),
      in,
      parameters),
  m_information(),
  m_min(0),
  m_max(0),
  m_isComputedMinMax(false)
    {}
    
    template<typename T>
    AccessImageViewAlgorithm<T>::~AccessImageViewAlgorithm() {}

    template<typename T>
    const core::ImageViewInfo&
    AccessImageViewAlgorithm<T>::information() const {
      return m_information;
    }

    template<typename T>
    T
    AccessImageViewAlgorithm<T>::getMin() const {
      return m_min;
    }
    
    template<typename T>
    T
    AccessImageViewAlgorithm<T>::getMax() const {
      return m_max;
    }
    
    template<typename T>
    void
    AccessImageViewAlgorithm<T>::setMinMax(T min, T max) {
      m_min = min;
      m_max = max;
    }

    template<typename T>
    bool
    AccessImageViewAlgorithm<T>::isReadOnly() const {
      return true;
    }

    // iterators
    template<typename T>
    typename AccessImageViewAlgorithm<T>::const_iterator
    AccessImageViewAlgorithm<T>::begin() const {
      return iterator(new core::AccessImageViewIterator<T>(this));
    }

    template<typename T>
    typename AccessImageViewAlgorithm<T>::const_iterator
    AccessImageViewAlgorithm<T>::end() const {
      return iterator(new core::AccessImageViewIterator<T>(this, true));
    }

    template<typename T>
    typename AccessImageViewAlgorithm<T>::const_reverse_iterator
    AccessImageViewAlgorithm<T>::rbegin() const {
      return const_reverse_iterator(end());
    }

    template<typename T>
    typename AccessImageViewAlgorithm<T>::const_reverse_iterator
    AccessImageViewAlgorithm<T>::rend() const {
      return const_reverse_iterator(begin());
    }
    
    template<typename T>
    typename AccessImageViewAlgorithm<T>::iterator
    AccessImageViewAlgorithm<T>::begin() {
      return iterator(new core::AccessImageViewIterator<T>(this));
    }
    
    template<typename T>
    typename AccessImageViewAlgorithm<T>::iterator
    AccessImageViewAlgorithm<T>::end() {
      return iterator(new core::AccessImageViewIterator<T>(this, true));
    }
    
    template<typename T>
    typename AccessImageViewAlgorithm<T>::reverse_iterator
    AccessImageViewAlgorithm<T>::rbegin() {
      return reverse_iterator(end());
    }

    template<typename T>
    typename AccessImageViewAlgorithm<T>::reverse_iterator
    AccessImageViewAlgorithm<T>::rend() {
      return reverse_iterator(begin());
    }
    
    template<typename T>
    void
    AccessImageViewAlgorithm<T>::init(const core::ImageViewInfo& information) {
      m_information = information;
    }
    
  }
}

#endif /*! SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HXX */
