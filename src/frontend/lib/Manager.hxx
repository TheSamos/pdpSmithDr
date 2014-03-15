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

#ifndef SD_FRONTEND_MANAGER_HXX
#define SD_FRONTEND_MANAGER_HXX

#include <iostream>

namespace sd {
  namespace frontend {

    template<typename Object>
    Manager<Object>::Manager()
      : m_objects()
    {
    }

    template<typename Object>
    Manager<Object>::~Manager()
    {
#ifdef DEBUG
      std::cout << "~Manager\n";
#endif
      for (reverse_iterator it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
#ifdef DEBUG
	std::cout << "destroy " << it->second->name() << std::endl;
#endif
	delete it->second;
      }
    }

    template<typename Object>
    void
    Manager<Object>::addObject(Object* p)
    {
      if (p) {
	std::string name = p->name();
	m_objects.insert(std::make_pair(name, p));
#ifdef DEBUG
	std::cout << "add " << name << std::endl;
#endif
      }
    }

    template<typename Object>
    size_t
    Manager<Object>::getSize() const
    {
      return m_objects.size();
    }

    template<typename Object>
    Query<Object>
    Manager<Object>::availablePrototypes()
    {
      Query<Object> query;
      for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	query.addPrototype(it->second);

      return query;
    }

    template<typename Object>
    Query<Object>
    Manager<Object>::availablePrototypes(const std::string& name)
    {
      TMapRange range = m_objects.equal_range(name);
      if (range.first == m_objects.end())
	return Query<Object>();

      Query<Object> query;
      for (auto it = range.first; it != range.second; ++it)
	query.addPrototype(it->second);

      return query;
    }

  }
}

#endif /*! SD_FRONTEND_MANAGER_HXX */
