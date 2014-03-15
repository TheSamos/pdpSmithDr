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

#ifndef SD_CORE_ATTRIBUTE_DESCRIPTION_HPP
#define SD_CORE_ATTRIBUTE_DESCRIPTION_HPP

#include <string>
#include <typeinfo>

#include <Types.hpp>
#include <core/DataType.hpp>

#include <core/mesh/MeshView_.hpp>

namespace sd {
  
  namespace core {
    
    class AttributeDescription;
    
    class AttributeDescription {
      
    public:
      
      typedef enum{GLOBAL, VERTICES, FACES, EDGES} AttributeField;
      
      static const sd::core::AttributeType ANY_DATATYPE;
      
      template<typename T, unsigned int N>
      static void getAttributesOf(const sd::core::MeshView_<T, N>& mesh,
				  std::vector<AttributeDescription>& result);
      
      AttributeDescription(std::string fieldName,
			   AttributeField attributeField,
			   bool isBasicType = false,
			   const AttributeType& dataType = AttributeDescription::ANY_DATATYPE,
			   std::string attributeName = "");
      
      AttributeField attributeField() const;

      const std::string& fieldName() const;
      
      const std::string& attributeName() const;
      
      bool isA(const AttributeDescription& ad) const;
      
      bool containsOneOf(const std::vector<AttributeDescription>& attributes) const;
      
      std::set<AttributeDescription> getCompatiblesKeysIn(const std::set<AttributeDescription>& keys) const;
      
    private:

      AttributeDescription() = delete;

      friend std::ostream& operator<<(std::ostream& os, const AttributeDescription&);
      
    private:
      
      std::string m_fieldName;
      AttributeField m_attributeField;
      bool m_isBasicType;
      AttributeType m_dataType;
      std::string m_attributeName;
      
    };
    
    std::ostream& operator<<(std::ostream& os, const AttributeDescription&);
    
  }
  
}

#include "AttributeDescription.hxx"

#endif /*! SD_CORE_ATTRIBUTE_DESCRIPTION_HPP */
