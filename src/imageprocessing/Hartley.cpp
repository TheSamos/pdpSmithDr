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

#include <Types.hpp>
#include <SmithDRDefs.hpp>
#include "Hartley.hpp"
#include <iostream>
#include <cmath>
#include <cstring>

namespace sd {
  namespace imageprocessing {

    size_t expandedSize(size_t n, double coeff);
    int hartley_mod(int i, int n);
    int mirrorValue(int i, int n);
    void swapQuadrants(size_t w, size_t h, size_t d, double** x);
    void makeSinCosTables(size_t n, double* s, double* c);
    void rc2DFHT(double* x, size_t w, size_t h, double* sw, double* cw, double* sh, double* ch, int* bitRev);
    void dfht3(double* x, int base, int maxN, double* s, double* c, int* bitRev);
    int hartley_btst(int x, int bit);
    int hartley_log2(int x);
    void BitRevRArr(double* x, int base, int bitlen, size_t n, int* bitRev);
    int hartley_bitRevX(size_t x, int bitlen);
    double* hartleyCoefs(int max);
    void slowHT(double* u, double* cas, int max, double* work);
    void pspect1d(double* data, size_t n, int row, double* pspect);
    int powerOf2Size(int w);


    template<typename T>
    Hartley::Hartley(T* data, size_t width, size_t height, size_t depth, fhtDomain domain, double sizeExpansionCoeff, fhtDataInitialization init)
      : m_width(0), m_height(0), m_depth(0), m_data(NULL), m_slices(NULL), m_domain(domain), m_sw(NULL), m_cw(NULL), m_sh(NULL), m_ch(NULL), m_bitRev(NULL)
    {
      // vérifie que les dimensions sont des puissances de 2
      m_width = expandedSize(width, sizeExpansionCoeff);
      m_height = expandedSize(height, sizeExpansionCoeff);
      m_depth = (depth == 1) ? 1 : expandedSize(depth, sizeExpansionCoeff);

      size_t wh = m_width * m_height;
      size_t n = wh * m_depth;
      // copie les données (avec zero-padding si nécessaire)
      m_data = new double[n];
      memset(m_data, 0, n*sizeof(double));
      m_slices = new double*[m_depth];
      for (size_t k = 0; k < m_depth; ++k)
	m_slices[k] = &(m_data[k*wh]);

      if (data != NULL) {
	if (init == FHT_MIRROR) {
	  int kOff = (m_depth - depth + 1)/2;
	  int jOff = (m_height - height + 1)/2;
	  int iOff = (m_width - width + 1)/2;
	  for (int k = -kOff; k < (int) m_depth-kOff; ++k) {
	    int kOut = k + kOff;
	    int kIn = mirrorValue(k, depth) * width*height;
	    for (int j = -jOff; j < (int) m_height-jOff; ++j) {
	      int jOut = (j + jOff) * m_width;
	      int jIn = mirrorValue(j, height) * width;
	      for (int i = -iOff; i < (int) m_width-iOff; ++i) {
		int iOut = i + iOff;
		int iIn = mirrorValue(i, width);
		m_slices[kOut][iOut + jOut] = data[kIn + iIn + jIn];
	      }
	    }
	  }
	}
	else if (init == FHT_CENTERED_MASK) {
	  size_t kOff = (m_depth - depth + 1)/2;
	  size_t jOff = (m_height - height + 1)/2;
	  size_t iOff = (m_width - width + 1)/2;

	  for (size_t k = 0; k < depth; ++k) {
	    int kIn = k*width*height;
	    for (size_t j = 0; j < height; ++j) {
	      int jIn = j*width;
	      int jOut = m_width*(j+jOff);
	      for (size_t i = 0; i < width; ++i)
		m_slices[k+kOff][i+iOff + jOut] = data[kIn + i + jIn];
	    }
	  }
	  swapQuadrants(m_width, m_height, m_depth, m_slices);
	}
	else /*if (init == FHT_RAW_COPY)*/ {
	  for (size_t k = 0, index = 0; k < m_depth; ++k)
	    for (size_t j = 0; j < m_height; ++j) {
	      int jIn = j*width;
	      for (size_t i = 0; i < m_width; ++i, ++index)
		m_data[index] = (i < width && j < height) ? data[i+jIn] : 0;
	    }
	}
      }

      buildTables();
    }

