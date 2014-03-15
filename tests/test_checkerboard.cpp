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

#include <frontend/lib/SaverManager.hpp>

#include <core/image/Image.hpp>
#include <core/ViewNode.hpp>

#include <iostream>

#include <cstring>

bool
buildCheckerBoard(size_t width, size_t height, size_t sqSize, const std::string& outFilename)
{
  auto query = sd::frontend::queryAlgorithm("BuildCheckerboardImagePlugin");
  auto algoPtr = query.create();

  if (!algoPtr) {
    std::cerr << "Unable to create algorithm.\n\n";
    return false;
  }

  algoPtr->setParams("width", width, "height", height, "sqSize", sqSize);
  bool successful = algoPtr->run();
  if (!successful)
    return false;

  sd::core::ImageView* result = NULL;
  algoPtr->getOutputs(result);

  if (result) {
    bool saved = sd::frontend::saveFile(result, outFilename);
    if (!saved)
      std::cerr << "Unable to save file: " << outFilename << ".\n\n";
  }
  else
    std::cout << "Not an ImageView\n";

  delete algoPtr; // it's a BlackBoxAlgorithm

  return true;
}

int
main(int argc, char* argv[])
{
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <width> <height> <square size> <output.pgm>\n";
    return EXIT_FAILURE;
  }

  size_t width = atoi(argv[1]);
  size_t height = atoi(argv[2]);
  size_t sqSize = atoi(argv[3]);
  std::string outFilename(argv[4]);

  // load plugin
  std::cout << "Plugin dir: " << PLUGIN_DIR << std::endl;
  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  pm.loadPlugin(PLUGIN_DIR, "BuildCheckerboardImagePlugin");
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");

  // run algo
  bool successful = buildCheckerBoard(width, height, sqSize, outFilename);
  return (successful ? EXIT_SUCCESS : EXIT_FAILURE);
}
