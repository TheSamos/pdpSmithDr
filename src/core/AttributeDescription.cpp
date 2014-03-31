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

#include "AttributeDescription.hpp"

const sd::core::AttributeType sd::core::AttributeDescription::ANY_DATATYPE;

sd::core::AttributeDescription::AttributeDescription(std::string fieldName,
						      AttributeField attributeField,
						      bool isBasicType,
						      const AttributeType& dataType,
						      std::string attributeName)
  : m_fieldName(fieldName),
    m_attributeField(attributeField),
    m_isBasicType(isBasicType),
    m_dataType(dataType),
    m_attributeName(attributeName)
{}

const std::string&
sd::core::AttributeDescription::fieldName() const {
  return m_fieldName;
}
      
sd::core::AttributeDescription::AttributeField
sd::core::AttributeDescription::attributeField() const {
  return m_attributeField;
}

const std::string&
sd::core::AttributeDescription::attributeName() const {
  return m_attributeName;
}

bool
sd::core::AttributeDescription::isA(const AttributeDescription& ad) const {
  bool result = (m_attributeField == (ad.m_attributeField));
  result &= (!(ad.m_isBasicType) || (ad.m_isBasicType && m_isBasicType));
  result &= ((ad.m_dataType == ANY_DATATYPE) || (m_dataType == ad.m_dataType));
  result &= ((ad.m_attributeName == "") || (m_attributeName == ad.m_attributeName));
  return result;
}

bool
sd::core::AttributeDescription::containsOneOf(const std::vector<AttributeDescription>& attributes) const {
  bool result = false;
  for (unsigned int i=0; i<attributes.size(); ++i)
    result |= attributes[i].isA(*this);
  return result;
}
      
std::ostream&
sd::core::operator<<(std::ostream& os, const AttributeDescription& ad) {
  os << ad.m_attributeField << ".";
  os << ad.m_isBasicType << ".";
  os << ad.m_dataType.toString() << ".";
  os << ad.m_attributeName << " ";
  return os;
}