    Hartley::Hartley(const Hartley& fht)
      : m_width(fht.m_width), m_height(fht.m_height), m_depth(fht.m_depth), m_data(NULL), m_slices(NULL), m_domain(fht.m_domain), m_sw(NULL), m_cw(NULL), m_sh(NULL), m_ch(NULL), m_bitRev(NULL)
    {
      if (fht.m_data) {
	size_t wh = m_width * m_height;
	size_t n = wh * m_depth;
	m_data = new double[n];
	memcpy(m_data, fht.m_data, n*sizeof(double));

	m_slices = new double*[m_depth];
	for (size_t k = 0; k < m_depth; ++k)
	  m_slices[k] = &(m_data[k*wh]);
      }

      buildTables();
    }

    Hartley::~Hartley()
    {
      if (m_data)
	delete [] m_data;

      if (m_slices)
	delete [] m_slices;

      delete [] m_sw;
      delete [] m_cw;
      delete [] m_sh;
      delete [] m_ch;

      delete [] m_bitRev;
    }

    Hartley&
    Hartley::operator=(const Hartley& fht)
    {
      if (this != &fht) {
	m_width = fht.m_width;
	m_height = fht.m_height;
	m_depth = fht.m_depth;
	m_data = NULL;
	m_slices = NULL;
	m_domain = fht.m_domain;

	if (fht.m_data) {
	  size_t wh = m_width * m_height;
	  size_t n = wh * m_depth;
	  m_data = new double[n];
	  memcpy(m_data, fht.m_data, n*sizeof(double));

	  m_slices = new double*[m_depth];
	  for (size_t k = 0; k < m_depth; ++k)
	    m_slices[k] = &(m_data[k*wh]);
	}

	buildTables();
      }

      return *this;
    }

    void
    Hartley::buildTables()
    {
      m_sw = new double[m_width>>2];
      m_cw = new double[m_width>>2];
      m_sh = new double[m_height>>2];
      m_ch = new double[m_height>>2];
      makeSinCosTables(m_width, m_sw, m_cw);
      makeSinCosTables(m_height, m_sh, m_ch);

      size_t maxN = m_width;
      if (m_height > maxN)
	maxN = m_height;
      if (m_depth > maxN)
	maxN = m_depth;

      m_bitRev = new int[maxN];
      int nLog2 = hartley_log2(maxN);
      for (size_t i = 0; i < maxN; ++i)
	m_bitRev[i] = hartley_bitRevX(i, nLog2);
    }

