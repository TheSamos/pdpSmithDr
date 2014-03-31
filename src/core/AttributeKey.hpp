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

#ifndef SD_CORE_ATTRIBUTE_KEY_HPP
#define SD_CORE_ATTRIBUTE_KEY_HPP

#include <string>
#include <vector>

namespace sd {

  namespace core {
    
    class AttributeKey;
    
    class AttributeKey {

    public:
      
      typedef unsigned int attribute_key_id;
      
      static const AttributeKey& getKey(const std::string& name);
      static const AttributeKey& getKey(attribute_key_id id);
      
      static bool addNewKey(const std::string& name);
      
      static bool containsKey(const std::string& name);
      
      static const AttributeKey& getKeyAndCreateIfNotExists(const std::string& name);
      
    public:
      
      AttributeKey(const AttributeKey& akey);
      
      const std::string& name() const;
      
      attribute_key_id id() const;
      
    private:
      
      AttributeKey() = delete;
      
      AttributeKey(const std::string& name);

    private:

      static std::vector<AttributeKey> m_existingKeys;
      static unsigned int m_nextId;

    private:
      
      std::string m_name;
      attribute_key_id m_id;
      
    };
    
    bool operator==(const AttributeKey& a1, const AttributeKey& a2);
    bool operator!=(const AttributeKey& a1, const AttributeKey& a2);
    
    bool operator<(const AttributeKey& a1, const AttributeKey& a2);
    bool operator>(const AttributeKey& a1, const AttributeKey& a2);
    
  }
  
}

#endif /* ! SD_CORE_ATTRIBUTE_KEY_HPP */
