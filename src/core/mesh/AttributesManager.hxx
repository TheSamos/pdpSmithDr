/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_CORE_ATTRIBUTES_MANAGER_HXX
#define SD_CORE_ATTRIBUTES_MANAGER_HXX

template<typename C, typename E>
sd::core::AttributesManager<C, E>::AttributesManager() : sd::core::SharedObject<map_attributes>(),
							 m_attributes(this->data()) {}

template<typename C, typename E>
sd::core::AttributesManager<C, E>::AttributesManager(const sd::core::AttributesManager<C, E>& am)
  : sd::core::SharedObject<map_attributes>(am) {}

template<typename C, typename E>
sd::core::AttributesManager<C, E>::~AttributesManager() {
  if ((*this).isTheLast() && m_attributes) {
    typename map_attributes::iterator it1 = (*m_attributes).begin();
    const typename map_attributes::iterator end1 = (*m_attributes).end();
    for (; it1 != end1; ++it1) {
      typename std::map<C*, AttributeCollection<E*>* >::iterator it2 = (*it1).second.begin();
      const typename std::map<C*, AttributeCollection<E*>* >::iterator end2 = (*it1).second.end();
      for (; it2 != end2; ++it2) {
	delete (*it2).second;
      }
    }
  }
}

template<typename C, typename E>
std::map<C*, sd::core::AttributeCollection<E* >* >&
sd::core::AttributesManager<C, E>::getAttributes(const sd::core::AttributeKey& akey, const C* container) {
  assert(akey.id() != 0);
  if ((*m_attributes).find(akey.id()) != (*m_attributes).end())
    return (*m_attributes).find(akey.id())->second;
  else if (!container->askableParent())
    assert(false && "Attribute is not defined !!!");
  return container->askableParent()->getAttribute(akey, container);
}

template<typename C, typename E>
void
sd::core::AttributesManager<C, E>::setAttributes(const sd::core::AttributeKey& akey,
						 sd::core::AttributeCollection<E* >* attribute, const C* container) {
  assert(akey.id() != 0);
  delete (*m_attributes)[akey.id()];
  (*m_attributes)[akey.id()] = attribute;
}

template<typename C, typename E>
void
sd::core::AttributesManager<C, E>::importParentAttribute(const sd::core::AttributeKey& akey, const C* container) {
  assert(container->askableParent());
  {
    if ((*m_attributes)[akey.id()].find((C*)container) != (*m_attributes)[akey.id()].end()) {
      delete (*m_attributes)[akey.id()][(C*)container];
      (*m_attributes)[akey.id()][(C*)container] = NULL;
    }
  }
  {
    (*m_attributes)[akey.id()][(C*)container] = (*m_attributes)[akey.id()][(C*)(container->askableParent())]->clone();
  }
}

template<typename C, typename E>
bool
sd::core::AttributesManager<C, E>::removeAttributeKey(const AttributeKey& akey, const C* container) {
  assert(akey.id() != 0);
  if ((*m_attributes).find(akey.id()) != (*m_attributes).end()) {
    if ((*m_attributes)[akey.id()].find((C*)container) != (*m_attributes)[akey.id()].end()) {
      delete (*m_attributes)[akey.id()][(C*)container];
    }
    (*m_attributes)[akey.id()][container] = NULL;
    return true;
  }
  return false;
}

template<typename C, typename E>
void
sd::core::AttributesManager<C, E>::removeAttributes(const C* container) {
  typename map_attributes::iterator it = (*m_attributes).begin();
  const typename map_attributes::iterator end = (*m_attributes).end();
  for (; it!=end; ++it)
    if ((*it).second.find((C*)container) != (*it).second.end()) {
      typename std::map<C*, AttributeCollection<E*>* >::iterator target = (*it).second.find((C*)container);
      delete (*target).second;
      (*it).second.erase(target);
    }
}