    void
    Hartley::transform(fhtDirection direction)
    {
      size_t w = m_width;
      size_t h = m_height;
      size_t d = m_depth;
      double** data = m_slices;
      m_domain = (direction == FHT_BACKWARD ? FHT_SPATIAL : FHT_FREQUENTIAL);

      for (size_t i = 0; i < d; ++i)
	rc2DFHT(data[i], w, h, m_sw, m_cw, m_sh, m_ch, m_bitRev);

      double* u = new double[d];
      if (powerOf2Size(d)) {
	double* s = new double[d>>2];
	double* c = new double[d>>2];
	makeSinCosTables(d, s, c);
	for (size_t k2 = 0; k2 < h; ++k2) {
	  for (size_t k1 = 0; k1 < w; ++k1) {
	    size_t ind = k1 + k2*w;
	    for (size_t k3 = 0; k3 < d; ++k3)
	      u[k3] = data[k3][ind];
	    dfht3(u, 0, d, s, c, m_bitRev);
	    for (size_t k3 = 0; k3 < d; ++k3)
	      data[k3][ind] = u[k3];
	  }
	}
	delete [] c;
	delete [] s;
      }
      else {
	double* cas = hartleyCoefs(d);
	double* work = new double[d];
	for (size_t k2 = 0; k2 < h; ++k2) {
	  size_t ind = k2*w;
	  for (size_t k1 = 0; k1 < w; ++k1, ++ind) {
	    for (size_t k3 = 0; k3 < d; ++k3)
	      u[k3] = data[k3][ind];
	    slowHT(u, cas, d, work);
	    for (size_t k3 = 0; k3 < d; ++k3)
	      data[k3][ind] = u[k3];
	  }
	}
	delete [] work;
	delete [] cas;
      }
      delete [] u;
      //Convert to actual Hartley transform
      for (size_t k3 = 0; k3 <= d/2; ++k3) {
	size_t k3C = (d - k3) % d;
	for (size_t k2 = 0; k2 <= h/2; ++k2) {
	  size_t k2C = (h - k2) % h;
	  for (size_t k1 = 0; k1 <= w/2; ++k1) {
	    size_t k1C = (w - k1) % w;
	    double A = data[k3][k1 + w*k2C];
	    double B = data[k3][k1C + w*k2];
	    double C = data[k3C][k1 + w*k2];
	    double D = data[k3C][k1C + w*k2C];
	    double E = data[k3C][k1 + w*k2C];
	    double F = data[k3C][k1C + w*k2];
	    double G = data[k3][k1 + w*k2];
	    double H = data[k3][k1C + w*k2C];
	    data[k3][k1 + w*k2] = (A+B+C-D)/2;
	    data[k3C][k1 + w*k2] = (E+F+G-H)/2;
	    data[k3][k1 + w*k2C] = (G+H+E-F)/2;
	    data[k3C][k1 + w*k2C] = (C+D+A-B)/2;
	    data[k3][k1C + w*k2] = (H+G+F-E)/2;
	    data[k3C][k1C + w*k2] = (D+C+B-A)/2;
	    data[k3][k1C + w*k2C] = (B+A+D-C)/2;
	    data[k3C][k1C + w*k2C] = (F+E+H-G)/2;
	  }
	}
      }
      if (direction == FHT_BACKWARD) {
	double norm = d*h*w;
	for (size_t k3 = 0; k3 < d; ++k3)
	  for (size_t k2 = 0; k2 < h; ++k2)
	    for (size_t k1 = 0; k1 < w; ++k1)
	      data[k3][k1 + w*k2] /= norm;
      }
    }

    double*
    Hartley::powerSpectrum()
    {
      if (m_domain == FHT_SPATIAL) {
	std::cerr << "powerSpectrum(): FHT must be in frequency domain\n";
	return NULL;
      }

      size_t n = m_width * m_height * m_depth;
      double* pspect = new double[n];
      for (size_t k = 0; k < m_depth; ++k)
	for (size_t row = 0; row < m_height; ++row)
	  pspect1d(m_slices[k], m_width, row, pspect);

      return pspect;
    }

