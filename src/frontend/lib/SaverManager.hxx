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

#ifndef SD_FRONTEND_SAVER_MANAGER_HXX
#define SD_FRONTEND_SAVER_MANAGER_HXX

#include <utils/sdFileUtils.hpp>

namespace sd {
  namespace frontend {

    inline SaverManager&
    SaverManager::instance()
    {
      static SaverManager inst;
      return inst;
    }

    template<class Input>
    bool
    saveFile(Input* input, const std::string& filename, bool binary)
    {
      Query<Saver> proto = SaverManager::instance().availablePrototypes();
      if (proto.empty())
	return false;

      // get prototypes according to file extension filters
      auto predicate = [&filename](Saver* obj) -> bool {
	const std::string& fileExt = obj->fileExtensionFilters();
	return !sd::utils::fileMatchesFilters(filename, fileExt);
      };

      proto.withoutPredicate(predicate);
      if (proto.empty())
	return false;

      // try to call load function of each remaining prototype
      for (auto it = proto.begin(); it != proto.end(); ++it) {
	// use transient connection
	(*it)->setInputNodes(input);
	//(*it)->setParams("filename", filename, "binary", binary);
	bool successful = (*it)->save();
	(*it)->disconnectInputNodes(input);
	if (successful)
	  return true;
      }

      return false;
    }

  }
}

#endif /*!SD_FRONTEND_SAVER_MANAGER_HXX */
