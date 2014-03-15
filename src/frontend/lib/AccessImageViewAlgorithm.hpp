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

#ifndef SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HPP
#define SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HPP

#include <SmithDRDefs.hpp>
#include <core/image/ImageView_.hpp>
#include <core/image/ImageViewInfo.hpp>
#include <core/image/SliceArray.hpp>
#include <core/image/iterators/AccessImageViewIterator.hpp>
#include "Algorithm.hpp"
#include "AlgorithmInfo.hpp"
#include "Parameter.hpp"

#include <vector>
#include <string>

namespace sd {
  
  namespace frontend {

    template<typename T>
    class AccessImageViewAlgorithm : public core::ImageView_<T>, public Algorithm {

    public:
      
      virtual inline const core::ImageViewInfo& information() const;

      inline virtual core::Pixel<T> getAt(const sd::Point& p)
      {return ((const AccessImageViewAlgorithm*)this)->getAt(p);}
      
      virtual const core::Pixel<T> getAt(const sd::Point& p) const = 0;
      virtual T getMin() const;
      virtual T getMax() const;

      virtual void setAt(const Point& /*p*/, const core::Pixel<T>& /*val*/) {assert(false);}
      virtual void setMinMax(const T min, const T max);
      
      virtual bool isReadOnly() const;
      
      // iterators
      typedef typename core::ConstIterator<T> const_iterator;
      typedef typename core::Iterator<T> iterator;
      
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      
      virtual const_iterator begin() const;
      virtual const_iterator end() const;
      
      virtual const_reverse_iterator rbegin() const;
      virtual const_reverse_iterator rend() const;
      
      virtual iterator begin();
      virtual iterator end();
      
      virtual reverse_iterator rbegin();
      virtual reverse_iterator rend();
      
    protected:
      
      AccessImageViewAlgorithm(const std::string& name,
			       const AlgorithmInfo& input = AlgorithmInfo(),
			       const ParameterList& parameters = ParameterList());
      
      virtual ~AccessImageViewAlgorithm();
      
      void init(const core::ImageViewInfo& information);
      
    private:
      
      AccessImageViewAlgorithm(const AccessImageViewAlgorithm<T>&) = delete;
      AccessImageViewAlgorithm& operator=(const AccessImageViewAlgorithm<T>&) = delete;
      
    private:
      
      core::ImageViewInfo m_information;
      T m_min;
      T m_max;
      bool m_isComputedMinMax;
      
    };
    
  }
  
}

#include "AccessImageViewAlgorithm.hxx"

#endif /*! SD_FRONTEND_ACCESS_IMAGE_VIEW_ALGORITHM_HPP */
