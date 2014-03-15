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
#include <frontend/lib/LoaderManager.hpp>

#include <iostream>

int
main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.txt>\n";
    return EXIT_FAILURE;
  }

  std::string inFilename(argv[1]);

  std::cout << "Plugin dir: " << PLUGIN_DIR << std::endl;

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load AlgoExamplePlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "AlgoExamplePlugin");
  std::cout << "Load LoaderExamplePlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderExamplePlugin");

  size_t nbAlgo = sd::frontend::AlgorithmManager::instance().getSize();
  size_t nbLoader = sd::frontend::LoaderManager::instance().getSize();

  std::cout << std::endl;
  std::cout << "Loaded: " << nbAlgo << " AlgoPlugin(s)\n";

  std::cout << std::endl;
  std::cout << "Loaded: " << nbLoader << " LoaderPlugin(s)\n";

  std::cout << std::endl;

  // algo
  auto algo = sd::frontend::queryAlgorithm("AlgoExamplePlugin").create();
  if (!algo) {
    std::cerr << "Unable to create algorithm.\n\n";
    return EXIT_FAILURE;
  }
  algo->run();
  std::cout << std::endl;
  delete algo; // because not attached to graph of views

  // loader
  std::cout << std::endl;
  auto loaded = sd::frontend::loadFile(inFilename);
  if (!loaded) {
    std::cerr << "Unable to load file: " << inFilename << ".\n\n";
    return EXIT_FAILURE;
  }
}
