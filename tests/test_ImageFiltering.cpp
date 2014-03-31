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
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>
#include <core/image/Image.hpp>
#include <core/image/ImageView_.hpp>
#include <imageprocessing/filters/Filter.hpp>
#include <imageprocessing/filters/StructuralElement.hpp>
#include <imageprocessing/filters/GaussianLinearFilter.hpp>
#include <imageprocessing/filters/DogFilter.hpp>
#include <imageprocessing/filters/LogFilter.hpp>
#include <imageprocessing/filters/UniformFilter.hpp>
#include <imageprocessing/filters/MedianFilter.hpp>
#include <imageprocessing/filters/PrewittFilter.hpp>
#include <imageprocessing/filters/RobertsFilter.hpp>
#include <imageprocessing/filters/SobelFilter.hpp>
#include <imageprocessing/filters/KirschFilter.hpp>
#include <imageprocessing/filters/DilationFilter.hpp>
#include <imageprocessing/filters/ErosionFilter.hpp>
#include <imageprocessing/filters/GaussianFrequencyFilter.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>

using namespace sd::imageprocessing;

template<typename T>
void
saveImage(sd::core::ImageView_<T> *img, std::string outFilename)
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

  bool saved = sd::frontend::saveFile((sd::core::ImageView*) &im, outFilename);
  if (!saved)
    std::cerr << "Unable to save file: " << outFilename << ".\n\n";
}

void
usage(std::string progName)
{
  std::cerr << "\nUsage: " << progName << " <image.pgm> <output.pgm> <filter> [options]\n";
  std::cerr << "\tFilters:\n\n";

  std::cerr << "\t\t<kirsch,prewitt,roberts>\n";
  std::cerr << "\t\t\tAll are 3x3 filters\n\n";

  std::cerr << "\t\t<uniform,median,sobel> <kernel_size>\n\n";

  std::cerr << "\t\t<log,gaussian> <kernel_size> <sigma>\n";
  std::cerr << "\t\t\tlog is Lapacian of Gaussian\n";
  std::cerr << "\t\t\tgaussian is applied in spatial domain\n\n";

  std::cerr << "\t\tgaussian <sigma>\n";
  std::cerr << "\t\t\tgaussian is applied in frequency domain\n\n";

  std::cerr << "\t\tdog <kernel_size> <sigma1> <sigma2>\n";
  std::cerr << "\t\t\tdog is Difference of Gaussian\n\n";

  std::cerr << "\t\t<dilation,erosion,closing,opening> <structual_element> [params]\n";
  std::cerr << "\t\tStructural elements:\n";
  std::cerr << "\t\t\t<square> <side>\n";
  std::cerr << "\t\t\t<circle> <radius>\n";
  std::cerr << "\t\t\t<cross> <side> <isDiagonal>\n";

  std::cerr << std::endl;
}

int
main(int argc, char *argv[])
{
  if (argc < 4) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  std::string filenameIn(argv[1]);
  std::string filenameOut(argv[2]);
  std::string filterName(argv[3]);

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  std::cout << "Load SaverPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  sd::core::ImageView* image = NULL;
  bool loaded = sd::frontend::loadFile(filenameIn, image);
  if (!loaded || !image) {
    std::cerr << "Unable to load file: " << filenameIn << ".\n\n";
    return EXIT_FAILURE;
  }

  typedef sd::UINT8 Type;
  sd::core::ImageView_<Type>* im = dynamic_cast<sd::core::ImageView_<Type>*>(image);
  if (!im) {
    std::cout << "Only UINT8 data type is accepted. Sorry!\n";
    return EXIT_FAILURE;
  }

  sd::UINT16 width = im->size().width();
  sd::UINT16 height = im->size().height();
  Filter<Type>* filter = NULL;
  StructuralElement* sel = NULL;

  if (filterName == "kirsch") {
    filter = new KirschFilter<Type>;
  }
  else if (filterName == "prewitt") {
    filter = new PrewittFilter<Type>;
  }
  else if (filterName == "roberts") {
    filter = new RobertsFilter<Type>;
  }
  else if (filterName == "uniform" ||
	   filterName == "median" ||
	   filterName == "sobel") {
    if (argc != 5) {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
    unsigned int fsize = atoi(argv[4]);
    if (filterName == "uniform")
      filter = new UniformFilter<Type>(fsize, fsize, 1);
    else if (filterName == "median")
      filter = new MedianFilter<Type>(fsize, fsize, 1);
    else if (filterName == "sobel")
      filter = new SobelFilter<Type>(fsize);
  }
  else if (filterName == "log") {
    if (argc != 6) {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
    unsigned int fsize = atoi(argv[4]);
    double sigma = atof(argv[5]);
    filter = new LogFilter<Type>(fsize, fsize, 1, sigma);
  }
  else if (filterName == "gaussian") {
    if (argc == 5) {
      double sigma = atof(argv[4]);
      filter = new GaussianFrequencyFilter<Type>(width, height, 1, sigma);
    }
    else if (argc == 6) {
      unsigned int fsize = atoi(argv[4]);
      double sigma = atof(argv[5]);
      filter = new GaussianLinearFilter<Type>(fsize, fsize, 1, sigma);
    }
    else {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }
  else if (filterName == "dog") {
    if (argc != 7) {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
    unsigned int fsize = atoi(argv[4]);
    double sigma1 = atof(argv[5]);
    double sigma2 = atof(argv[6]);
    filter = new DogFilter<Type>(fsize, fsize, 1, sigma1, sigma2);
  }
  else if (filterName == "dilation" ||
	   filterName == "erosion" ||
	   filterName == "closing" ||
	   filterName == "opening") {
    if (argc < 6) {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
    std::string selName(argv[4]);
    unsigned int fsize = atoi(argv[5]);
    if (selName == "square")
      sel = StructuralElement::buildSquare(fsize);
    else if (selName == "circle")
      sel = StructuralElement::buildCircle(fsize);
    else if (selName == "cross") {
      if (argc != 7) {
	usage(argv[0]);
	return EXIT_FAILURE;
      }
      unsigned int diagonal = atoi(argv[6]);
      sel = StructuralElement::buildCross(fsize, diagonal);
    }
    else  {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
    if (filterName == "dilation" || filterName == "closing")
      filter = new DilationFilter<Type>(*sel);
    else if (filterName == "erosion" || filterName == "opening")
      filter = new ErosionFilter<Type>(*sel);
  }
  else {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  // apply filter
  if (filter) {
    sd::core::Image<Type> conv;
    filter->applyTo(conv, *im);
    delete filter;

    if (filterName == "opening") {
      filter = new DilationFilter<Type>(*sel);
      filter->applyTo(conv, conv);
      delete filter;
    }
    else if (filterName == "closing") {
      filter = new ErosionFilter<Type>(*sel);
      filter->applyTo(conv, conv);
      delete filter;
    }

    if (sel)
      delete sel;

    saveImage(&conv, filenameOut);
  }

  delete image;
}
