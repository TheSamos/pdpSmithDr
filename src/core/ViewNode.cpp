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

#include "ViewNode.hpp"

#include "ViewDAG.hpp"

#include <algorithm>

#include <cassert>

sd::core::ViewNode::~ViewNode() {
  
  // sd::core::ViewDAG::instance().release(this);
  if (m_dag)
    m_dag->release(this);
  
  std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
  const std::vector<ViewNode*>::iterator end = m_previousViewNodes.end();
  for (; it != end; ++it)
    (*it)->removeChild(this);
  
  // to disable this node when accessed from children during destruction
  unSetType();
  
  while(m_nextViewNodes.begin() != m_nextViewNodes.end())
    delete *(m_nextViewNodes.begin());

  typename std::map<attribute_key_id, Attribute*>::iterator itA = m_attributes.begin();
  const typename std::map<attribute_key_id, Attribute*>::iterator endA = m_attributes.end();
  for (; itA != endA; ++itA)
    delete (*itA).second;
}

const std::vector<sd::core::ViewNode*>&
sd::core::ViewNode::getPreviousViewNodes() const {
  return m_previousViewNodes;
}

const std::vector<sd::core::ViewNode*>&
sd::core::ViewNode::getNextViewNodes() const {
  return m_nextViewNodes;
}

void
sd::core::ViewNode::getParentsList(std::vector<ViewNode*>& parents) {
  parents.push_back(this);
  if (m_previousViewNodes.size() != 0) {
    std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
    const std::vector<ViewNode*>::iterator end = m_previousViewNodes.end();
    for (; it != end; ++it)
      (*it)->getParentsList(parents);
  }
}

void
sd::core::ViewNode::addParent(sd::core::ViewNode* parent) {
  m_previousViewNodes.push_back(parent);
  parent->addChild(this);
  computeMyId();
}

void
sd::core::ViewNode::disconnectChild(ViewNode* child) {
  if (m_nextViewNodes.empty())
    return;
  std::vector<ViewNode*> next;
  std::vector<ViewNode*>::iterator it = m_nextViewNodes.begin();
  const std::vector<ViewNode*>::iterator itEnd = m_nextViewNodes.end();
  for (; it != itEnd; ++it) {
    if (*it == child)
      child->disconnectParent(this);
    else
      next.push_back(child);
  }
  m_nextViewNodes = next;
}

void
sd::core::ViewNode::disconnectChildren() {
  if (m_nextViewNodes.empty())
    return;
  std::vector<ViewNode*>::iterator it = m_nextViewNodes.begin();
  const std::vector<ViewNode*>::iterator end = m_nextViewNodes.end();
  for (; it != end; ++it)
    (*it)->disconnectParent(this);
  m_nextViewNodes.clear();
}

sd::core::ViewId&
sd::core::ViewNode::getId() {
  return m_id;
}

const sd::core::ViewId&
sd::core::ViewNode::getId() const {
  return m_id;
}

void
sd::core::ViewNode::setGhostView(bool value) {
  if (m_isGhostView != value) {
    m_isGhostView = value;
    computeMyId();
  }
}

bool
sd::core::ViewNode::isGhostView() const {
  return m_isGhostView;
}

void
sd::core::ViewNode::setDAG(ViewDAG* dag) {
  if (m_dag != dag) {
    if (m_dag)
      m_dag->release(this);
    m_dag = dag;
    computeMyId();
  }
}

// =========

bool
sd::core::ViewNode::isAttributeSet(const sd::core::AttributeKey& akey) const {
  return getAttribute(akey) != NULL;
}

std::vector<sd::core::AttributeKey>
sd::core::ViewNode::getGlobalAttributesKeys() const {
  std::vector<sd::core::AttributeKey> result;
  std::map<attribute_key_id, Attribute*>::const_iterator it = m_attributes.begin();
  const std::map<attribute_key_id, Attribute*>::const_iterator end = m_attributes.end();
  for (; it != end; ++it)
    result.push_back(sd::core::AttributeKey::getKey((*it).first));
  return result;
}

