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

#ifndef SD_IMAGE_PROCESSING_KIRSCH_FILTER_HXX
#define SD_IMAGE_PROCESSING_KIRSCH_FILTER_HXX

namespace sd {
  namespace imageprocessing {

    const double KirschH[] = { -3, -3, -3, -3, 0, -3, 5, 5, 5 };
    const double KirschV[] = { -3, -3, 5, -3, 0, 5, -3, -3, 5 };

    template<typename T>
    KirschFilter<T>::KirschFilter()
      : GradientFilter<T>(KirschH, KirschV, 3)
    {
    }

    template<typename T>
    KirschFilter<T>::~KirschFilter()
    {
    }

  }
}

#endif /* #! SD_IMAGE_PROCESSING_KIRSCH_FILTER_HXX */
