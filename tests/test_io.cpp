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
#include <core/image/ImageView.hpp>
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <cstdlib>

void
saveAs(sd::core::ImageView* iv, const std::string& filename, bool binary)
{
  bool saved = sd::frontend::saveFile(iv, filename, binary);
  if (!saved)
    std::cerr << "Unable to save file: " << filename << ".\n\n";
}

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>\n";
    std::cerr << "   Valid extensions are .pgm, .pbm, .pgm3d, .pbm3d, .sdp3d\n";

    return EXIT_FAILURE;
  }

  std::string inFilename(argv[1]);
  std::string outFilename(argv[2]);

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgmPlugin");
  std::cout << "Load LoaderPgm3dPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderPgm3dPlugin");

  std::cout << "Load SaverPgmPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgmPlugin");
  std::cout << "Load SaverPgm3dPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverPgm3dPlugin");
  std::cout << "Load SaverSdp3dPlugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverSdp3dPlugin");

  // load
  sd::core::ImageView* iv = NULL;
  bool loaded = sd::frontend::loadFile(inFilename, iv);
  if (!loaded || !iv) {
    std::cerr << "Unable to load file: " << inFilename << ".\n\n";
    return EXIT_FAILURE;
  }

  // save
  std::string base = sd::utils::cutFileExtension(outFilename, ".pgm;.pbm;.pgm3d;.pbm3d;.sdp3d");
  saveAs(iv, base+"_binary.pgm", true);
  saveAs(iv, base+"_ascii.pgm", false);
  saveAs(iv, base+"_binary.pgm3d", true);
  saveAs(iv, base+"_ascii.pgm3d", false);
  saveAs(iv, base+"_binary.sdp3d", true);
  saveAs(iv, base+"_ascii.sdp3d", false);

  delete iv;

  return EXIT_SUCCESS;  
}
