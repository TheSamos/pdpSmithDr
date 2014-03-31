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

#include "ViewId.hpp"

#include <utils/sdTypeUtils.hpp>

#include <iostream>

sd::core::ViewId::ViewId()
  : m_id(0), m_isBranchRoot(true)
{}

bool
sd::core::ViewId::isNull() const {
  return (m_id.size() == 0);
}

bool
sd::core::ViewId::isBranchRoot() const {
  return m_isBranchRoot;
}

std::string
sd::core::ViewId::toString() const {
  std::string result = "ID::";
  if (m_id.size() == 0)
    return result;
  result = result + m_id[0];
  std::vector<view_id_data_type>::const_iterator it = m_id.begin()+1;
  std::vector<view_id_data_type>::const_iterator end = m_id.end();
  for (; it != end; ++it)
    result = result + "." + *it;
  return result;
}

char
sd::core::ViewId::compare(const ViewId& vi1, const ViewId& vi2) {
  if (vi1.isNull() || vi2.isNull())
    return 2;
  
  if (vi1.m_id.size() <= vi2.m_id.size()) {
    
    unsigned int idIdx = vi1.m_id.size()-1;
    
    if (vi1.m_id.size() == vi2.m_id.size())
      return (vi1.m_id[idIdx] > vi2.m_id[idIdx]) ?  1
	: (vi1.m_id[idIdx] < vi2.m_id[idIdx]) ? -1
	: 0;
    
    if (vi1.m_id[idIdx] < vi2.m_id[idIdx])
      return -1;
    
    return 2;
    
    /*
    bool e = true;
    bool ge = true;
    bool le = true;
    std::vector<sd::core::ViewId::view_id_data_type>::const_iterator it1 = vi1.m_id.begin();
    std::vector<sd::core::ViewId::view_id_data_type>::const_iterator it2 = vi2.m_id.begin();
    const std::vector<sd::core::ViewId::view_id_data_type>::const_iterator end1 = vi1.m_id.end();
    for (; it1 != end1; ++it1, ++it2) {
      e &= (*it1 == *it2);
      ge &= (*it1 >= *it2);
      le &= (*it1 <= *it2);
    }
    
    if (e)
      return (vi1.m_id.size() == vi2.m_id.size()) ? 0 : 2;
    
    if (le)
      return -1;
    
    if (ge && vi1.m_id.size() == vi2.m_id.size())
      return 1;
    
    return 2;
    */
  } else {
    return ((char)-compare(vi2, vi1));
  }
}

sd::core::ViewId::ViewId(unsigned int size, bool isBranchRoot)
  : m_id(size, 0),
    m_isBranchRoot(isBranchRoot)
{}

sd::core::ViewId::ViewId(const ViewId& vi, bool isBranchRoot)
  : m_id(vi.m_id),
    m_isBranchRoot(isBranchRoot)
{}

bool
sd::core::comparable(const ViewId& vi1, const ViewId& vi2) {
  if (vi1.isNull() || vi2.isNull())
    return false;
  
  return ((ViewId::compare(vi2, vi1) < 2) && (ViewId::compare(vi2, vi1) > -2));
}

std::ostream&
sd::core::operator<<(std::ostream& os, const sd::core::ViewId& v) {
  os << v.toString();
  return os;
}

bool
sd::core::operator==(const sd::core::ViewId& vi1, const sd::core::ViewId& vi2) {
  return (ViewId::compare(vi1, vi2) == 0);
}

bool
sd::core::operator!=(const sd::core:: ViewId& vi1, const ViewId& vi2) {
  return !(vi1 == vi2);
}

bool
sd::core::operator<(const sd::core::ViewId& vi1, const sd::core::ViewId& vi2) {
  return (ViewId::compare(vi1, vi2) == -1);
}

bool
sd::core::operator>(const sd::core::ViewId& vi1, const sd::core::ViewId& vi2) {
  return (ViewId::compare(vi1, vi2) == 1);
}

bool
sd::core::operator<=(const sd::core::ViewId& vi1, const sd::core::ViewId& vi2) {
  return ((vi1 == vi2) || (ViewId::compare(vi1, vi2) == -1));
}

bool
sd::core::operator>=(const sd::core::ViewId& vi1, const sd::core::ViewId& vi2) {
  return ((vi1 == vi2) || (ViewId::compare(vi1, vi2) == 1));
}