    // Return log-scale normalized data in [0;255]
    double*
    Hartley::powerSpectrumLog() {
      if (m_domain == FHT_SPATIAL) {
	std::cerr << "powerSpectrum(): FHT must be in frequency domain\n";
	return NULL;
      }

      size_t n = m_width * m_height * m_depth;
      double* pspect = new double[n];
      double** pspect_slices = new double* [m_depth];
      double mini = m_data[0];
      double maxi = m_data[0];
      for (size_t k = 0; k < m_depth; ++k) {
	for (size_t row = 0; row < m_height; ++row) {
	  pspect1d(m_slices[k], m_width, row, pspect);
	  int base = row * m_width;
	  for (size_t col = 0; col < m_width; ++col) {
	    double r = pspect[base+col];
	    if (r < mini)
	      mini = r;
	    if (r > maxi)
	      maxi = r;
	  }
	}
	pspect_slices[k] = pspect + k*m_width*m_height;
      }

      swapQuadrants(m_width, m_height, m_depth, pspect_slices);
      mini = (mini < 1.) ? 0. : log(mini);
      maxi = log(maxi);
      double scale = 255. / (maxi-mini);
      double* logData = new double[n];

      for (size_t k = 0, index = 0; k < m_depth; ++k) {
	int base_s = k * m_width * m_height;
	for (size_t row = 0; row < m_height; ++row) {
	  int base_r = row * m_width;
	  for (size_t col = 0; col < m_width; ++col, ++index) {
	    double r = pspect[base_s+base_r+col];
	    r = (r < 1) ? 0. : log(r);
	    logData[index] = (r-mini)*scale;
	  }
	}
      }

      delete [] pspect_slices;
      delete [] pspect;

      return logData;
    }

    Hartley*
    Hartley::multiply(const Hartley& fht2, int conjugate)
    {
      size_t w = m_width;
      size_t h = m_height;
      size_t d = m_depth;
      if (w != fht2.m_width || h != fht2.m_height || d != fht2.m_depth) {
	std::cerr << "hartley_multiply: dismensions mismatch\n";
	return NULL;
      }
      if (m_domain != fht2.m_domain) {
	std::cerr << "hartley_multiply: FHT domains mismatch\n";
	return NULL;
      }

      Hartley* mult = new Hartley((double*)NULL, w, h, d, m_domain, 1., FHT_MIRROR);
      double** f1 = m_slices;
      double** f2 = fht2.m_slices;
      for (size_t slice = 0; slice < d; ++slice) {
	int sliceMod = (d-slice) % d;
	for (size_t row = 0; row < h; ++row) {
	  int rowMod = (h-row) % h;
	  for (size_t col = 0; col < w; ++col) {
	    int colMod = (w-col) % w;
	    size_t p1 = row*w + col;
	    size_t p2 = rowMod*w + colMod;
	    double f2e = (f2[slice][p1] + f2[sliceMod][p2])/2;
	    double f2o = (f2[slice][p1] - f2[sliceMod][p2])/2;
	    mult->m_slices[slice][p1] = (conjugate ? f1[slice][p1]*f2e - f1[sliceMod][p2]*f2o : f1[slice][p1]*f2e + f1[sliceMod][p2]*f2o);
	  }
	}
      }

      return mult;
    }

    Hartley*
    Hartley::divide(const Hartley& fht2)
    {
      size_t w = m_width;
      size_t h = m_height;
      size_t d = m_depth;
      if (w != fht2.m_width || h != fht2.m_height || d != fht2.m_depth) {
	std::cerr << "hartley_divide: dismensions mismatch\n";
	return NULL;
      }
      if (m_domain != fht2.m_domain) {
	std::cerr << "hartley_divide: FHT domains mismatch\n";
	return NULL;
      }

      Hartley* div = new Hartley((double*)NULL, w, h, d, m_domain, 1., FHT_MIRROR);
      double** f1 = m_slices;
      double** f2 = fht2.m_slices;
      for (size_t slice = 0; slice < d; ++slice) {
	int sliceMod = (d-slice) % d;
	for (size_t row = 0; row < h; ++row) {
	  int rowMod = (h-row) % h;
	  for (size_t col = 0; col < w; ++col) {
	    int colMod = (w-col) % w;
	    size_t p1 = row*w + col;
	    size_t p2 = rowMod*w + colMod;
	    double mag = f2[slice][p1]*f2[slice][p1] + f2[sliceMod][p2]*f2[sliceMod][p2];
	    if (mag < 1e-10)
	      mag = 1e-10;
	    double f2e = f2[slice][p1] + f2[sliceMod][p2];
	    double f2o = f2[slice][p1] - f2[sliceMod][p2];
	    div->m_slices[slice][p1] = (f1[slice][p1]*f2e - f1[sliceMod][p2]*f2o)/mag;
	  }
	}
      }

      return div;
    }

