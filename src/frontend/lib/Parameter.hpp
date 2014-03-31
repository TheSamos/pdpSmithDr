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

#ifndef SD_FRONTEND_PARAMETER_HPP
#define SD_FRONTEND_PARAMETER_HPP

#include <SmithDRDefs.hpp>
#include <string>
#include <vector>
#include <ostream>
#include <limits>
#include "Any.hpp"

#include <vector>

namespace sd {
  
  namespace frontend {
 
    class Parameter
    {
      
    public:

      template<typename Type>
      Parameter(std::string name, const Type& val, std::string description = "");
      
      template<typename Type>
      Parameter(std::string name, const Type& val, const Type& mini, const Type& maxi, std::string description = "");
      
      template<typename Type>
      Parameter(std::string name, const Type& val, const Any& mini, const Type& maxi, std::string description = "");
      
      template<typename Type>
      Parameter(std::string name, const Type& val, const Type& mini, const Any& maxi, std::string description = "");

      template<typename Type>
      Parameter(std::string name, const Type& val, const Any& mini, const Any& maxi, std::string description = "");
      
      // val must be in predefinedValues
      template<typename Type>
      Parameter(std::string name, const Type& val, const std::vector<Type>& predefinedValues, std::string description = "");
      
      template<typename Type>
      Parameter(std::string name, const std::vector<Type>& val, std::string description = "");
      
      Parameter(std::string name = "");
      Parameter(const Parameter& p);
      
      ~Parameter();
      
      Parameter& operator=(const Parameter& p);

      // val must be in predefinedValues (if any), and in the range [min,max] (if defined)
      template<typename Type>
      Parameter& operator=(const Type& val);
      
      template<typename Type>
      Parameter& operator=(const std::vector<Type>& val);
      
      inline std::string name() const;
      inline void setName(std::string name);
      
      inline std::string getDescription() const;
      inline void setDescription(std::string description);
      
      inline bool hasPredefinedValues() const;
      
      inline bool isMinDefined() const;
      inline bool isMaxDefined() const;
      template<typename Type> void setMinMax(const Type& mini, const Type& maxi);
      template<typename Type> void setMinMax(const std::vector<Type>& mini, const std::vector<Type>& maxi) = delete;
      void setMinMax(const bool&, const bool&) = delete;
      void setMinMax(const std::string&, const std::string&) = delete;
      
      inline std::string getWidgetFactoryName() const;
      inline void setWidgetFactoryName(std::string name);
      
      // Manage const char* as std::string
      Parameter(std::string name, const char* val, std::string description = "");
      Parameter(std::string name, const char* val, const std::vector<char*>& predefinedValues, std::string description = "") = delete;
      // val must be in predefinedValues (if any)
      Parameter& operator=(const char* val);
      void setMinMax(const char*, const char*) = delete;
      
      // Parameters cannot be pointers!!
      // will produce a compilation error if trying to create or access a parameter with a pointer as its value
      template<typename Type>
      Parameter(std::string name, Type* val, std::string description = "") = delete;
      template<typename Type>
      Parameter(std::string name, std::vector<Type>* val, std::string description = "") = delete;
      template<typename Type>
      Parameter(std::string name, std::vector<Type*> val, std::string description = "") = delete;
      template<typename Type>
      Parameter(std::string name, std::vector<Type*>* val, std::string description = "") = delete;
      
      bool isBoolean() const;
      bool isInteger() const;
      bool isFloat() const;
      bool isString() const;
      bool isAtomic() const;
      
      bool isBooleanVector() const;
      bool isIntegerVector() const;
      bool isFloatVector() const;
      bool isStringVector() const;
      bool isVector() const;
      
      // throw a bad_cast exception if Type does not match real type exactly!
      // For example, casting from int to long raises an exception.
      template<typename Type> Type getAs() const;
      template<typename Type> std::vector<Type> getAsVectorOf() const;
      template<typename Type> Type getMin() const;
      template<typename Type> Type getMax() const;
      template<typename Type> void getMinMaxAs(Type& mini, Type& maxi) const;
      template<typename Type> std::vector<Type> getPredefinedValuesAsVectorOf() const;
      
      // throw a bad_cast exception if parameter's type does is not compatible
      bool getBoolean() const; // bool-->bool (no cast)
      long getInteger() const; // {all integral types}-->long (cast)
      double getFloat() const; // {all floating point types}-->double (cast)
      std::string getString() const; // std::string-->std::string (no cast)
      
    private:
      
      template<typename T> friend bool isTypeOf(const Parameter&);
      template<typename T> friend bool isVectorOf(const Parameter&);
      
      friend std::ostream& operator<<(std::ostream& os, const Parameter& p);
      
    public:
      
      static const Any undefined;

      void setType(std::string);
      std::string getType();
      
    private:
      std::string m_name;
      Any m_value;
      std::string m_description;
      bool m_hasPredefinedValues;
      Any m_predefinedValues;
      Any m_min, m_max;
      std::string m_widgetFactoryName;

      std::string m_type;
      
    };
    
    typedef std::vector<Parameter> ParameterList;
    
    // Parameters are a series of name,value, name,value, ...
    template<typename ...Params>
    ParameterList
    make_parameter_list(Params ...params);
    
  }
  
}

#include "Parameter.hxx"

#endif /*! SD_FRONTEND_PARAMETER_HPP */
