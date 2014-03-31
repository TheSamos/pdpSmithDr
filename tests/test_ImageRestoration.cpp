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
#include <imageprocessing/Hartley.hpp>
#include <imageprocessing/ImageRestoration.hpp>
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>
#include <core/image/Image.hpp>
#include <core/image/ImageView_.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

using namespace sd::imageprocessing;

void
saveImage(sd::core::ImageView_<double> *img, std::string outFilename)
{
  unsigned int width = img->size().width();
  unsigned int height = img->size().height();
  unsigned int nbSlices = img->size().depth();
  int n = width * height * nbSlices;
  double mini = *std::min_element(img->begin(), img->end());
  double maxi = *std::max_element(img->begin(), img->end());

  sd::UINT8 *data = new sd::UINT8[n];
  auto it = img->begin();
  for (int i = 0; it != img->end() && i < n; ++i, ++it)
    data[i] = 255. * (*it - mini) / (maxi-mini);

  sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS,
			       sd::Size(width, height, nbSlices, 1));
  sd::core::Image<sd::UINT8> im(info);
  im.setData(data);

  outFilename += (nbSlices == 1 ? ".pgm" : ".pgm3d");
  bool saved = sd::frontend::saveFile((sd::core::ImageView*) &im, outFilename);
  if (!saved)
    std::cerr << "Unable to save file: " << outFilename << ".\n\n";
}

int
main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <image.pgm> <filter size> <out_dir>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename(argv[1]);
  unsigned int fsize = atoi(argv[2]);
  std::string outDir(argv[3]);

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  std::cout << "Load SaverPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  sd::core::ImageView* image = NULL;
  bool loaded = sd::frontend::loadFile(filename, image);
  if (!loaded || !image) {
    std::cerr << "Unable to load file: " << filename << ".\n\n";
    return EXIT_FAILURE;
  }

  typedef sd::UINT8 Type;
  sd::core::ImageView_<Type>* im = dynamic_cast<sd::core::ImageView_<Type>*>(image);
  if (!im) {
    std::cout << "Only UINT8 data type is accepted. Sorry!\n";
    return EXIT_FAILURE;
  }

  unsigned int nbSlices = im->size().depth();

  // filtre
  double *psf = new double[fsize*fsize*nbSlices];
  for (unsigned int i = 0; i < fsize*fsize*nbSlices; ++i)
    psf[i] = 1;

  ImageRestoration<Type> imgRestor;

  sd::core::ImageView_<double>* ifilt = imgRestor.inverseFilter(*im, psf, fsize);
  saveImage(ifilt, outDir+"/inverseFilter");
  delete ifilt;

  sd::core::ImageView_<double>* pif = imgRestor.pseudoInverseFilter(*im, psf, fsize, 0.2);
  saveImage(pif, outDir+"/pseudoInverseFilter");
  delete pif;

  sd::core::ImageView_<double>* id = imgRestor.iterativeDeconvolution(*im, psf, fsize);
  saveImage(id, outDir+"/iterativeDeconvolution");
  delete id;


  delete [] psf;
  delete image;
}