    Hartley*
    Hartley::divideGamma(const Hartley& fht2, double gamma, double mag_max)
    {
      size_t w = m_width;
      size_t h = m_height;
      size_t d = m_depth;
      if (w != fht2.m_width || h != fht2.m_height || d != fht2.m_depth) {
	std::cerr << "hartley_divide: dismensions mismatch\n";
	return NULL;
      }
      if (m_domain != fht2.m_domain) {
	std::cerr << "hartley_divide: FHT domains mismatch\n";
	return NULL;
      }

      double gamma_scaled = gamma * mag_max;
      Hartley* div = new Hartley((double*)NULL, w, h, d, m_domain, 1., FHT_MIRROR);
      double** f1 = m_slices;
      double** f2 = fht2.m_slices;
      for (size_t slice = 0; slice < d; ++slice) {
	int sliceMod = (d-slice) % d;
	for (size_t row = 0; row < h; ++row) {
	  int rowMod = (h-row) % h;
	  for (size_t col = 0; col < w; ++col) {
	    int colMod = (w-col) % w;
	    size_t p1 = row*w + col;
	    size_t p2 = rowMod*w + colMod;
	    double f2e = (f2[slice][p1] + f2[sliceMod][p2]) / 2.;
	    double f2o = (f2[slice][p1] - f2[sliceMod][p2]) / 2.;
	    double mag = f2[slice][p1]*f2[slice][p1] + f2[sliceMod][p2]*f2[sliceMod][p2];
	    if (mag < 1e-10)
	      mag = 1e-10;
	    div->m_slices[slice][p1] = (f1[slice][p1]*f2e - f1[sliceMod][p2]*f2o)/(mag+gamma_scaled);
	  }
	}
      }

      return div;
    }


    /* PRIVATE FUNCTIONS */

    size_t
    expandedSize(size_t n, double coeff)
    {
      //Expand this to a power of 2 that is at least coeff* as large, to avoid wrap effects
      //Start with 4 to avoid apparent normalization problems with n = 2
      size_t p = 4;
      if (n > 1) {
	while (p < coeff * n) p <<= 1;
      }
      return p;
    }

    //A version of mod that is periodic for postive and negative i
    int
    hartley_mod(int i, int n)
    {
      return ((i % n) + n) % n;
    }

    int
    mirrorValue(int i, int n)
    {
      int ip = hartley_mod(i, 2*n);
      return (ip < n) ? ip : n - (ip % n) - 1;
    }

    void
    swapQuadrants(size_t w, size_t h, size_t d, double** x)
    {
      size_t wHalf = w>>1;
      size_t hHalf = h>>1;
      size_t dHalf = d>>1;

      //Shift by half of the grid, less one pixel, in each direction
      for (size_t k3 = 0; k3 < dHalf; ++k3) {
	size_t k3P = k3 + dHalf;
	for (size_t k2 = 0; k2 < h; ++k2) {
	  for (size_t k1 = 0; k1 < w; ++k1) {
	    double temp = x[k3][k1 + w*k2];
	    x[k3][k1 + w*k2] = x[k3P][k1 + w*k2];
	    x[k3P][k1 + w*k2] = temp;
	  }
	}
      }
      for (size_t k2 = 0; k2 < hHalf; ++k2) {
	size_t k2P = k2 + hHalf;
	for (size_t k3 = 0; k3 < d; ++k3) {
	  for (size_t k1 = 0; k1 < w; ++k1) {
	    double temp = x[k3][k1 + w*k2];
	    x[k3][k1 + w*k2] = x[k3][k1 + w*k2P];
	    x[k3][k1 + w*k2P] = temp;
	  }
	}
      }
      for (size_t k1 = 0; k1 < wHalf; ++k1) {
	size_t k1P = k1 + wHalf;
	for (size_t k2 = 0; k2 < h; ++k2) {
	  for (size_t k3 = 0; k3 < d; ++k3) {
	    double temp = x[k3][k1 + w*k2];
	    x[k3][k1 + w*k2] = x[k3][k1P + w*k2];
	    x[k3][k1P + w*k2] = temp;
	  }
	}
      }
    }

