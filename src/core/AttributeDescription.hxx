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

#ifndef SD_CORE_ATTRIBUTE_DESCRIPTION_HXX
#define SD_CORE_ATTRIBUTE_DESCRIPTION_HXX

template<typename T, unsigned int N>
void
sd::core::AttributeDescription::getAttributesOf(const sd::core::MeshView_<T, N>& mesh,
						std::vector<AttributeDescription>& result) {
  AttributeField attributeField = VERTICES;
  const std::set<AttributeKey> aKeys = mesh.getVerticesAttributesKeys();
  std::set<AttributeKey>::const_iterator it = aKeys.begin();
  const std::set<AttributeKey>::const_iterator end = aKeys.end();
  for (; it!=end; ++it) {
    AttributeType dataType = mesh.getVerticesAttributeDataTypeAtKey(*it);
    bool isBasicType = dataType.isBasicType();
    std::string name = (*it).name();
    result.push_back(AttributeDescription("", attributeField, isBasicType, dataType, name));
  }
}
    
#endif /*! SD_CORE_ATTRIBUTE_DESCRIPTION_HXX */
