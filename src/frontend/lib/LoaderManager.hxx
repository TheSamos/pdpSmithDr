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

#ifndef SD_FRONTEND_LOADER_MANAGER_HXX
#define SD_FRONTEND_LOADER_MANAGER_HXX

#include <utils/sdFileUtils.hpp>
#include <map>

namespace sd
{

namespace frontend
{

inline LoaderManager &
LoaderManager::instance()
{
    static LoaderManager inst;
    return inst;
}

template<typename... Outputs>
bool
loadFile(const std::string &filename, Outputs &...outputs)
{
    Query<Loader> proto = detectLoaders(filename);
    if (proto.empty())
        return false;

    std::string filename_xml =  "<parameter name=\"filename\" type=\"string\"><value>" + filename + "</value></parameter>";
    std::map<std::string, std::string> map = {{"filename", filename_xml}};

    // try to call load function of each remaining prototype
    for (auto it = proto.begin(); it != proto.end(); ++it)
    {
        (*it)->setXMLParams(map);

        bool successful = (*it)->load();

        if (successful)
        {
            (*it)->getOutputs(outputs...);
            (*it)->disconnectOutputNodes();

            return true;
        }
    }

    return false;
}

}

}

#endif /* ! SD_FRONTEND_LOADER_MANAGER_HXX */
