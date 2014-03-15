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

#ifndef SD_FRONTEND_ANY_HXX
#define SD_FRONTEND_ANY_HXX

#include <sstream>

namespace sd {
  
  namespace frontend {

    template<typename ValueType>
    class Any::AnyHolder : public Any::PlaceHolder {

    public:

      AnyHolder(const ValueType& value)
	: m_held(value)
      {}

      virtual const std::type_info&
      typeInfo() const {
	return typeid(ValueType);
      }
      
      virtual bool isAnyType() const {
	return true;
      }
      
      virtual PlaceHolder*
      clone() const {
	return new AnyHolder(m_held);
      }

      std::string
      toString() const {
	return sd::utils::toString(m_held);
      }

    private:

      AnyHolder(const AnyHolder& rhs) = delete;
      AnyHolder& operator=(const AnyHolder& rhs) = delete;

    public:

      const ValueType m_held;

    };
    
    class Any::ObjectHolder : public Any::PlaceHolder {

    public:

      ObjectHolder(const sd::core::ObjectDescription& od)
	: m_held(od)
      {}

      virtual const std::type_info&
      typeInfo() const {
	return typeid(sd::core::ObjectDescription);
      }
      
      virtual bool isAnyType() const {
	return false;
      }
      
      virtual PlaceHolder*
      clone() const {
	return new ObjectHolder(m_held);
      }

      virtual std::string
      toString() const {
	return sd::utils::toString(m_held);
      }

      const sd::core::ObjectDescription&
      operator*() {return m_held;}
      
    private:

      ObjectHolder(const ObjectHolder& rhs) = delete;
      ObjectHolder& operator=(const ObjectHolder& rhs) = delete;

    public:

      const sd::core::ObjectDescription m_held;

    };
    
    Any::Any(const sd::core::ObjectDescription& od)
    : m_content(new ObjectHolder(od)) {}
    
    template<typename ValueType>
    Any::Any(const ValueType& value)
      : m_content(new AnyHolder<ValueType>(value)) {}
    
    Any::Any(const sd::core::ImageView* iv)
      : m_content((iv != NULL)
		  ? (PlaceHolder*) new ObjectHolder(sd::core::ObjectDescription::getObjectDescription(*iv)/*::ImageView(iv->channelType(),
										     (int) iv->nbDimensions(),
										     (int) iv->nbChannels())*/)
		  : (PlaceHolder*) new AnyHolder<const sd::core::ImageView*>(iv)) {}

    Any::Any(const sd::core::MeshView* mv)
      : m_content((mv != NULL)
		  ? (PlaceHolder*) new ObjectHolder(sd::core::ObjectDescription::getObjectDescription(*mv)/*::MeshView(mv->getVertexDataType(),
										     (int) mv->getFaceDegree())*/)
		  : (PlaceHolder*) new AnyHolder<const sd::core::MeshView*>(mv)) {}

    template<typename ValueType>
    Any&
    Any::operator=(const ValueType& rhs) {
      Any x(rhs);
      return swap(x);
    }

    template<typename ValueType>
    bool
    Any::isA() const {
      if (m_content && !m_content->isAnyType()) {
	  if (typeid(ValueType) == typeid(sd::core::ImageView*))
	    return (**static_cast<ObjectHolder*>(m_content)).isA(sd::core::ObjectDescription::ImageView());
	  if (typeid(ValueType) == typeid(sd::core::MeshView*))
	    return (**static_cast<ObjectHolder*>(m_content)).isA(sd::core::ObjectDescription::MeshView());
      }
      return ((m_content) ? m_content->typeInfo() : typeid(void)) == typeid(ValueType);
    }
    
    
    bool
    Any::isA(const sd::core::ObjectDescription& od) const {
      return ((m_content && !m_content->isAnyType())
	      && (**static_cast<ObjectHolder*>(m_content)).isA(od));
    }
    
    bool
    Any::containsA(const sd::core::ObjectDescription& od) const {
      return ((m_content && !m_content->isAnyType())
	      && od.isA((**static_cast<ObjectHolder*>(m_content))));
    }

    template<typename ValueType>
    bool
    Any::copyTo(ValueType& value) const {
      const ValueType* copyable = toPtr<ValueType>();
      if (copyable)
	value = *copyable;
      return copyable;
    }

    template<typename ValueType>
    const ValueType*
    Any::toPtr() const {
      return (m_content && m_content->isAnyType() && isA<ValueType>())
	? &static_cast<AnyHolder<ValueType>*>(m_content)->m_held
	: NULL;
    }
    
    const sd::core::ObjectDescription*
    Any::toPtr() const {
      return (m_content && !m_content->isAnyType() && isA<sd::core::ObjectDescription>())
	? &static_cast<ObjectHolder*>(m_content)->m_held
	: NULL;
    }

    template<typename ValueType>
    ValueType
    anyCast(const Any& operand) {
      const ValueType* result = operand.toPtr<ValueType>();
      return result ? *result : throw std::bad_cast();
    }

    template<typename ResultType, typename ArgType>
    ResultType
    interpretCast(const ArgType& arg) {
      std::stringstream st;
      st << arg;
      ResultType result = ResultType();
      st >> result;
      return result;
    }

  }
}

#endif /*! SD_FRONTEND_ANY_HXX */
