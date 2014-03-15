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

#ifndef SD_FRONTEND_PLUGINS_PLUGIN_MANAGER_HPP
#define SD_FRONTEND_PLUGINS_PLUGIN_MANAGER_HPP

#include <core/SharedObject.hpp>

#include "DynamicLibrary.hpp"

#include <map>
#include <string>

namespace sd {
  
  namespace frontend {
    
    namespace plugins {

      class PluginManager {

	// Map of plugins by their associated file names
	typedef std::map<std::string, sd::core::SharedObject<DynamicLibrary> > PluginMap;

      public:

	static PluginManager& instance();

	void loadPlugin(const std::string& path, const std::string& pluginName);

	static bool loadPlugins(const std::string& dir);

      private:

	PluginManager();
	~PluginManager();

	PluginManager(const PluginManager&) = delete;
	const PluginManager& operator=(const PluginManager&) = delete;

      private:

	PluginMap m_loadedPlugins;

      };

    }
    
  }
  
}

#endif /* ! SD_FRONTEND_PLUGINS_PLUGIN_MANAGER_HPP */
