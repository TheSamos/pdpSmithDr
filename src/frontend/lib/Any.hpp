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

#ifndef SD_FRONTEND_ANY_HPP
#define SD_FRONTEND_ANY_HPP

#include <SmithDRDefs.hpp>

#include <core/ObjectDescription.hpp>
#include <core/image/ImageView.hpp>
#include <core/mesh/MeshView.hpp>

#include <typeinfo>
#include <ostream>

namespace sd {
  
  namespace frontend {

    class Any {

    public:

      Any();
      inline Any(const sd::core::ObjectDescription& );
      inline Any(const sd::core::ImageView*);
      inline Any(const sd::core::MeshView*);
      template<typename ValueType> explicit Any(const ValueType& value);
      
      Any(const Any& x);
      
      ~Any();

      Any& operator=(const Any& rhs);
      template<typename ValueType> Any& operator=(const ValueType& value);

      template<typename ValueType> bool isA() const;
      inline bool isA(const sd::core::ObjectDescription& od) const;
      inline bool containsA(const sd::core::ObjectDescription& od) const;
      
      template<typename ValueType> bool copyTo(ValueType& value) const;
      template<typename ValueType> const ValueType* toPtr() const;
      inline const sd::core::ObjectDescription* toPtr() const;
      operator const void*() const;
      
      std::ostream& operator<<(std::ostream& o) const;

    private:
      
      Any& swap(Any& rhs);
      //const std::type_info& typeInfo() const;
      
      class PlaceHolder {

      public:

	virtual ~PlaceHolder() {}
	virtual const std::type_info& typeInfo() const = 0;
	virtual bool isAnyType() const = 0;
	virtual PlaceHolder* clone() const = 0;
	virtual std::string toString() const = 0;

      };

      template<typename ValueType>
      class AnyHolder;
      
      class ObjectHolder;
      
    private:

      PlaceHolder* m_content;

    };

    template<typename ValueType> ValueType anyCast(const Any& operand);

    template<typename ResultType, typename ArgType> ResultType interpretCast(const ArgType& arg);
    
  }
  
}

#include "Any.hxx"

#endif /* ! SD_FRONTEND_ANY_HPP */
