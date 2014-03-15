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

#include "AttributeKey.hpp"

sd::core::AttributeKey::attribute_key_id sd::core::AttributeKey::m_nextId = 0;
std::vector<sd::core::AttributeKey> sd::core::AttributeKey::m_existingKeys = {sd::core::AttributeKey("Undefined"),
									      sd::core::AttributeKey("Selected"),
									      sd::core::AttributeKey("Normal"),
									      sd::core::AttributeKey("Color"),
									      sd::core::AttributeKey("Texture")};

const sd::core::AttributeKey&
sd::core::AttributeKey::getKey(const std::string& dname) {
  for (unsigned int i=1; i<m_existingKeys.size(); ++i)
    if (m_existingKeys[i].name() == dname) return m_existingKeys[i];
  return m_existingKeys[0];
}

const sd::core::AttributeKey&
sd::core::AttributeKey::getKey(attribute_key_id id) {
  if (m_existingKeys.size() > id)
    return m_existingKeys[id];
  return m_existingKeys[0];
}

bool
sd::core::AttributeKey::addNewKey(const std::string& name) {
  if(containsKey(name))
    return false;
  m_existingKeys.push_back(AttributeKey(name));
  return true;
}

bool
sd::core::AttributeKey::containsKey(const std::string& name) {
  return getKey(name) != sd::core::AttributeKey::m_existingKeys[0];
}

const sd::core::AttributeKey&
sd::core::AttributeKey::getKeyAndCreateIfNotExists(const std::string& name) {
  if (!sd::core::AttributeKey::containsKey(name))
    sd::core::AttributeKey::addNewKey(name);
  return sd::core::AttributeKey::getKey(name);
}

sd::core::AttributeKey::AttributeKey(const AttributeKey& akey)
  : m_name(akey.m_name),
    m_id(akey.m_id)
{}

const std::string&
sd::core::AttributeKey::name() const {
  return m_name;
}

unsigned int
sd::core::AttributeKey::id() const {
  return m_id;
}

sd::core::AttributeKey::AttributeKey(const std::string& sname)
  : m_name(sname),
    m_id(m_nextId++)
{}

bool
sd::core::operator==(const AttributeKey& a1, const AttributeKey& a2) {
  return a1.id() == a2.id();
}

bool
sd::core::operator!=(const AttributeKey& a1, const AttributeKey& a2) {
  return !(a1 == a2);
}

bool
sd::core::operator<(const AttributeKey& a1, const AttributeKey& a2) {
  return a1.id() < a2.id();
}

bool
sd::core::operator>(const AttributeKey& a1, const AttributeKey& a2) {
  return a1.id() > a2.id();
}
