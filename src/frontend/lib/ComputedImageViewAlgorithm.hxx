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

#ifndef SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HXX
#define SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HXX

namespace sd {
  
  namespace frontend {
    
    /*template<typename T>
    ComputedImageViewAlgorithm<T>::ComputedImageViewAlgorithm(const std::string& algoName,
							      const AlgorithmInfo& in,
							      const ParameterList& parameters)
      : core::Image<T>(core::ImageViewInfo(), algoName),
	Algorithm(make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
								   sd::core::ObjectDescription::ANY_CARD,
								   sd::core::ObjectDescription::ANY_CARD)),
		  in,
		  parameters)
    {}*/

template<typename T>
    ComputedImageViewAlgorithm<T>::ComputedImageViewAlgorithm(const std::string& algoName,
                    const AlgorithmInfo& in,
                    const std::string& parameters)
      : core::Image<T>(core::ImageViewInfo(), algoName),
  Algorithm(make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
                   sd::core::ObjectDescription::ANY_CARD,
                   sd::core::ObjectDescription::ANY_CARD)),
      in,
      parameters)
    {}
    
    template<typename T>
    ComputedImageViewAlgorithm<T>::~ComputedImageViewAlgorithm()
    {}
    
  }
  
}

#endif /*! SD_FRONTEND_COMPUTED_IMAGE_VIEW_ALGORITHM_HXX */