const sd::core::Attribute*
sd::core::ViewNode::getAttribute(const sd::core::AttributeKey& akey) const {
  assert(akey.id() != 0);
  if (m_attributes.find(akey.id()) != m_attributes.end())
    return m_attributes.find(akey.id())->second;
  if (askableParent())
    return askableParent()->getAttribute(akey);
  else
    return NULL;
}

void
sd::core::ViewNode::setAttribute(const sd::core::AttributeKey& akey,
				 sd::core::Attribute* attribute) {
  assert(akey.id() != 0);
  delete m_attributes[akey.id()];
  m_attributes[akey.id()] = attribute;
}

sd::core::ViewNode*
sd::core::ViewNode::askableParent() {
  return (getPreviousViewNodes().size() == 1
	  && getPreviousViewNodes()[0]->getSDType() == this->getSDType())
    ? static_cast<ViewNode*>(getPreviousViewNodes()[0]) : NULL;
}

const sd::core::ViewNode*
sd::core::ViewNode::askableParent() const {
  return (getPreviousViewNodes().size() == 1
	  && getPreviousViewNodes()[0]->getSDType() == this->getSDType())
    ? static_cast<ViewNode*>(getPreviousViewNodes()[0]) : NULL;
}

// =========

sd::core::ViewNode::ViewNode(sd::ObjectType sdType,
			     ViewNode* previousViewNode,
			     const std::string& name)
  : sd::core::Object(sdType, name),
    m_id(),
    m_previousViewNodes(previousViewNode
			? std::vector<ViewNode*>(1, previousViewNode)
			: std::vector<ViewNode*>()),
    m_nextViewNodes(),
    m_isGhostView(true),
    m_dag(previousViewNode ? previousViewNode->dag() : NULL),
    m_attributes() {
  if (previousViewNode) {
    previousViewNode->addChild(this);
    if (name.empty()) {
      setName(name);
    }
  }
  computeMyId();
}

sd::core::ViewNode::ViewNode(sd::ObjectType sdType,
			     std::vector<ViewNode*>& previousViewNodes,
			     const std::string& name)
  : sd::core::Object(sdType, name),
    m_id(),
    m_previousViewNodes(previousViewNodes),
    m_nextViewNodes(),
    m_isGhostView(true),
    m_dag(NULL),
    m_attributes() {
  assert(m_previousViewNodes.size() > 0);
  m_dag = m_previousViewNodes[0]->dag();
  std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
  const std::vector<ViewNode*>::iterator end = m_previousViewNodes.end();
  for (; it != end; ++it) {
    assert(m_dag == (*it)->dag());
    (*it)->addChild(this);
  }
  computeMyId();
}

void
sd::core::ViewNode::removeChild(ViewNode* child) {
  std::vector<ViewNode*>::iterator it = m_nextViewNodes.begin();
  const std::vector<ViewNode*>::iterator end = m_nextViewNodes.end();
  for (; it != end; ++it)
    if (*it == child) {
      m_nextViewNodes.erase(it);
      child->removeParent(this);
      return;
    }
}

void
sd::core::ViewNode::removeParent(sd::core::ViewNode* parent) {
  std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
  const std::vector<ViewNode*>::iterator end = m_previousViewNodes.end();
  for (; it != end; ++it)
    if (*it == parent) {
      m_previousViewNodes.erase(it);
      computeMyId();
      return;
    }
}

void
sd::core::ViewNode::disconnectParent(sd::core::ViewNode* parent) {
  if (m_previousViewNodes.empty())
    return;
  std::vector<ViewNode*> prev;
  std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
  const std::vector<ViewNode*>::iterator itEnd = m_previousViewNodes.end();
  for (; it != itEnd; ++it)
    if (*it != parent)
      prev.push_back(*it);
  m_previousViewNodes = prev;
  computeMyId();
}

void
sd::core::ViewNode::addChild(ViewNode* child) {
  if (m_id.isNull() && !isGhostView() && m_dag)
    m_id = m_dag->getNewRootId();;
  // m_id = sd::core::ViewDAG::instance().getNewRootId();
  m_nextViewNodes.push_back(child);
}

bool
sd::core::ViewNode::isBranchRoot() const {
  return m_id.isBranchRoot();
}

