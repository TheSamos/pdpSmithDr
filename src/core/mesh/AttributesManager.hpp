/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_CORE_ATTRIBUTES_MANAGER_HPP
#define SD_CORE_ATTRIBUTES_MANAGER_HPP

#include <core/SharedObject.hpp>

#include <core/Attribute.hpp>
#include <core/AttributeKey.hpp>

#include <map>

namespace sd {
  
  namespace core {
    
    template<class C, typename E>
    class AttributesManager : public SharedObject<typename std::map<AttributeKey::attribute_key_id,
								    std::map<C*, AttributeCollection<E*>* > > > {
      
    private:
      
      typedef AttributeKey::attribute_key_id attribute_key_id;
      typedef typename std::map<attribute_key_id, std::map<C*, AttributeCollection<E*>* > > map_attributes;
      
    public:
      
      virtual ~AttributesManager();
      
      AttributesManager();
      
      AttributesManager(const sd::core::AttributesManager<C, E>& am);

      inline std::map<C*, AttributeCollection<E* >* >& getAttributes(const AttributeKey& akey, const C* container);
      inline void setAttributes(const AttributeKey& akey, AttributeCollection<E* >* attribute, const C* container);
      
      inline void importParentAttribute(const sd::core::AttributeKey& akey, const C* container);
      inline bool removeAttributeKey(const AttributeKey& akey, const C* container);
      inline void removeAttributes(const C* container);
      
      inline std::set<AttributeKey> getAttributesKeys(const E* e, const C* container) const;
      inline AttributeType getAttributeDataTypeAtKey(const E* e, const AttributeKey& akey, const C* container) const;
      
      inline bool isAttributeSet(const E* e, const sd::core::AttributeKey& akey, const C* container) const;
      template<typename A>
      inline const A& getAttribute(const E* e,  const AttributeKey& akey, const C* container);
      template<typename A>
      inline void setAttribute(const E* e, const AttributeKey& akey, const A& val, const C* container);
      inline bool importAttributes(E* e1, const E* e2, const C* container);
      
    private:
      
      map_attributes* m_attributes;
      
    };

  }

}

#include "AttributesManager.hxx"

#endif /*! SD_CORE_ATTRIBUTES_MANAGER_HPP */
