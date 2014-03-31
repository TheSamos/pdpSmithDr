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

#include "ObjectDescription.hpp"

#include <core/image/ImageView.hpp>
#include <core/mesh/MeshView.hpp>

const sd::core::DataType sd::core::ObjectDescription::ANY_DATATYPE;
//const int sd::core::ObjectDescription::ANY_CARD = -1;

sd::core::ObjectDescription
sd::core::ObjectDescription::ImageView(const DataType& datatype,
				       int nbDims,
				       int nbChannels) {
  ObjectType objectType = sd::ImageObjectType;
  std::vector<DataType> types(1, datatype);
  std::vector<int> cards(2);
  cards[0] = nbDims;
  cards[1] = nbChannels;
  return ObjectDescription(objectType, types, cards);
}

sd::core::ObjectDescription
sd::core::ObjectDescription::MeshView(const DataType& datatype,
				      int faceCard) {
  ObjectType objectType = sd::MeshObjectType;
  std::vector<DataType> types(1, datatype);
  std::vector<int> cards(1, faceCard);
  return ObjectDescription(objectType, types, cards);
}

sd::core::ObjectDescription
sd::core::ObjectDescription::getObjectDescription(const sd::core::Object& obj) {
  ObjectType objectType = obj.getSDType();
  std::vector<DataType> types;
  std::vector<int> cards;
  std::vector<AttributeDescription> attributes;
  
  if (objectType == sd::ImageObjectType) {
    types.push_back(static_cast<const sd::core::ImageView&>(obj).channelType());
    cards.push_back(static_cast<const sd::core::ImageView&>(obj).nbDimensions());
    cards.push_back(static_cast<const sd::core::ImageView&>(obj).nbChannels());
  } else if (objectType == sd::MeshObjectType) {
    types.push_back(static_cast<const sd::core::MeshView&>(obj).getVertexDataType());
    cards.push_back(static_cast<const sd::core::MeshView&>(obj).getFaceDegree());
    {
    #define SD_DEF_MESH_TYPE(T, N)					\
      if (types[0] == DataType_<T>()) {					\
	AttributeDescription::getAttributesOf(static_cast<const sd::core::MeshView_<T, N>& >(obj), attributes); \
      }									\
      
#define SD_DEF_MESH_CARD(N)			\
      if (cards[0] == N) {			\
	SD_DEF_MESH_TYPE(double, N)		\
	  SD_DEF_MESH_TYPE(float, N)		\
	  SD_DEF_MESH_TYPE(char, N)		\
	  SD_DEF_MESH_TYPE(unsigned char, N)	\
	  SD_DEF_MESH_TYPE(int, N)		\
	  SD_DEF_MESH_TYPE(unsigned int, N)	\
	  SD_DEF_MESH_TYPE(size_t, N)		\
	  }								
      
      SD_DEF_MESH_CARD(3)
	SD_DEF_MESH_CARD(4)
	 
#undef SD_DEF_MESH_CARD
#undef SD_DEF_MESH_TYPE
	}
    
  }
  return ObjectDescription(objectType, types, cards, attributes);
}
      
sd::ObjectType
sd::core::ObjectDescription::objectType() const {
  return m_objectType;
}

bool
sd::core::ObjectDescription::isA(const ObjectDescription& od) const {
  bool result = (m_objectType == (od.m_objectType));
  for (unsigned int i=0; i<od.m_types.size(); ++i)
    result &= ((od.m_types[i] == ANY_DATATYPE) || (m_types[i] == od.m_types[i]));
  for (unsigned int i=0; i<od.m_cards.size(); ++i)
    result &= ((od.m_cards[i] == ANY_CARD) || (m_cards[i] == od.m_cards[i]));
  for (unsigned int i=0; i<od.m_attributes.size(); ++i)
    result &= od.m_attributes[i].containsOneOf(m_attributes);
  return result;
}

unsigned int
sd::core::ObjectDescription::getOptionalKeysNb() const {
  unsigned int result=0;
  for (unsigned int i=0; i<m_attributes.size(); ++i)
    if (m_attributes[i].attributeName().empty())
      ++result;
  return result;
}

std::vector<std::string>
sd::core::ObjectDescription::getOptionalKeysNames() const {
  std::vector<std::string> result;
  for (unsigned int i=0; i<m_attributes.size(); ++i)
    if (m_attributes[i].attributeName().empty())
      result.push_back(m_attributes[i].fieldName());
  return result;
}

void
sd::core::ObjectDescription::getOptionalKeys(const sd::core::Object& obj, std::vector<std::vector<std::string> >& atts) const {
  getOptionalKeys(getObjectDescription(obj), atts);
}

void
sd::core::ObjectDescription::getOptionalKeys(const sd::core::ObjectDescription& od, std::vector<std::vector<std::string> >& atts) const {
  for (unsigned int i=0; i<m_attributes.size(); ++i) {
    if (m_attributes[i].attributeName().empty()) {
      atts.push_back(std::vector<std::string>());
      for (unsigned int j=0; j<od.m_attributes.size(); ++j)
	if (od.m_attributes[j].isA(m_attributes[i]))
	  atts.back().push_back(od.m_attributes[j].attributeName());
    }
  }
}

sd::core::ObjectDescription::ObjectDescription(ObjectType objectType,
					       const std::vector<DataType>& types,
					       const std::vector<int>& cards,
					       std::vector<AttributeDescription> attributes)
  : m_objectType(objectType),
    m_types(types),
    m_cards(cards),
    m_attributes(attributes)
{}

std::ostream&
sd::core::operator<<(std::ostream& os, const ObjectDescription& od) {
  os << od.m_objectType;
  for (unsigned int i=0; i<od.m_types.size(); ++i)
    os << od.m_types[i].toString();
  for (unsigned int i=0; i<od.m_cards.size(); ++i)
    os << od.m_cards[i];
  return os;
}
    