template<typename C, typename E>
std::set<sd::core::AttributeKey>
sd::core::AttributesManager<C, E>::getAttributesKeys(const E* element, const C* container) const {
  std::set<AttributeKey> result;
  assert(element->isValidAtView((C*)container));
  /*if (container->askableParent() != NULL
      && element->isValidAtView((C*)container->askableParent())) {
    result = ((C*)container->askableParent())->getAttributesKeys(element);
  }*/
  typename map_attributes::const_iterator it = (*m_attributes).begin();
  const typename map_attributes::const_iterator end = (*m_attributes).end();
  for (; it!=end; ++it) {
    AttributeKey akey = AttributeKey::getKey((*it).first);
    typename std::map<C*, AttributeCollection<E* >* >::const_iterator itC = (*it).second.begin();
    const typename std::map<C*, AttributeCollection<E* >* >::const_iterator endC = (*it).second.end();
    for (; itC!=endC; ++itC) {
      if ((*(*itC).first) <= *container
	  && element->isValidAtView((*itC).first)
	  && (*itC).second != NULL
	  && (static_cast<typename sd::core::AttributeCollection<E*>*>((*itC).second)->containsKey((E *) element)))
	result.insert(akey);
    }
  }
  return result;
}

template<typename C, typename E>
sd::core::AttributeType
sd::core::AttributesManager<C, E>::getAttributeDataTypeAtKey(const E* element,
							     const AttributeKey& akey,
							     const C* container) const {
  assert(akey.id() != 0);
  assert(element->isValidAtView((C*)container));
  if (((*m_attributes).find(akey.id()) == (*m_attributes).end()
       || (*m_attributes).find(akey.id())->second.find((C*)container) == (*m_attributes).find(akey.id())->second.end())
      && container->askableParent() != NULL
      && element->isValidAtView(container->askableParent())) {
    return container->askableParent()->getAttributeDataTypeAtKey(element, akey);
  } else {
    assert((*m_attributes).find(akey.id()) != (*m_attributes).end());
    assert((*m_attributes).find(akey.id())->second.find((C*)container) != (*m_attributes).find(akey.id())->second.end());
    assert((*m_attributes).find(akey.id())->second.find((C*)container)->second != NULL);
    assert((*m_attributes).find(akey.id())->second.find((C*)container)->second->containsKey(element));
    return (*m_attributes).find(akey.id())->second.find((C*)container)->second->getAttributeType();
  }
}

template<typename C, typename E>
bool
sd::core::AttributesManager<C, E>::isAttributeSet(const E* element,
						  const sd::core::AttributeKey& akey,
						  const C* container) const {
  assert(akey.id() != 0);
  element->isValidAtView((C*)container);
  if (((*m_attributes).find(akey.id()) == (*m_attributes).end()
       || (*m_attributes).find(akey.id())->second.find((C*)container) == (*m_attributes).find(akey.id())->second.end())
      && container->askableParent() != NULL
      && element->isValidAtView((MeshView*)container->askableParent())) {
    return container->askableParent()->isAttributeSet(element, akey);
  } else {
    return ((*m_attributes).find(akey.id()) != (*m_attributes).end()
	    && (*m_attributes).find(akey.id())->second.find((C*)container) != (*m_attributes).find(akey.id())->second.end()
	    && (*m_attributes).find(akey.id())->second.find((C*)container)->second != NULL
	    && (*static_cast<sd::core::AttributeCollection<E*>*>((*m_attributes).find(akey.id())->second.find((C*)container)->second)).containsKey((E*)element));
  }
}

