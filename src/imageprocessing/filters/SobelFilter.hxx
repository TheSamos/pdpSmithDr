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

#ifndef SD_IMAGE_PROCESSING_SOBEL_FILTER_HXX
#define SD_IMAGE_PROCESSING_SOBEL_FILTER_HXX

namespace sd {
  namespace imageprocessing {

    template<typename T>
    SobelFilter<T>::SobelFilter(size_t size)
      : GradientFilter<T>(NULL, NULL, size)
    {
      assert(size%2); // size must be odd

      int f_2 = (size-1) / 2;
      for (int j = -f_2, idx = 0; j <= f_2; ++j) {
	for (int i = -f_2; i <= f_2; ++i, ++idx) {
	  double x = (i > 0 ? i : -i);
	  double y = (j > 0 ? j : -j);
	  this->kernel()[idx] = (i==0 ? 0 : double(i)/double(x*(x+y)));
	  this->kernel()[idx+size*size] = (j==0 ? 0 : double(j)/double(y*(x+y)));
	}
      }
    }

    template<typename T>
    SobelFilter<T>::~SobelFilter()
    {
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_SOBEL_FILTER_HXX */
