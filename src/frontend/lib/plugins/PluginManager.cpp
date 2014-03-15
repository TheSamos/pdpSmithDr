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

#include "PluginManager.hpp"

#include <dirent.h>
#if defined _WIN32 || defined __CYGWIN__
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#include <iostream>

sd::frontend::plugins::PluginManager::PluginManager()
  : m_loadedPlugins() {}

sd::frontend::plugins::PluginManager::~PluginManager() {
  m_loadedPlugins.clear();
}

sd::frontend::plugins::PluginManager&
sd::frontend::plugins::PluginManager::instance() {
  static PluginManager instance;
  return instance;
}

void
sd::frontend::plugins::PluginManager::loadPlugin(const std::string& path, const std::string& pluginName) {
  if (m_loadedPlugins.find(pluginName) == m_loadedPlugins.end()) {
    std::string fullName = path + PATH_SEPARATOR + "lib" + pluginName;
#if defined _WIN32 || defined __CYGWIN__
    fullName += ".dll";
#elif defined __APPLE__
    //fullName += ".dylib";
    fullName += ".so"; // mac modules (bundles) use .so file extension
#else
    fullName += ".so";
#endif

    std::string err;
    DynamicLibrary* dl = DynamicLibrary::load(fullName, err);
    if (!dl) {
      std::cerr << err << std::endl;
      return;
    }

    m_loadedPlugins[pluginName] = core::SharedObject<DynamicLibrary>(dl);

    typedef void (*RegisterPluginFunc)();
    RegisterPluginFunc rpFunc = reinterpret_cast<RegisterPluginFunc>((intptr_t) dl->getSymbol("registerPlugin"));
    if (!rpFunc)
      return;

    rpFunc();
  }
}

bool
sd::frontend::plugins::PluginManager::loadPlugins(const std::string& dir) {
#ifdef DEBUG
  std::cout << "Plugin dir: " << dir << std::endl;
#endif

  DIR* rep = opendir(dir.c_str());
  if (rep == NULL) {
    perror("");
    return false;
  }

  PluginManager& pm = instance();
  struct dirent* readFile = NULL;
  while ((readFile = readdir(rep)) != NULL) {
    std::string filename(readFile->d_name);
    if (filename.find("Plugin") != std::string::npos) {
      if (filename.size() > 3 && filename.substr(0, 3) == "lib") {
	size_t pos = filename.find_first_of('.');
	if (pos != std::string::npos) {
	  std::string pluginName = filename.substr(3, pos-3);
#ifdef DEBUG
	  std::cout << "\t Load " << pluginName << std::endl;
#endif
	  pm.loadPlugin(PLUGIN_DIR, pluginName);
	}
      }
    }
  }

  if (closedir(rep) == -1) {
    perror("");
    return false;
  }

  return true;
}
