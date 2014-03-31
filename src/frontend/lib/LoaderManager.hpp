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

#ifndef SD_FRONTEND_LOADER_MANAGER_HPP
#define SD_FRONTEND_LOADER_MANAGER_HPP

#include "Manager.hpp"
//#include "Parameter.hpp"
#include "Loader.hpp"
#include "Query.hpp"

#include <SmithDRDefs.hpp>

#include <string>

namespace sd {
  namespace frontend {

    class LoaderManager
      : public Manager<Loader>
    {

    public:

      static inline LoaderManager& instance();

    protected:

      virtual ~LoaderManager();

    };

    void registerLoader(Loader* obj);
    //void registerLoader(Loader* obj, const ParameterList& parameters);
    void registerLoader(Loader* obj, const std::string& parameters);

    template<typename... Outputs>
    bool loadFile(const std::string& filename, Outputs& ...outputs);

    Query<Loader> queryLoader(const std::string& name);

    const Query<Loader> getLoaders();

    // identify loaders based on filename extension
    const Query<Loader> detectLoaders(const std::string& filename);

  }
}

#include "LoaderManager.hxx"

#endif /*!SD_FRONTEND_LOADER_MANAGER_HPP */
