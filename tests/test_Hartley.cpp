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
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>
#include <core/image/Image.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

using namespace sd::imageprocessing;

void
save(double* data, unsigned int w, unsigned int h, unsigned int d, std::string filename)
{
  unsigned int n = w*h*d;

  double mini = *std::min_element(data, data+n);
  double maxi = *std::max_element(data, data+n);

  sd::UINT8* data8 = new sd::UINT8[n];
  for (unsigned int i = 0; i < n; ++i)
    data8[i] = 255. * (data[i] - mini) / (maxi-mini);

  sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS, sd::Size(w, h, d, 1));
  sd::core::Image<sd::UINT8> im(info);
  im.setData(data8);

  filename += (d == 1 ? ".pgm" : ".pgm3d");
  bool saved = sd::frontend::saveFile((sd::core::ImageView*) &im, filename);
  if (!saved)
    std::cerr << "Unable to save file: " << filename << ".\n\n";
}

void
save(Hartley* ht, std::string filename)
{
  save(ht->getSlice(0), ht->getWidth(), ht->getHeight(), ht->getDepth(), filename);
}

int
main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <image.pgm> <filter size> <output_dir>" << std::endl;
    return EXIT_FAILURE;
  }

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  std::cout << "Load SaverPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");
  
  std::string filename(argv[1]);
  unsigned int fsize = atoi(argv[2]);
  std::string outputDir(argv[3]);

  sd::core::ImageView* image = NULL;
  bool loaded = sd::frontend::loadFile(filename, image);
  if (!loaded || !image) {
    std::cerr << "Unable to load file: " << filename << ".\n\n";
    return EXIT_FAILURE;
  }

  sd::core::ImageView_<sd::UINT8>* im = dynamic_cast<sd::core::ImageView_<sd::UINT8>*>(image);
  if (!im) {
    std::cout << "Only UINT8 data type is accepted. Sorry!\n";
    return EXIT_FAILURE;
  }

  unsigned int width = im->size().width();
  unsigned int height = im->size().height();
  unsigned int nbSlices = im->size().depth();

  // filtre
  double* mask = new double[width*height*nbSlices];
  memset(mask, 0, width*height*nbSlices*sizeof(double));
  int offx = (width >> 1) + 1;
  int offy = (height >> 1) + 1;
  int f0 = fsize >> 1;
  int f1 = fsize-f0-1;
  for (int j = -f0; j <= f1; ++j)
    for (int i = -f0; i <= f1; ++i)
      mask[i+offx + (j+offy)*width] = 1;
  for (unsigned int i = 1; i < nbSlices; ++i)
    memcpy(mask+i*width*height, mask, width*height*sizeof(double));

  Hartley fht_mask(mask, width, height, nbSlices, Hartley::FHT_SPATIAL, 1.5, Hartley::FHT_CENTERED_MASK);
  delete [] mask;

  save(&fht_mask, outputDir+"/mask");

  // FHT filtre
  fht_mask.transform(Hartley::FHT_FORWARD);
  save(&fht_mask, outputDir+"/fht_mask");

  // power spectrum filtre
  double* pspect_mask = fht_mask.powerSpectrumLog();
  save(pspect_mask, fht_mask.getWidth(), fht_mask.getHeight(), fht_mask.getDepth(), outputDir+"/pspect_mask");
  delete [] pspect_mask;


  // image initiale
  //for (unsigned int ind = 0; ind < height*width; ind++) data[ind] /= norm;
  Hartley fht(im->begin().operator->(), width, height, nbSlices, Hartley::FHT_SPATIAL, 1.5, Hartley::FHT_MIRROR);


  fht.transform(Hartley::FHT_FORWARD);
  //save(&fht, outputDir+"/fht");

  double* pspect_img = fht.powerSpectrumLog();
  save(pspect_img, fht.getWidth(), fht.getHeight(), fht.getDepth(), outputDir+"/pspect_img");
  delete [] pspect_img;

  // convolution (multiplication FHT)
  Hartley* conv = fht.multiply(fht_mask, 0);
  //save(conv, outputDir+"/fht_conv");

  double* pspect_conv = conv->powerSpectrumLog();
  save(pspect_conv, conv->getWidth(), conv->getHeight(), conv->getDepth(), outputDir+"/pspect_conv");
  delete [] pspect_conv;

  // FHT inverse
  conv->transform(Hartley::FHT_BACKWARD);
  save(conv, outputDir+"/conv");

  // déconvolution (division FHT)
  conv->transform(Hartley::FHT_FORWARD);
  Hartley* deconv = conv->divide(fht_mask);
  //save(deconv, outputDir+"/fht_deconv");

  double* pspect_deconv = deconv->powerSpectrumLog();
  save(pspect_deconv, deconv->getWidth(), deconv->getHeight(), deconv->getDepth(), outputDir+"/pspect_deconv");
  delete [] pspect_deconv;

  deconv->transform(Hartley::FHT_BACKWARD);
  save(deconv, outputDir+"/deconv");

  delete deconv;
  delete conv;

  delete image;
}
