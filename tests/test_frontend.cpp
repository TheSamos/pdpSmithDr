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
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/Parameter.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <core/image/Image.hpp>
#include <core/mesh/MeshView_.hpp>
#include <core/ViewNode.hpp>

#include <iostream>

#include <cstring>

#define VERBOSE_VIEW(View) \
  std::cout << "\t" << #View << " has " << View->getPreviousViewNodes().size() << " parents and " << View->getNextViewNodes().size() << " children\n";

bool
mirror(sd::core::ImageView* iv, bool horizontal, const std::string& outFilename, sd::core::ImageView*& result)
{
  auto query = sd::frontend::queryAlgorithm("ImageMirrorPlugin");
  auto algoPtr = query.withSameTypeAs(iv).create();

  if (!algoPtr) {
    std::cerr << "Unable to create algorithm.\n\n";
    return false;
  }

  std::cout << std::endl;
  std::cout << "Input image: " << iv->name() << std::endl;

  algoPtr->setInputNodes(iv);
  algoPtr->setParams("horizontal", horizontal);
  bool successful = algoPtr->run();
  if (!successful)
    return false;

  result = dynamic_cast<sd::core::ImageView*>(algoPtr);

  if (result) {
    bool saved = sd::frontend::saveFile(result, outFilename);
    if (!saved)
      std::cerr << "Unable to save file: " << outFilename << ".\n\n";
  }
  else
    std::cout << "Not an ImageView\n";

  return true;
}

bool
mix(sd::core::ImageView* iv1, sd::core::ImageView* iv2, float alpha, const std::string& outFilename, sd::core::ImageView*& result)
{
  auto algoPtr = sd::frontend::queryAlgorithm("ImageMixPlugin").withSameTypeAs(iv1).create();
  if (!algoPtr) {
    std::cerr << "Unable to create algorithm.\n\n";
    return false;
  }

  std::cout << std::endl;
  std::cout << "Input image1: " << iv1->name() << std::endl;
  std::cout << "Input image2: " << iv2->name() << std::endl;

  algoPtr->setInputNodes(iv1, iv2);
  algoPtr->setParams("alpha", alpha);
  bool successful = algoPtr->run();
  if (!successful)
    return false;

  result = dynamic_cast<sd::core::ImageView*>(algoPtr);

  if (result) {
    bool saved = sd::frontend::saveFile(result, outFilename);
    if (!saved)
      std::cerr << "Unable to save file: " << outFilename << ".\n\n";
  }
  else
    std::cout << "Not an ImageView*\n";

  return true;
}

int
main(int argc, char* argv[])
{
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0] << " <input.pgm> <horizontal,vertical> <alpha> <output1.pgm> <output2.pgm>\n";
    return EXIT_FAILURE;
  }

  std::string inFilename(argv[1]);
  bool horizontal = (!strcmp(argv[2], "horizontal"));
  float alpha = atof(argv[3]);
  std::string outFilename1(argv[4]);
  std::string outFilename2(argv[5]);

  // load plugins
  std::cout << "Plugin dir: " << PLUGIN_DIR << std::endl;
  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  pm.loadPlugin(PLUGIN_DIR, "ImageMirrorPlugin"); // ImageViewAlgorithm
  pm.loadPlugin(PLUGIN_DIR, "ImageMixPlugin"); // ComputedImageViewAlgorithm
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  // load image
  sd::core::ImageView* iv = NULL;
  bool loaded = sd::frontend::loadFile(inFilename, iv);
  if (!loaded || !iv) {
    std::cerr << "Unable to load file: " << inFilename << ".\n\n";
    return EXIT_FAILURE;
  }

  std::cout << "At the beginning:\n";
  VERBOSE_VIEW(iv);

  // run algo
  sd::core::ImageView* ivMirrored = NULL;
  bool successful = mirror(iv, horizontal, outFilename1, ivMirrored);
  if (!successful)
    return EXIT_FAILURE;

  std::cout << "After mirror:\n";
  VERBOSE_VIEW(iv);
  VERBOSE_VIEW(ivMirrored);

  sd::core::ImageView* ivMixed = NULL;
  if (successful)
    successful = successful && mix(iv, ivMirrored, alpha, outFilename2, ivMixed);

  std::cout << "After mix:\n";
  VERBOSE_VIEW(iv);
  VERBOSE_VIEW(ivMirrored);
  VERBOSE_VIEW(ivMixed);

  delete ivMixed;
  delete ivMirrored;
  delete iv;

  return (successful ? EXIT_SUCCESS : EXIT_FAILURE);
}

#undef VERBOSE_VIEW
