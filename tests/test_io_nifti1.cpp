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

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>\n";
    std::cerr << "   Valid extensions are .hdr, .img, .nii, .nia,\n";
    std::cerr << "   or any of them followed by .gz (if zlib is on your system).\n";

    return EXIT_FAILURE;
  }

  std::string inFilename(argv[1]);
  std::string outFilename(argv[2]);

  sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  std::cout << "Load LoaderNifti1Plugin\n";
  pm.loadPlugin(PLUGIN_DIR, "LoaderNifti1Plugin");
  std::cout << "Load SaverNifti1Plugin\n";
  pm.loadPlugin(PLUGIN_DIR, "SaverNifti1Plugin");

  // load
  sd::core::ImageView* iv = NULL;
  bool loaded = sd::frontend::loadFile(inFilename, iv);
  if (!loaded || !iv) {
    std::cerr << "Unable to load file: " << inFilename << ".\n\n";
    return EXIT_FAILURE;
  }

  // save
  bool saved = sd::frontend::saveFile(iv, outFilename);
  if (!saved) {
    std::cerr << "Unable to save file: " << outFilename << ".\n\n";
    return EXIT_FAILURE;
  }

  delete iv;

  return EXIT_SUCCESS;  
}
