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

#ifndef SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HPP
#define SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HPP

#include <SmithDRDefs.hpp>
#include <core/image/Image.hpp>
#include "Algorithm.hpp"
#include "AlgorithmInfo.hpp"
#include "Parameter.hpp"

#include <string>

namespace sd {
  
  namespace frontend {

    template<typename T>
    class ComputedImageViewAlgorithm : public core::Image<T>, public Algorithm {
      
    protected:
      
      ComputedImageViewAlgorithm(const std::string& name,
				 const AlgorithmInfo& input = AlgorithmInfo(),
				 const ParameterList& parameters = ParameterList());
      
      virtual ~ComputedImageViewAlgorithm();
      
    private:

      ComputedImageViewAlgorithm(const ComputedImageViewAlgorithm<T>&) = delete;
      ComputedImageViewAlgorithm& operator=(const ComputedImageViewAlgorithm<T>&) = delete;
      
    };

  }
  
}

#include "ComputedImageViewAlgorithm.hxx"

#endif /*! SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HPP */