std::vector<sd::core::ViewNode*>
filterGhostNodes(const std::vector<sd::core::ViewNode*>& setBN) {
  std::vector<sd::core::ViewNode*> result;  
  for (unsigned int i=0; i<setBN.size(); ++i)
    if (!setBN[i]->isGhostView())
      if (std::find(result.begin(), result.end(), setBN[i])
	== result.end())
      result.push_back(setBN[i]);
  return result;
}

void
sd::core::ViewNode::computeMyId() {
  if (!m_dag) {
    m_id = sd::core::ViewId();
    return;
  }
  // sd::core::ViewDAG::instance().release(this);
  m_dag->release(this);
  
  if (isGhostView()) {
    m_id = sd::core::ViewId();
  } else {  
    std::vector<const ViewId*> parentsId;
    std::vector<ViewNode*>::iterator it = m_previousViewNodes.begin();
    const std::vector<ViewNode*>::iterator end = m_previousViewNodes.end();
    for (; it != end; ++it) {
      if (std::find(parentsId.begin(), parentsId.end(), &((*it)->m_id))
	  == parentsId.end())
	parentsId.push_back(&((*it)->m_id));
    }
  
    std::vector<sd::core::ViewNode*> realsParents = filterGhostNodes(m_previousViewNodes);
  
    if (realsParents.size() == 0
	/*|| (m_previousViewNodes.size() == 1 && m_previousViewNodes[0]->isGhostView())*/) { // Has no parent
      if (m_nextViewNodes.size() == 0) // Has no child
	m_id = /*sd::core::ViewId();*/m_dag->getNewRootId();
      else // Is root
	m_id = /*sd::core::ViewDAG::instance().*/m_dag->getNewRootId();
    } else if (realsParents.size() == 1
	       && (filterGhostNodes(realsParents[0]->getNextViewNodes()).size() == 0
		   || (filterGhostNodes(realsParents[0]->getNextViewNodes()).size() == 1
		       && filterGhostNodes(realsParents[0]->getNextViewNodes())[0] == this))) { // Has one parent who has no or one child (me)
      ObjectType type = getSDType();
      ObjectType parentType = realsParents[0]->getSDType();
      if (type == parentType)
	m_id = /*sd::core::ViewDAG::instance().*/m_dag->getIncrementedId(*parentsId[0]);
      else
	m_id = /*sd::core::ViewDAG::instance().*/m_dag->getNewBranchId(*parentsId[0]);
    } else { // Has several parents, or a single parent with at least one child
      m_id = /*sd::core::ViewDAG::instance().*/m_dag->getNewBranchId(parentsId);
    }

    std::vector<ViewNode*>::iterator itN = m_nextViewNodes.begin();
    const std::vector<ViewNode*>::iterator endN = m_nextViewNodes.end();
    for (; itN != endN; ++itN) {
      (*itN)->computeMyId();
    }
  }

  // sd::core::ViewDAG::instance().add(this);
  m_dag->add(this);
  
}

bool
sd::core::comparable(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return comparable(bn1.m_id, bn2.m_id);
}

bool
sd::core::operator==(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return bn1.m_id == bn2.m_id || &bn1 == &bn2;
}

bool
sd::core::operator!=(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return bn1.m_id != bn2.m_id;
}

bool
sd::core::operator<(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  if (bn1.isGhostView()) {
    bool result = false;
    for (unsigned int i=0; i<bn1.m_nextViewNodes.size(); ++i)
      result |= (*bn1.m_nextViewNodes[i] <= bn2);
    return result;
  } else if (bn2.isGhostView()) {
    bool result = false;
    for (unsigned int i=0; i<bn2.m_previousViewNodes.size(); ++i)
      result |= (bn1 <= *bn2.m_previousViewNodes[i]);
    return result;
  }
  return bn1.m_id < bn2.m_id;
}

bool
sd::core::operator>(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return bn2 < bn1;//bn1.m_id > bn2.m_id;
}

bool
sd::core::operator<=(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return (bn1 == bn2) || (bn1 < bn2);
}

bool
sd::core::operator>=(const sd::core::ViewNode& bn1, const sd::core::ViewNode& bn2) {
  return (bn1 == bn2) || (bn1 > bn2);
}
