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

#include <../plugins/algo/tomo/FastRadon.hpp> // :GLITCH: relative path is unsafe

#include <core/image/Image.hpp>
#include <core/image/ImageView_.hpp>

#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/Parameter.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>

// estimate osem factor based on nbAngles: k.osem=nbAngles with k in [18-30]
int
estimateOSEM(int nbAngles)
{
  int k = 18;
  while ((nbAngles%k) != 0 && k <= 30)
    ++k;
  return (k > 30 ? 1 : nbAngles / k);
}

void
saveImage(sd::core::ImageView_<double> *img, std::string outFilename) {
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
			       sd::Size(width, height, nbSlices));
  sd::core::Image<sd::UINT8> im(info);
  im.setData(data);

  bool saved = sd::frontend::saveFile((sd::core::ImageView*) &im, outFilename);
  if (!saved)
    std::cerr << "Unable to save file: " << outFilename << ".\n\n";
}


bool
tomo(sd::core::ImageView* iv, int scheme, bool useFBP, const std::string& method, int nbIter, const std::vector<double>& angles, const std::string& outFilename, sd::core::ImageView_<double>*& result)
{
  auto query = sd::frontend::queryAlgorithm("TomographicReconstructionPlugin");
  auto algoPtr = query.withSameTypeAs(iv).create();
  //auto algoPtr = query.create();

  if (!algoPtr) {
    std::cerr << "Unable to create algorithm.\n\n";
    return false;
  }

  std::cout << std::endl;
  std::cout << "Input image: " << iv->name() << std::endl;

  algoPtr->setInputNodes(iv);

  algoPtr->setParams("scheme", scheme, "useFBP", useFBP, "method", method, "nbIter", nbIter, "angles", angles);
  bool successful = algoPtr->run();
  if (!successful)
    return false;

  result = dynamic_cast<sd::core::ImageView_<double>*>(algoPtr);
  saveImage(result, outFilename);

  return true;
}

int
main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <sinogram.pgm> <method=radon,fbp,sart,osem> <output.pgm>\n";
    return EXIT_FAILURE;
  }

  std::string inFilename(argv[1]);
  std::string method(argv[2]);
  std::string outFilename(argv[3]);

  // load plugin
  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  pm.loadPlugin(PLUGIN_DIR, "TomographicReconstructionPlugin");
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  // load image
  sd::core::ImageView* sinoImg = NULL;
  bool loaded = sd::frontend::loadFile(inFilename, sinoImg);
  if (!loaded || !sinoImg) {
    std::cerr << "Unable to load file: " << inFilename << ".\n\n";
    return EXIT_FAILURE;
  }

  // tomo reconstruction
  bool useFBP = true;
  if (method.compare("radon") == 0) {
    method = "Radon inverse";
    useFBP = false;
  }
  else if (method.compare("fbp") == 0) {
    method = "Radon inverse";
    useFBP = true;
  }
  else if (method.compare("sart") == 0) {
    method = "SART";
  }
  else if (method.compare("osem") == 0) {
    method = "OSEM";
  }

  int nbAngles = sinoImg->size().height();
  std::vector<double> angles(nbAngles);
  for (int i = 0; i < nbAngles; ++i)
    angles[i] = double(i)*180./double(nbAngles);

  sd::core::ImageView_<double>* result = NULL;
  bool successful = tomo(sinoImg, sd::tomo::OPTIMAL, useFBP, method, 4, angles, outFilename, result);

  delete result;
  delete sinoImg;

  return (successful ? EXIT_SUCCESS : EXIT_FAILURE);
}
