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

#ifndef SD_FRONTEND_QUERY_HXX
#define SD_FRONTEND_QUERY_HXX

#include <algorithm>

namespace sd {
  
  namespace frontend {

    template<class Object>
    Query<Object>::Query() : m_proto() {}

    template<class Object>
    Query<Object>::Query(const Query<Object>& q)
      : m_proto(q.m_proto) {}

    template<class Object>
    Query<Object>::~Query() {}

    template<class Object>
    Query<Object>&
    Query<Object>::operator=(const Query& q) {
      if (&q != this)
	m_proto = q.m_proto;
      return *this;
    }

    template<class Object>
    bool
    Query<Object>::empty() const {
      return m_proto.empty();
    }

    template<class Object>
    size_t
    Query<Object>::size() const {
      return m_proto.size();
    }

    template<class Object>
    void
    Query<Object>::addPrototype(Object* prototype) {
      m_proto.push_back(prototype);
    }

    template<class Object>
    Object*
    Query<Object>::create() const {
      if (empty())
	return NULL;

      return static_cast<Object*>(m_proto[0]->clone());
    }

    template<class Object>
    Query<Object>&
    Query<Object>::withoutPredicate(Predicate p) {
      m_proto.erase(std::remove_if(m_proto.begin(), m_proto.end(), p), m_proto.end());
      return *this;
    }

    template<class Object>
    bool
    isSameType(Object* obj, core::ViewNode* node) {
      // compare object types: ImageView, MeshView, ...
      auto nodeObj = dynamic_cast<core::ViewNode*>(obj);
      if (nodeObj == NULL || nodeObj->getSDType() != node->getSDType())
	return false;

      // compare data types: int, float, ...
      return true; // :TODO:Why???? (*(nodeObj->dataTypeIn()) == *(node->dataTypeOut()));
    }

    template<class Object>
    Query<Object>&
    Query<Object>::withSameTypeAs(core::ViewNode* node) {
      auto predicate = [&](Object* obj) { return !isSameType(obj, node); };
      return withoutPredicate(predicate);
    }
    
    template<class Object>
    Query<Object>&
    Query<Object>::withName(const std::string &name) {
      auto predicate = [&](Object* obj) { return (obj->name() != name); };
      return withoutPredicate(predicate);
    }


    template<class Object>
    typename Query<Object>::iterator
    Query<Object>::begin() {
      return m_proto.begin();
    }

    template<class Object>
    typename Query<Object>::iterator
    Query<Object>::end() {
      return m_proto.end();
    }

    template<class Object>
    typename Query<Object>::const_iterator
    Query<Object>::begin() const {
      return m_proto.begin();
    }

    template<class Object>
    typename Query<Object>::const_iterator
    Query<Object>::end() const {
      return m_proto.end();
    }

    template<class Object>
    const Object*
    Query<Object>::first() const {
      return (m_proto.empty() ? NULL : *(m_proto.begin()));
    }

  }
  
}

#endif /*! SD_FRONTEND_QUERY_HXX */