template<typename C, typename E>
template<typename A>
const A&
sd::core::AttributesManager<C, E>::getAttribute(const E* element,
						const sd::core::AttributeKey& akey, const C* container) {
  assert(akey.id() != 0);
  assert(element->isValidAtView((C*)container));
  if (((*m_attributes).find(akey.id()) == (*m_attributes).end()
       || (*m_attributes)[akey.id()].find((C*)container) == (*m_attributes)[akey.id()].end())
      && container->askableParent() != NULL
      && element->isValidAtView((C*)container->askableParent())) {
    return ((C*)container->askableParent())->template getAttribute<A>(element, akey);
  } else {
#ifdef DEBUG
    if ((*m_attributes).find(akey.id()) == (*m_attributes).end()
	|| (*m_attributes)[akey.id()].find((C*)container) == (*m_attributes)[akey.id()].end()) {
      std::cerr << container->askableParent() << " && " << container->getId() << " < " << container->getId() << " ? " << (element->isValidAtView((C*)container)) << " askAtt : " << akey.name() << std::endl;
    }
    assert((*m_attributes).find(akey.id()) != (*m_attributes).end());
    assert(element->isValidAtView((C*)container));
    assert(container->askableParent() == NULL || element->isValidAtView((C*)container));
    assert((*m_attributes)[akey.id()].find((C*)container) != (*m_attributes)[akey.id()].end());
    assert((*static_cast<typename sd::core::AttributeMap<E*, A>*>((*m_attributes)[akey.id()][(C*)container])).containsKey((E*)element));
    assert((*m_attributes)[akey.id()][(C*)container]->getAttributeType() == DataType_<A>());
#endif // DEBUG
    //if((*static_cast<typename sd::core::AttributeMap<E*, A>*>((*m_attributes)[akey.id()][container])).size() <= element)
    //(*static_cast<typename sd::core::AttributeMap<E*, A>*>((*m_attributes)[akey.id()][container])).getData().resize(element+1);
    return (*static_cast<typename sd::core::AttributeMap<E*, A>*>((*m_attributes)[akey.id()][(C*)container]))[(E*)element];
  }
}

template<typename C, typename E>
template<typename A>
void
sd::core::AttributesManager<C, E>::setAttribute(const E* element,
						const sd::core::AttributeKey& akey,
						const A& val, const C* container) {
  assert(akey.id() != 0);
  if ((*m_attributes).find(akey.id()) == (*m_attributes).end()
      || (*m_attributes)[akey.id()].find((C*)container) == (*m_attributes)[akey.id()].end()) {
    if (isAttributeSet(element, akey, container)) {
      importParentAttribute(akey, container);
    } else {
      if ((*m_attributes).find(akey.id()) == (*m_attributes).end())
	(*m_attributes)[akey.id()] = std::map<C*, AttributeCollection<E*>* >();
      assert((*m_attributes)[akey.id()].find((C*)container) == (*m_attributes)[akey.id()].end());
      (*m_attributes)[akey.id()][(C*)container] = new sd::core::AttributeMap<E*, A>();
    }
  }

  assert((*m_attributes)[akey.id()][(C*)container]->getAttributeType() == DataType_<A>());
  (*static_cast<typename sd::core::AttributeMap<E*, A>*>((*m_attributes)[akey.id()][(C*)container]))[(E*)element] = val;
}

template<typename C, typename E>
bool
sd::core::AttributesManager<C, E>::importAttributes(E* e1, const E* e2, const C* container) {
  std::set<sd::core::AttributeKey> keys = getAttributesKeys(e2, container);
  bool result = true;
  std::set<sd::core::AttributeKey>::const_iterator keyIt = keys.begin();
  const std::set<sd::core::AttributeKey>::const_iterator endKey = keys.end();
  for (; keyIt!=endKey; ++keyIt) {
    if ((*m_attributes).find((*keyIt).id()) == (*m_attributes).end())
      importParentAttribute(*keyIt, container);
    typename map_attributes::iterator itAtts = (*m_attributes).find((*keyIt).id());
    assert(itAtts != (*m_attributes).end());
    /*
    assert((*itAtts).second.find(container) != (*itAtts).second.end());
    if ((*itAtts).second.find(container) == (*itAtts).second.end()) {
      AttributeCollection<E*>* tmp = (*(*itAtts).second.find(containingView2)).second->clone();
      tmp->clear();
      (*itAtts).second[containingView1] = tmp;
    }
    */
    result &= (*itAtts).second[(C*)container]->import(e1, (*itAtts).second[(C*)container], (E*)e2);
  }
  return result;
}

#endif /*! SD_CORE_ATTRIBUTES_MANAGER_HXX */
