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

#ifndef SD_IMAGE_PROCESSING_LOG_FILTER_HX
#define SD_IMAGE_PROCESSING_LOG_FILTER_HXX

#include <cmath>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    LogFilter<T>::LogFilter(size_t w, size_t h, size_t d, double sigma)
      : LinearFilter<T>(NULL, w, h, d)
    {
      // Note: Assume odd dimensions

      double s2 = sigma * sigma;
      double A = 1./(2.*M_PI*s2);
      size_t w_2 = (w-1) / 2;
      size_t h_2 = (h-1) / 2;
      size_t d_2 = (d-1) / 2;
      double sum = 0;
      for (size_t k = 0, index = 0; k < d; ++k) {
	double z = k-d_2;
	for (size_t j = 0; j < h; ++j) {
	  double y = j-h_2;
	  for (size_t i = 0; i < w; ++i, ++index) {
	    double x = i-w_2;
	    double g = A*exp(-(x*x + y*y + z*z)/(2.0*s2));
	    if (i != w_2 || j != h_2 || k != d_2) { // not the central point
	      this->kernel()[index] = (x*x + y*y - 2.0*s2) / (s2*s2) * g;
	      sum += this->kernel()[index];
	    }
	  }
	}
      }
      this->kernel()[w_2 + h_2*w + d_2*w*h] = -sum;
    }

    template<typename T>
    LogFilter<T>::~LogFilter()
    {
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_LOG_FILTER_HXX */
