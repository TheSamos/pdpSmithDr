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

#ifndef SD_IMAGE_PROCESSING_HARTLEY_HPP
#define SD_IMAGE_PROCESSING_HARTLEY_HPP

// Implementation of the Fast Hartley Transform (FHT)

#include <cassert>
#include <cstdio>

namespace sd {
  namespace imageprocessing {

    class Hartley
    {

    public :

      typedef enum { FHT_SPATIAL = 1, FHT_FREQUENTIAL } fhtDomain;
      typedef enum { FHT_FORWARD = +1, FHT_BACKWARD = -1 } fhtDirection;
      typedef enum { FHT_CENTERED_MASK = 1, FHT_MIRROR, FHT_RAW_COPY } fhtDataInitialization;

      // available types:
      //   floating points: float, double
      //   integrals: INT8, UINT8, INT16, UINT16, INT32, UINT32
      template<typename T>
      Hartley(T* data, size_t width, size_t height, size_t depth, fhtDomain domain, double sizeExpansionCoeff = 1.0, fhtDataInitialization init = FHT_RAW_COPY);

      Hartley(const Hartley& fht);
      ~Hartley();

      inline size_t getWidth() { return m_width; }
      inline size_t getHeight() { return m_height; }
      inline size_t getDepth() { return m_depth; }

      inline double* getSlice(size_t i) { assert(i < m_depth); return m_slices[i]; }

      Hartley& operator=(const Hartley& fht);

      void transform(fhtDirection direction);

      double* powerSpectrum();
      // Return log-scale normalized data in [0;255]
      double* powerSpectrumLog();

      Hartley* multiply(const Hartley& fht2, int conjugate);
      Hartley* divide(const Hartley& fht2);
      Hartley* divideGamma(const Hartley& fht2, double gamma, double mag_max);

    private:

      void buildTables();

    private:

      size_t m_width, m_height, m_depth; // chaque dimension est une puissance de 2
      double* m_data; // trié par colonne, puis ligne, puis coupe
      double** m_slices; // pointeurs sur les débuts de coupes dans data
      fhtDomain m_domain;

      double *m_sw, *m_cw, *m_sh, *m_ch; // cos and sin tables
      int* m_bitRev; // bit reverse table

    };

  }
}

#endif /* #! SD_IMAGE_PROCESSING_HARTLEY_HPP */