    void
    makeSinCosTables(size_t n, double* s, double* c)
    {
      size_t p = n>>2;
      double theta = 0.0;
      double dTheta = 2.0 * M_PI / (double) n;
      for (size_t i = 0; i < p; ++i, theta += dTheta) {
	c[i] = cos(theta);
	s[i] = sin(theta);
      }
    }

    // Row-column Fast Hartley Transform
    void
    rc2DFHT(double* x, size_t w, size_t h, double* sw, double* cw, double* sh, double* ch, int* bitRev)
    {
      for (size_t row = 0; row < h; ++row)
	dfht3(x, row*w, w, sw, cw, bitRev);

      double* temp = new double[h];
      for (size_t col = 0; col < w; ++col) {
	for (size_t row = 0; row < h; ++row)
	  temp[row] = x[col + w*row];

	dfht3(temp, 0, h, sh, ch, bitRev);

	for (size_t row = 0; row < h; ++row)
	  x[col + w*row] = temp[row];
      }

      delete [] temp;
    }

    void
    dfht3(double* x, int base, int maxN, double* s, double* c, int* bitRev)
    {
      int nLog2 = hartley_log2(maxN);
      BitRevRArr(x, base, nLog2, maxN, bitRev);	//bitReverse the input array
      int gpSize = 2;     //first & second stages - do radix 4 butterflies once thru
      int numGps = maxN / 4;

      for (int gpNum = 0, gpNum4 = 0; gpNum < numGps; ++gpNum, gpNum4+=4) {
	int Ad1 = gpNum4;
	int Ad2 = Ad1 + 1;
	int Ad3 = Ad1 + gpSize;
	int Ad4 = Ad2 + gpSize;
	double rt1 = x[base+Ad1] + x[base+Ad2];   // a + b
	double rt2 = x[base+Ad1] - x[base+Ad2];   // a - b
	double rt3 = x[base+Ad3] + x[base+Ad4];   // c + d
	double rt4 = x[base+Ad3] - x[base+Ad4];   // c - d
	x[base+Ad1] = rt1 + rt3;      // a + b + (c + d)
	x[base+Ad2] = rt2 + rt4;      // a - b + (c - d)
	x[base+Ad3] = rt1 - rt3;      // a + b - (c + d)
	x[base+Ad4] = rt2 - rt4;      // a - b - (c - d)
      }

      if (nLog2 > 2) {
	// third + stages computed here
	gpSize = 4;
	int numBfs = 2;
	numGps = numGps / 2;
	for (int stage = 2; stage < nLog2; ++stage) {
	  int gpSize2 = (gpSize << 1);
	  int gpSize_2 = (gpSize >> 1);
	  for (int gpNum = 0, incAd0 = 0; gpNum < numGps; ++gpNum, incAd0+=gpSize2) {
	    int Ad0 = incAd0;
	    int Ad1 = Ad0;     // 1st butterfly is different from others - no mults needed
	    int Ad2 = Ad1 + gpSize;
	    int Ad3 = Ad1 + gpSize_2;
	    int Ad4 = Ad3 + gpSize;
	    double rt1 = x[base+Ad1];
	    x[base+Ad1] = x[base+Ad1] + x[base+Ad2];
	    x[base+Ad2] = rt1 - x[base+Ad2];
	    rt1 = x[base+Ad3];
	    x[base+Ad3] = x[base+Ad3] + x[base+Ad4];
	    x[base+Ad4] = rt1 - x[base+Ad4];
	    for (int bfNum = 1, incBfNum = numGps; bfNum < numBfs; ++bfNum, incBfNum+=numGps) {
	      // subsequent BF's dealt with together
	      Ad1 = bfNum + Ad0;
	      Ad2 = Ad1 + gpSize;
	      Ad3 = gpSize - bfNum + Ad0;
	      Ad4 = Ad3 + gpSize;

	      int CSAd = incBfNum;
	      rt1 = x[base+Ad2] * c[CSAd] + x[base+Ad4] * s[CSAd];
	      double rt2 = x[base+Ad4] * c[CSAd] - x[base+Ad2] * s[CSAd];

	      x[base+Ad2] = x[base+Ad1] - rt1;
	      x[base+Ad1] = x[base+Ad1] + rt1;
	      x[base+Ad4] = x[base+Ad3] + rt2;
	      x[base+Ad3] = x[base+Ad3] - rt2;
	    } // end bfNum loop
	  } // end gpNum loop
	  gpSize <<= 1;
	  numBfs <<= 1;
	  numGps >>= 1;
	} // end for all stages
      } // end if Nlog2 > 2
    }

