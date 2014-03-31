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

#ifndef SD_CORE_OBJECT_DESCRIPTION_HPP
#define SD_CORE_OBJECT_DESCRIPTION_HPP

#include <string>
#include <typeinfo>

#include <Types.hpp>
#include <core/DataType.hpp>

#include "AttributeDescription.hpp"

namespace sd {
  
  namespace core {
    
    class ObjectDescription;
    
    class ObjectDescription {
      
    public:
      
      static const sd::core::DataType ANY_DATATYPE;
      static const int ANY_CARD = -1;
      
      static ObjectDescription ImageView(const DataType& datatype = ObjectDescription::ANY_DATATYPE,
					 int nbDims = ObjectDescription::ANY_CARD,
					 int nbChannels = ObjectDescription::ANY_CARD);
      
      template<typename ...VALUES>
      static ObjectDescription MeshView(const DataType& datatype,
					int faceCard,
					VALUES ...values);
      
      static ObjectDescription MeshView(const DataType& datatype = ObjectDescription::ANY_DATATYPE,
					int faceCard = ObjectDescription::ANY_CARD);
      
      static ObjectDescription getObjectDescription(const sd::core::Object& obj);
      
      ObjectType objectType() const;
      
      bool isA(const ObjectDescription& od) const;
      
      unsigned int getOptionalKeysNb() const;
      
      std::vector<std::string> getOptionalKeysNames() const;
      
      void getOptionalKeys(const sd::core::Object& obj, std::vector<std::vector<std::string> >& atts) const;
      
      void getOptionalKeys(const sd::core::ObjectDescription& od, std::vector<std::vector<std::string> >& atts) const;
      
    private:
      
      ObjectDescription() = delete;
      
      ObjectDescription(ObjectType objectType,
			const std::vector<DataType>& types,
			const std::vector<int>& cards,
			std::vector<AttributeDescription> attributes = std::vector<AttributeDescription>());

      template<typename ...VALUES>
      static void fill(std::vector<AttributeDescription>& attributes, const AttributeDescription& ad, VALUES ...rest);
      
      template<typename ...VALUES>
      static void fill(std::vector<AttributeDescription>& attributes);

      friend std::ostream& operator<<(std::ostream& os, const ObjectDescription&);
      
    private:
      
      ObjectType m_objectType;
      std::vector<DataType> m_types;
      std::vector<int> m_cards;
      std::vector<AttributeDescription> m_attributes;
      
    };
    
    std::ostream& operator<<(std::ostream& os, const ObjectDescription&);
    
  }
  
}

#include "ObjectDescription.hxx"

#endif /*! SD_CORE_OBJECT_DESCRIPTION_HPP */
