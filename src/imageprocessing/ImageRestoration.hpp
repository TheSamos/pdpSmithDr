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

#ifndef SD_IMAGE_PROCESSING_IMAGE_RESTORATION_HPP
#define SD_IMAGE_PROCESSING_IMAGE_RESTORATION_HPP

#include <core/image/ImageView_.hpp>

namespace sd {
  namespace imageprocessing {

    // available types:
    //   floating points: float, double
    //   integrals: INT8, UINT8, INT16, UINT16, INT32, UINT32
    template<typename T>
    class ImageRestoration
    {

    public :

      ImageRestoration();
      ~ImageRestoration();

      sd::core::ImageView_<double>* pseudoInverseFilter(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, double coeff);

      sd::core::ImageView_<double>*
      inverseFilter(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size)
      {
	return pseudoInverseFilter(iv, spatial_psf, psf_size, 0);
      }

      sd::core::ImageView_<double>* iterativeDeconvolution(const sd::core::ImageView_<T>& iv, double* spatial_psf, size_t psf_size, int nbIter = 5);

    };

  }
}

#endif /* #! SD_IMAGE_PROCESSING_IMAGE_RESTORATION_HPP */
