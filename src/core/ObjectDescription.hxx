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

#ifndef SD_CORE_OBJECT_DESCRIPTION_HXX
#define SD_CORE_OBJECT_DESCRIPTION_HXX


template<typename ...VALUES>
sd::core::ObjectDescription
sd::core::ObjectDescription::MeshView(const sd::core::DataType& datatype,
				      int faceCard,
				      VALUES ...values) {
  ObjectDescription od = MeshView(datatype, faceCard);
  fill(od.m_attributes, std::forward<VALUES>(values)...);
  return od;
}

template<typename ...VALUES>
void
sd::core::ObjectDescription::fill(std::vector<sd::core::AttributeDescription>& attributes,
     const sd::core::AttributeDescription& ad, VALUES ...rest) {
  attributes.push_back(ad);
  fill(attributes, std::forward<VALUES>(rest)...);
}

template<typename ...VALUES>
void
sd::core::ObjectDescription::fill(std::vector<sd::core::AttributeDescription>&) {
  return;
}

#endif /*! SD_CORE_OBJECT_DESCRIPTION_HXX */
