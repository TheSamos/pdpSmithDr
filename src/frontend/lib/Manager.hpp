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

#ifndef SD_FRONTEND_MANAGER_HPP
#define SD_FRONTEND_MANAGER_HPP

#include <SmithDRDefs.hpp>
#include "Query.hpp"

#include <map>

namespace sd {
  namespace frontend {

    template<typename Object>
    class Manager
    {

    public:

      // A map of objects
      typedef std::multimap<std::string, Object*> TMap;
      typedef typename TMap::iterator iterator;
      typedef typename TMap::reverse_iterator reverse_iterator;
      typedef std::pair<iterator, iterator> TMapRange;

    public:

      // Allows specialized managers to add new objects
      void addObject(Object* p);

      // Get the total number of registered objects
      size_t getSize() const;

      Query<Object> availablePrototypes(); // list of all objects
      Query<Object> availablePrototypes(const std::string& name);

    protected:

      Manager();

      virtual ~Manager();

    private:

      Manager(const Manager<Object>&) = delete;
      const Manager<Object>& operator=(const Manager<Object>&) = delete;

    private:

      TMap m_objects; // All available objects

    };

  }
}

#include "Manager.hxx"

#endif /*! SD_FRONTEND_MANAGER_HPP */
