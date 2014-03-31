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

#ifndef SD_FRONTEND_SAVER_MANAGER_HPP
#define SD_FRONTEND_SAVER_MANAGER_HPP

#include "Manager.hpp"
#include "Saver.hpp"
#include "Query.hpp"

#include <SmithDRDefs.hpp>

#include <string>

namespace sd {
  
  namespace frontend {

    class SaverManager : public Manager<Saver> {

    public:

      static inline SaverManager& instance();

    protected:

      virtual ~SaverManager();

    };

    void registerSaver(Saver* obj);

    template<class Input>
    bool saveFile(Input* input, const std::string& filename, bool binary=true);

    const Query<Saver> getSavers();

  }
  
}

#include "SaverManager.hxx"

#endif /*!SD_FRONTEND_SAVER_MANAGER_HPP */
