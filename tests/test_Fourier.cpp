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

#include <SmithDRDefs.hpp>
#include <Types.hpp>
#include <core/image/ImageView_.hpp>
#include <core/image/Image.hpp>
#include <imageprocessing/Fourier.hpp>
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

void
test1d(const std::string& filename)
{
  // From Matlab
  double Fs = 1000; // Sampling frequency
  double T = 1/Fs; // Sample time
  const int L = 1000; // Length of signal
  double t[L]; // Time vector
  // Sum of a 50 Hz sinusoid and a 120 Hz sinusoid
  double x[L], y[L];
  for (int i = 0; i < L; i++) {
    t[i] = i * T;
    x[i] = 0.7*sin(2*M_PI*50*t[i]) + sin(2*M_PI*120*t[i]);
    double r = rand() / (double) RAND_MAX;
    y[i] = x[i] + r;
  }


  const long NFFT = 1024; // Next power of 2 from length of y
  double Y[NFFT];
  for (int i = 0; i < L; i++) Y[i] = y[i];
  for (int i = L; i < NFFT; i++) Y[i] = 0;

  sd::imageprocessing::Fourier<double> fourier(NFFT);
  double Yre[NFFT];
  double Yim[NFFT];
  fourier.forward(Y, Yre, Yim);

  double Yamp[NFFT];
  double Ypha[NFFT];
  fourier.I2MP(Yre, Yim, Yamp, Ypha);

  double y2[NFFT];
  fourier.backward(Yre, Yim, y2);


  // sauvegarde des données
  char buf[1024];
  FILE *f = fopen(filename.c_str(), "wb");
  for (int i = 0; i < NFFT; i++) {
    if (i < L) {
      sprintf(buf, "%d %f %f %f\n", i, y[i], Yamp[i], y2[i]);
    } else {
      sprintf(buf, "%d %f %f %f\n", i, 0., Yamp[i], y2[i]);
    }
    fputs(buf, f);
  }
  fclose(f);
}

void
testImage(const std::string& filenameIn, const std::string& filenameOut)
{
  sd::core::ImageView* image = NULL;
  bool loaded = sd::frontend::loadFile(filenameIn, image);
  if (!loaded || !image) {
    std::cerr << "Unable to load file: " << filenameIn << ".\n\n";
    return;
  }

  typedef sd::UINT8 Type;
  sd::core::ImageView_<Type>* iv = dynamic_cast<sd::core::ImageView_<Type>*>(image);
  if (!iv) {
    std::cout << "Only UINT8 data type is accepted. Sorry!\n";
    return;
  }

  sd::Size sz = iv->size();
  unsigned int w = sz.width();
  unsigned int h = sz.height();

  unsigned int wh = w * h;
  Type* dataResult = new Type[wh];

  std::vector<double> re(wh);
  std::vector<double> im(wh);
  sd::imageprocessing::Fourier<Type> fourier(wh);

  fourier.forward2d(w, h, iv->begin().operator->(), &(re[0]), &(im[0]), true);
  fourier.backward2d(w, h, &(re[0]), &(im[0]), dataResult, true);

  sd::core::Image<Type> result(iv->information());
  result.setData(dataResult);

  bool saved = sd::frontend::saveFile((sd::core::ImageView*) &result, filenameOut);
  if (!saved)
    std::cerr << "Unable to save file: " << filenameOut << ".\n\n";

  delete iv;
}

int
main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <output.data> <input.pgm> <output.pgm>\n";
    return EXIT_FAILURE;
  }

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  std::cout << "Load SaverPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  test1d(argv[1]);
  testImage(argv[2], argv[3]);
}
