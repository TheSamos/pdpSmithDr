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

#ifndef SD_FRONTEND_QUERY_HPP
#define SD_FRONTEND_QUERY_HPP

#include <SmithDRDefs.hpp>
#include <core/ViewNode.hpp>

#include <vector>
#include <functional>
#include <string>

namespace sd {
  
  namespace frontend {

    template<class Object>
    class Query {

    public:

      typedef std::function<bool(Object*)> Predicate;
      typedef typename std::vector<Object*>::iterator iterator;
      typedef typename std::vector<Object*>::const_iterator const_iterator;

      Query();
      Query(const Query&);
      ~Query();

      Query& operator=(const Query<Object>&);

      bool empty() const;
      size_t size() const;
      void addPrototype(Object* prototype);

      Object* create() const;
      
      Query<Object>& withoutPredicate(Predicate p);

      Query<Object>& withSameTypeAs(core::ViewNode* node);

      Query<Object>& withName(const std::string &name);

      iterator begin();
      iterator end();

      const_iterator begin() const;
      const_iterator end() const;

      const Object* first() const;

    private:

      std::vector<Object*> m_proto;

    };

  }
  
}

#include "Query.hxx"

#endif /*! SD_FRONTEND_QUERY_HPP */