    int
    hartley_btst(int x, int bit)
    {
      return ((x & (1<<bit)) != 0);
    }

    int
    hartley_log2(int x)
    {
      int count = 15;
      while (!hartley_btst(x, count))
	count --;
      return count;
    }

    void
    BitRevRArr(double* x, int base, int /*bitlen*/, size_t n, int* bitRev)
    {
      double* tmp = new double[n];
      for (size_t i = 0; i < n; ++i) {
	//size_t l = hartley_bitRevX(i, bitlen); //i=1, l=32767, bitlen=15
	//tmp[i] = x[base+l];
	tmp[i] = x[base+bitRev[i]];
      }
      for (size_t i = 0; i < n; ++i)
	x[base+i] = tmp[i];
      delete [] tmp;
    }

    int
    hartley_bitRevX(size_t x, int bitlen)
    {
      int temp = 0;
      for (int i = 0; i <= bitlen; ++i)
	if ((x & (1<<i)) != 0)
	  temp |= (1<<(bitlen-i-1));
      return temp & 0x0000ffff;
    }

    double*
    hartleyCoefs(int max)
    {
      double* cas = new double[max*max];
      for (int n = 0, ind = 0; n < max; ++n) {
	for (int k = 0; k < max; ++k) {
	  double arg = (2*M_PI*k*n)/max;
	  cas[ind++] = cos(arg) + sin(arg);
	}
      }
      return cas;
    }

    void
    slowHT(double* u, double* cas, int max, double* work)
    {
      for (int k = 0, ind = 0; k < max; ++k) {
	double sum = 0;
	for (int n = 0; n < max; ++n)
	  sum += u[n]*cas[ind++];
	work[k] = sum;
      }
      for (int k = 0; k < max; ++k)
	u[k] = work[k];
    }

#define SQR(x) (x)*(x)

    void
    pspect1d(double* data, size_t n, int row, double* pspect)
    {
      int base = row * n;
      for (size_t c = 0; c < n; ++c) {
	int l = ((n-row)%n) * n + (n-c)%n;
	pspect[base+c] = (SQR(data[base+c]) + SQR(data[l])) / 2.;
      }
    }

    int
    powerOf2Size(int w)
    {
      int i = 2;
      while (i<w) i <<= 2;
      return (i == w);
    }

#define MAKE_TEMPLATED(T)						\
    template Hartley::Hartley(T*, size_t, size_t, size_t, fhtDomain, double, fhtDataInitialization);


    MAKE_TEMPLATED(float)
    MAKE_TEMPLATED(double)
    MAKE_TEMPLATED(sd::INT8)
    MAKE_TEMPLATED(sd::UINT8)
    MAKE_TEMPLATED(sd::INT16)
    MAKE_TEMPLATED(sd::UINT16)
    MAKE_TEMPLATED(sd::INT32)
    MAKE_TEMPLATED(sd::UINT32)

#undef MAKE_TEMPLATED

  }
}
