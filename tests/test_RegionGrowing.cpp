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
#include <imageprocessing/RegionGrowing.hpp>
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
saveAs(sd::core::ImageView* iv, const std::string& filename, bool binary)
{
  bool saved = sd::frontend::saveFile(iv, filename, binary);
  if (!saved)
    std::cerr << "Unable to save file: " << filename << ".\n\n";
}

int
main(int argc, char *argv[])
{
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0] << " <image.pgm> <2d,3d> <tolerance> <connexity_max?> <out_dir>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename(argv[1]);
  std::string dim(argv[2]);
  float tolerance = atof(argv[3]);
  bool connexityMax = (std::string(argv[4]) == "true");
  std::string outDir(argv[5]);

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

  sd::core::Image<sd::UINT8>* result = new sd::core::Image<sd::UINT8>(image->information());
  sd::UINT8 valOn = 255;
  sd::UINT8 valOff = 0;

  sd::Size sz = image->size();
  sd::UINT8* resultData = new sd::UINT8[sz.dataSize()];
  std::fill(resultData, resultData+sz.dataSize(), valOff);
  result->setData(resultData);
  result->setMinMax(std::min(valOff, valOn),
		    std::max(valOff, valOn));

  RegionGrowing<Type> rg;
  sd::Point seed(image->size().width()/2,
		 image->size().height()/2,
		 image->size().depth()/2);

  if (dim == "2d") {
    sd::imageprocessing::FloodFill2dSet ffset;
    rg.floodFill2d(*im, seed, tolerance, tolerance, ffset, connexityMax);
    for (auto itr = ffset.begin(); itr != ffset.end(); ++itr) {
      sd::UINT16 x = std::get<0>(*itr);
      sd::UINT16 y = std::get<1>(*itr);
      sd::Point pt(x, y, seed.z());
      result->setAt(pt, valOn);
    }
  }
  else { // dim == "3d"
    sd::imageprocessing::FloodFill3dSet ffset;
    rg.floodFill3d(*im, seed, tolerance, tolerance, ffset, connexityMax);
    for (auto itr = ffset.begin(); itr != ffset.end(); ++itr) {
      sd::UINT16 x = std::get<0>(*itr);
      sd::UINT16 y = std::get<1>(*itr);
      sd::UINT16 z = std::get<2>(*itr);
      sd::Point pt(x, y, z);
      result->setAt(pt, valOn);
    }
  }

  saveAs(result, outDir+"/mask.pgm", true);
  delete result;

  delete image;
}
