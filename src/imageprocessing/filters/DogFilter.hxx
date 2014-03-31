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

#ifndef SD_IMAGE_PROCESSING_DOG_FILTER_HXX
#define SD_IMAGE_PROCESSING_DOG_FILTER_HXX

#include <cmath>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    DogFilter<T>::DogFilter(size_t w, size_t h, size_t d, double sigma1, double sigma2)
      : LinearFilter<T>(NULL, w, h, d)
    {
      // Note: Assume odd dimensions

      double A1 = 1./(2.*M_PI*sigma1*sigma1);
      double A2 = 1./(2.*M_PI*sigma2*sigma2);
      int w_2 = (w-1) / 2;
      int h_2 = (h-1) / 2;
      int d_2 = (d-1) / 2;
      for (size_t k = 0, index = 0; k < d; ++k) {
	double z = k-d_2;
	for (size_t j = 0; j < h; ++j) {
	  double y = j-h_2;
	  for (size_t i = 0; i < w; ++i, ++index) {
	    double x = i-w_2;
	    double g1 = A1*exp(-(x*x + y*y + z*z)/(2.0*sigma1*sigma1));
	    double g2 = A2*exp(-(x*x + y*y + z*z)/(2.0*sigma2*sigma2));
	    this->kernel()[index] = g1 - g2;
	  }
	}
      }
    }

    template<typename T>
    DogFilter<T>::~DogFilter()
    {
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_DOG_FILTER_HXX */
