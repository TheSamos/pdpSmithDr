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

#ifndef SD_IMAGE_PROCESSING_FOURIER_HPP
#define SD_IMAGE_PROCESSING_FOURIER_HPP

#include <fftw3.h>

namespace sd {
  namespace imageprocessing {

    // available types:
    //   floating points: float, double
    //   integrals: INT8, UINT8, INT16, UINT16, INT32, UINT32
    template<typename T>
    class Fourier
    {

    public :

      Fourier(size_t length);
      ~Fourier();

      void forward(const T* in, double* reOut, double* imOut, bool center = false) const;
      void forward2d(size_t width, size_t length, const T* in, double* reOut, double* imOut, bool center = false) const;

      void backward(const double* reIn, const double* imIn, T* out, bool centered = false) const;
      void backward2d(size_t width, size_t length, const double* reIn, const double* imIn, T* out, bool centered = false) const;

      void I2MP(const double* reIn, const double* imIn, double* magOut, double* phaseOut) const;

      void MP2I(const double* magIn, const double* phaseIn, double* reOut, double* imOut) const;

      // out = in*out (complex numbers multiplication)
      void multiply(const double* reIn, const double* imIn, double* reOut, double* imOut) const;

    private:

      Fourier(const Fourier&) = delete;
      Fourier& operator=(const Fourier&) = delete;

    private:

      fftw_complex* m_spatialRepr;
      fftw_complex* m_frequencyRepr;
      size_t m_length;

    };

  }
}

#endif /* #! SD_IMAGE_PROCESSING_FOURIER_HPP */
