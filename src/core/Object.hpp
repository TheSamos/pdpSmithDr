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

#ifndef SD_CORE_OBJECT_HPP
#define SD_CORE_OBJECT_HPP

#include <string>
#include <typeinfo>

#include "Types.hpp"

namespace sd {
  
  namespace core {
    
    class Object {
      
    public:
      
      inline Object(ObjectType sdType, const std::string& name);
      inline Object(const Object& x);
      
      virtual inline ~Object() = 0;
      
      inline Object& operator=(const Object& x);
      
      inline const std::string& name() const;
      inline void setName(const std::string& name);
      
      inline ObjectType getSDType() const;
      inline const std::type_info* dataTypeOut() const;
      inline const std::type_info* dataTypeIn() const;
      
    protected:
      
      inline void unSetType();
      
    private:
      
      ObjectType m_sdType;
      std::string m_name;
      
    };
    
  }
  
}

#include "Object.hxx"

#endif /* ! SD_CORE_OBJECT_HPP */
