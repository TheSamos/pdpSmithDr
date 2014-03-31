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

#include "ViewDAG.hpp"

#include "ViewNode.hpp"

#include <cassert>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

sd::core::ViewDAG* sd::core::ViewDAG::m_instance = new sd::core::ViewDAG();

sd::core::ViewDAG::~ViewDAG() {
  while (m_views.begin() != m_views.end()) {
    delete (*(m_views.begin())).second;
  }
  while (m_ghostNodes.begin() != m_ghostNodes.end()) {
    delete *(m_ghostNodes.begin());
  }
}

sd::core::ViewDAG::ViewDAG() : m_views(),
			       m_ghostNodes(),
			       m_lastCard(0)
{}

std::vector<sd::core::ViewNode*>
sd::core::ViewDAG::roots() {
  std::vector<ViewNode*> result;
  std::map<ViewId, ViewNode*>::iterator it = m_views.begin();
  const std::map<ViewId, ViewNode*>::iterator end = m_views.end();
  for (; it!=end; ++it) {
    if ((*it).first.isBranchRoot())
      result.push_back((*it).second);
  }
  return result;
}
      
sd::core::ViewId
sd::core::ViewDAG::getNewRootId() {
  ViewId result(++m_lastCard, true);
  result.m_id.back() = 1;
#ifdef DEBUG
  std::cerr << "getNewRootId : " << result.toString() << std::endl;
#endif
  return result;
}

sd::core::ViewId
sd::core::ViewDAG::getIncrementedId(const ViewId& parent) {
  ViewId result(parent, false);
  ++(result.m_id.back());
#ifdef DEBUG
  std::cerr << "getIncrementedId : " << result.toString() << std::endl;
#endif
  return result;
}

sd::core::ViewId
sd::core::ViewDAG::getNewBranchId(const ViewId& parent) {
  ViewId result(++m_lastCard, true);
  std::vector<sd::core::ViewId::view_id_data_type>::const_iterator it1 = parent.m_id.begin();
  const std::vector<sd::core::ViewId::view_id_data_type>::const_iterator end1 = parent.m_id.end();
  std::vector<sd::core::ViewId::view_id_data_type>::iterator it2 = result.m_id.begin();
  for (; it1 != end1; ++it1, ++it2) {
    *it2 = *it1;
  }
  --it2;
  ++(*it2);
  result.m_id.back() = 1;
#ifdef DEBUG
  std::cerr << "getNewBranchId with parent : " << result.toString() << std::endl;
#endif
  return result;
}

sd::core::ViewId
sd::core::ViewDAG::getNewBranchId(const std::vector<const ViewId*>& parents) {
  ViewId result(++m_lastCard, true);
  std::vector<const ViewId*>::const_iterator it = parents.begin();
  const std::vector<const ViewId*>::const_iterator end = parents.end();
  for (; it != end; ++it) {
    std::vector<sd::core::ViewId::view_id_data_type>::const_iterator it1 = (*it)->m_id.begin();
    const std::vector<sd::core::ViewId::view_id_data_type>::const_iterator end1 = (*it)->m_id.end();
    std::vector<sd::core::ViewId::view_id_data_type>::iterator it2 = result.m_id.begin();
    for (; it1 != end1; ++it1, ++it2) {  
      *it2 = std::max(*it2, *it1);
    }
    --it2;
    ++(*it2);
  }
  result.m_id.back() = 1;
#ifdef DEBUG
  std::cerr << "getNewBranchId with parents : " << result.toString() << std::endl;
#endif
  return result;
}

void
sd::core::ViewDAG::add(ViewNode* node) {
  assert(node);
  if (node->getId().isNull()) {
    m_ghostNodes.push_back(node);
  } else if (!node->getId().isNull()) {
    m_views[node->getId()] = node;
  }
}
/*
void
sd::core::ViewDAG::remove(ViewNode* node) {
  release(node);
  delete node;
}
*/
void
sd::core::ViewDAG::release(ViewNode* node) {
  assert(node);
  if (node->getId().isNull()) {
    std::vector<ViewNode*>::iterator it = std::find(m_ghostNodes.begin(), m_ghostNodes.end(), node);
    if(it != m_ghostNodes.end())
      m_ghostNodes.erase(it);
  } else {
    if(m_views.find(node->getId()) != m_views.end())
      m_views.erase(node->getId());
  }
}
      
