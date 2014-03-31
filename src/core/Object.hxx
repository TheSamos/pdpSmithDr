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

#ifndef SD_CORE_OBJECT_HXX
#define SD_CORE_OBJECT_HXX

namespace sd {
  
  namespace core {
    
    inline Object::Object(ObjectType sdType, const std::string& name)
      : m_sdType(sdType),
	m_name(name)
    {}
    
    inline Object::Object(const Object& x)
      : m_sdType(x.m_sdType),
	m_name(x.m_name)
    {}
    
    inline Object::~Object() {}
    
    inline Object&
    Object::operator=(const Object& x) {
      if (&x != this) {
	m_sdType = x.m_sdType;
	m_name = x.m_name;
      }
      return *this;
    }
    
    inline const std::string&
    Object::name() const {
      return m_name;
    }
    
    inline ObjectType
    Object::getSDType() const {
      return m_sdType;
    }
    
    inline void
    Object::setName(const std::string& name) {
      m_name = name;
    }
    
    inline void
    Object::unSetType() {
      m_sdType = sd::ObjectType::UnknownType;
    }
    
  }
  
}

#endif /*! SD_CORE_OBJECT_HXX */
