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

#ifndef SD_IMAGE_PROCESSING_GAUSSIAN_FREQUENCY_FILTER_HXX
#define SD_IMAGE_PROCESSING_GAUSSIAN_FREQUENCY_FILTER_HXX

#include <algorithm>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    GaussianFrequencyFilter<T>::GaussianFrequencyFilter(size_t w, size_t h, size_t d, double sigma)
      : FrequencyFilter<T>(NULL, w, h, d)
    {
      double A = 1./(2.*M_PI*sigma*sigma);
      for (size_t k = 0, index = 0; k < d; ++k) {
	double z = k-d/2.0;
	for (size_t j = 0; j < h; ++j) {
	  double y = j-h/2.0;
	  for (size_t i = 0; i < w; ++i, ++index) {
	    double x = i-w/2.0;
	    this->kernel()[index] = A*exp(-(x*x + y*y + z*z)/(2.0*sigma*sigma));
	  }
	}
      }
      double maxi = *std::max_element(this->kernel().begin(), this->kernel().end());
      for (auto it = this->kernel().begin(); it != this->kernel().end(); ++it)
	*it /= maxi;
    }

    template<typename T>
    GaussianFrequencyFilter<T>::~GaussianFrequencyFilter()
    {
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_GAUSSIAN_FREQUENCY_FILTER_HXX */
