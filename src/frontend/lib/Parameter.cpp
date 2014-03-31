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

#include "Parameter.hpp"

#include <algorithm>
#include <cassert>

namespace sd {
  
  namespace frontend {

    /*
     * Valid types
     */

    // integers
    typedef bool               BOOL;
    typedef char               CHAR;
    typedef signed char        SIGNED_CHAR;
    typedef unsigned char      UNSIGNED_CHAR;
    typedef wchar_t            WCHAR;
    typedef short              SHORT;
    typedef int                INT;
    typedef long               LONG;
    typedef long long          LONG_LONG;
    typedef unsigned short     UNSIGNED_SHORT;
    typedef unsigned int       UNSIGNED_INT;
    typedef unsigned long      UNSIGNED_LONG;
    typedef unsigned long long UNSIGNED_LONG_LONG;

    // float points
    typedef float              FLOAT;
    typedef double             DOUBLE;
    typedef long double        LONG_DOUBLE;

    // others
    typedef std::string        STD_STRING;


#define BUILD_PARAMETER(Type)						\
    template Parameter::Parameter(std::string, const Type&, std::string = ""); \
    template Parameter::Parameter(std::string, const Type&, const Type&, const Type&, std::string = ""); \
    template Parameter::Parameter(std::string, const Type&, const Any&, const Type&, std::string = ""); \
    template Parameter::Parameter(std::string, const Type&, const Type&, const Any&, std::string = ""); \
    template Parameter::Parameter(std::string, const Type&, const Any&, const Any&, std::string = ""); \
									\
    template Parameter::Parameter(std::string, const Type&, const std::vector<Type>&, std::string = ""); \
    template Parameter& Parameter::operator=(const Type&);		\
    template void Parameter::setMinMax(const Type&, const Type&);	\
									\
    template bool isTypeOf<Type>(const Parameter& p);			\
    template Parameter::Parameter(std::string, const std::vector<Type>&, std::string = ""); \
    template Parameter& Parameter::operator=(const std::vector<Type>&);	\
    template bool isVectorOf<Type>(const Parameter& p);

#define MAKE_TEMPLATED(Type)						\
    BUILD_PARAMETER(Type)						\
    template Type Parameter::getAs<Type>() const;			\
    template Type Parameter::getMin() const;				\
    template Type Parameter::getMax() const;				\
    template void Parameter::getMinMaxAs(Type&, Type&) const;		\
    template std::vector<Type> Parameter::getAsVectorOf<Type>() const;	\
    template std::vector<Type> Parameter::getPredefinedValuesAsVectorOf() const;


    // integers
    MAKE_TEMPLATED(BOOL)
    MAKE_TEMPLATED(CHAR)
    MAKE_TEMPLATED(SIGNED_CHAR)
    MAKE_TEMPLATED(UNSIGNED_CHAR)
    MAKE_TEMPLATED(WCHAR)
    MAKE_TEMPLATED(SHORT)
    MAKE_TEMPLATED(INT)
    MAKE_TEMPLATED(LONG)
    MAKE_TEMPLATED(LONG_LONG)
    MAKE_TEMPLATED(UNSIGNED_SHORT)
    MAKE_TEMPLATED(UNSIGNED_INT)
    MAKE_TEMPLATED(UNSIGNED_LONG)
    MAKE_TEMPLATED(UNSIGNED_LONG_LONG)

    // float points
    MAKE_TEMPLATED(FLOAT)
    MAKE_TEMPLATED(DOUBLE)
    MAKE_TEMPLATED(LONG_DOUBLE)

    // others
    MAKE_TEMPLATED(STD_STRING)

#undef MAKE_TEMPLATED
#undef BUILD_PARAMETER

    /*
     * ValueSet implementation
     */

    template<typename T>
    class ValueSet {

    public:

      ValueSet(const std::vector<T>& v);
      ValueSet(const ValueSet<T>& v);
      ~ValueSet();

      ValueSet<T>& operator=(const ValueSet<T>& v);

      template<typename U> friend std::ostream& operator<<(std::ostream& os, const ValueSet<U>& v);

      const std::vector<T>& values() const { return m_values; }

    private:

      std::vector<T> m_values;

    };

    template<typename T>
    ValueSet<T>::ValueSet(const std::vector<T>& v)
    : m_values(v) {}

    template<typename T>
    ValueSet<T>::ValueSet(const ValueSet<T>& v)
      : m_values(v.m_values) {}

    template<typename T>
    ValueSet<T>::~ValueSet() {}

    template<typename T>
    ValueSet<T>&
    ValueSet<T>::operator=(const ValueSet<T>& v) {
      if (&v != this)
	m_values = v.m_values;
      return *this;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const ValueSet<U>& v) {
      if (v.m_values.empty()) {
	os << "[ empty ]";
	return os;
      }

      os << "[ ";
      for (auto it = v.m_values.begin(); it != v.m_values.end(); ++it) {
	if (it != v.m_values.begin())
	  os << "  ";
	os << *it;
      }
      os << " ]";

      return os;
    }

    /*
     * Parameter implementation
     */

    const Any Parameter::undefined = Any(std::numeric_limits<double>::max());

    // this code does not use constructor delegation since it is only
    // available for gcc version >= 4.7

    // default type is int
    Parameter::Parameter(std::string name)
      //: Parameter(name, 0, "") // delegation
      : m_name(name), m_value(0), m_description(""),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(undefined), m_max(undefined),
	m_widgetFactoryName() {}

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val, std::string description)
    //: Parameter(name, val, undefined, undefined, description) // delegation
      : m_name(name), m_value(val), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(undefined), m_max(undefined),
	m_widgetFactoryName() {}

    Parameter::Parameter(std::string name, const char* val, std::string description)
      //: Parameter(name, std::string(val), description) // delegation
      : m_name(name), m_value(std::string(val)), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(undefined), m_max(undefined),
	m_widgetFactoryName() {}

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val, const Type& mini, const Type& maxi, std::string description)
      //: Parameter(name, val, Any(mini), Any(maxi), description) // delegation
      : m_name(name), m_value(val), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(mini), m_max(maxi),
	m_widgetFactoryName() {}

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val, const Any& mini, const Type& maxi, std::string description)
      //: Parameter(name, val, mini, Any(maxi), description) // delegation
      : m_name(name), m_value(val), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(mini), m_max(maxi),
	m_widgetFactoryName() {}

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val, const Type& mini, const Any& maxi, std::string description)
      //: Parameter(name, val, Any(mini), maxi, description) // delegation
      : m_name(name), m_value(val), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(mini), m_max(maxi),
	m_widgetFactoryName() {}

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val, const Any& mini, const Any& maxi, std::string description)
      // no delegation here
      : m_name(name), m_value(val), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(mini), m_max(maxi),
	m_widgetFactoryName() {}

    namespace {

      template<typename Type>
      void
      setValue(Any& result, const Type& val,const std::vector<Type>& predef) {
	typename std::vector<Type>::const_iterator it = std::find(predef.begin(), predef.end(), val);
	if (it != predef.end())
	  result = Any(val);
	else if (!predef.empty())
	  result = Any(predef[0]);
	else {
	  assert(!"Should never get here!");
	  result = Any();
	}
	
      }

    }

    template<typename Type>
    Parameter::Parameter(std::string name, const Type& val,
			 const std::vector<Type>& predefinedValues,
			 std::string description)
      // no delegation here
      : m_name(name), m_value(), m_description(description),
	m_hasPredefinedValues(true), m_predefinedValues(ValueSet<Type>(predefinedValues)),
	m_min(undefined), m_max(undefined),
	m_widgetFactoryName() {
      setValue(m_value, val, predefinedValues);
    }

    template<typename Type>
    Parameter::Parameter(std::string name, const std::vector<Type>& val,
			 std::string description)
      //: Parameter(name, ValueSet<Type>(val), description) // delegation
      : m_name(name), m_value(ValueSet<Type>(val)), m_description(description),
	m_hasPredefinedValues(false), m_predefinedValues(),
	m_min(undefined), m_max(undefined),
	m_widgetFactoryName() {}

    Parameter::Parameter(const Parameter& p)
      : m_name(p.m_name), m_value(p.m_value), m_description(p.m_description),
	m_hasPredefinedValues(p.m_hasPredefinedValues),
	m_predefinedValues(p.m_predefinedValues),
	m_min(p.m_min), m_max(p.m_max),
	m_widgetFactoryName(p.m_widgetFactoryName) {}

    Parameter::~Parameter() {}

    Parameter&
    Parameter::operator=(const Parameter& p) {
      if (&p != this) {
	m_name = p.m_name;
	m_value = p.m_value;
	m_description = p.m_description;
	m_hasPredefinedValues = p.m_hasPredefinedValues;
	m_predefinedValues = p.m_predefinedValues;
	m_min = p.m_min;
	m_max = p.m_max;
	m_widgetFactoryName = p.m_widgetFactoryName;
      }
      return *this;
    }

    template<typename Type>
    Parameter&
    Parameter::operator=(const Type& val) {
      if (m_hasPredefinedValues) {
	const std::vector<Type>& predef = getPredefinedValuesAsVectorOf<Type>();
	setValue(m_value, val, predef);
      }
      else
	m_value = Any(val);

      return *this;
    }

    template<typename Type>
    Parameter&
    Parameter::operator=(const std::vector<Type>& val) {
      m_value = Any(ValueSet<Type>(val));
      return *this;
    }

    Parameter&
    Parameter::operator=(const char* val) {
      m_value = Any(std::string(val));
      return *this;
    }

    template<typename Type>
    void
    Parameter::setMinMax(const Type& mini, const Type& maxi) {
      m_min = Any(mini);
      m_max = Any(maxi);
    }

    bool
    Parameter::isBoolean() const {
      return (isTypeOf<BOOL>(*this));
    }

    bool
    Parameter::isInteger() const {
      return (isTypeOf<CHAR>(*this) ||
	      isTypeOf<SIGNED_CHAR>(*this) ||
	      isTypeOf<UNSIGNED_CHAR>(*this) ||
	      isTypeOf<WCHAR>(*this) ||
	      isTypeOf<SHORT>(*this) ||
	      isTypeOf<INT>(*this) ||
	      isTypeOf<LONG>(*this) ||
	      isTypeOf<LONG_LONG>(*this) ||
	      isTypeOf<UNSIGNED_SHORT>(*this) ||
	      isTypeOf<UNSIGNED_INT>(*this) ||
	      isTypeOf<UNSIGNED_LONG>(*this) ||
	      isTypeOf<UNSIGNED_LONG_LONG>(*this)
	      );
    }

    bool
    Parameter::isFloat() const {
      return (isTypeOf<FLOAT>(*this) ||
	      isTypeOf<DOUBLE>(*this) ||
	      isTypeOf<LONG_DOUBLE>(*this)
	      );
    }

    bool
    Parameter::isString() const {
      return (isTypeOf<STD_STRING>(*this));
    }

    bool
    Parameter::isAtomic() const {
      return (isBoolean() ||
	      isInteger() ||
	      isFloat() ||
	      isString()
	      );
    }

    bool
    Parameter::isBooleanVector() const {
      return (isVectorOf<BOOL>(*this));
    }

    bool
    Parameter::isIntegerVector() const {
      return (isVectorOf<CHAR>(*this) ||
	      isVectorOf<SIGNED_CHAR>(*this) ||
	      isVectorOf<UNSIGNED_CHAR>(*this) ||
	      isVectorOf<WCHAR>(*this) ||
	      isVectorOf<SHORT>(*this) ||
	      isVectorOf<INT>(*this) ||
	      isVectorOf<LONG>(*this) ||
	      isVectorOf<LONG_LONG>(*this) ||
	      isVectorOf<UNSIGNED_SHORT>(*this) ||
	      isVectorOf<UNSIGNED_INT>(*this) ||
	      isVectorOf<UNSIGNED_LONG>(*this) ||
	      isVectorOf<UNSIGNED_LONG_LONG>(*this)
	      );
    }

    bool
    Parameter::isFloatVector() const {
      return (isVectorOf<FLOAT>(*this) ||
	      isVectorOf<DOUBLE>(*this) ||
	      isVectorOf<LONG_DOUBLE>(*this)
	      );
    }

    bool
    Parameter::isStringVector() const {
      return (isVectorOf<STD_STRING>(*this));
    }

    bool
    Parameter::isVector() const {
      return (isBooleanVector() ||
	      isIntegerVector() ||
	      isFloatVector() ||
	      isStringVector()
	      );
    }

    template<typename Type>
    Type
    Parameter::getAs() const {
      return anyCast<Type>(m_value);
    }

    template<typename Type>
    Type
    Parameter::getMin() const {
      return anyCast<Type>(m_min);
    }

    template<typename Type>
    Type
    Parameter::getMax() const {
      return anyCast<Type>(m_max);
    }

    template<typename Type>
    void
    Parameter::getMinMaxAs(Type& mini, Type& maxi) const {
      mini = anyCast<Type>(m_min);
      maxi = anyCast<Type>(m_max);
    }
    
    template<typename Type>
    std::vector<Type>
    Parameter::getAsVectorOf() const {
      return anyCast<ValueSet<Type> >(m_value).values();
    }
    
    template<typename Type>
    std::vector<Type>
    Parameter::getPredefinedValuesAsVectorOf() const {
      return anyCast<ValueSet<Type> >(m_predefinedValues).values();
    }
    
    template<typename Type>
    bool
    isTypeOf(const Parameter& p) {
      return p.m_value.isA<Type>();
    }
    
    template<typename Type>
    bool
    isVectorOf(const Parameter& p) {
      assert(isTypeOf<ValueSet<Type> >(p));
      return p.m_value.isA<ValueSet<Type> >();
    }

    bool
    Parameter::getBoolean() const {
      assert(isTypeOf<bool>(*this));
      return getAs<bool>();
    }

    long
    Parameter::getInteger() const {
      if (isTypeOf<CHAR>(*this))
	return (long) getAs<CHAR>();
      if (isTypeOf<SIGNED_CHAR>(*this))
	return (long) getAs<SIGNED_CHAR>();
      if (isTypeOf<UNSIGNED_CHAR>(*this))
	return (long) getAs<UNSIGNED_CHAR>();
      if (isTypeOf<WCHAR>(*this))
	return (long) getAs<WCHAR>();
      if (isTypeOf<SHORT>(*this))
	return (long) getAs<SHORT>();
      if (isTypeOf<INT>(*this))
	return (long) getAs<INT>();
      if (isTypeOf<LONG>(*this))
	return (long) getAs<LONG>();
      if (isTypeOf<LONG_LONG>(*this))
	return (long) getAs<LONG_LONG>();
      if (isTypeOf<UNSIGNED_SHORT>(*this))
	return (long) getAs<UNSIGNED_SHORT>();
      if (isTypeOf<UNSIGNED_INT>(*this))
	return (long) getAs<UNSIGNED_INT>();
      if (isTypeOf<UNSIGNED_LONG>(*this))
	return (long) getAs<UNSIGNED_LONG>();
      if (isTypeOf<UNSIGNED_LONG_LONG>(*this))
	return (long) getAs<UNSIGNED_LONG_LONG>();
      throw std::bad_cast();
    }

    double
    Parameter::getFloat() const {
      if (isTypeOf<FLOAT>(*this))
	return (double) getAs<FLOAT>();
      if (isTypeOf<DOUBLE>(*this))
	return (double) getAs<DOUBLE>();
      if (isTypeOf<LONG_DOUBLE>(*this))
	return (double) getAs<LONG_DOUBLE>();
      throw std::bad_cast();
    }

    std::string
    Parameter::getString() const {
      return getAs<STD_STRING>();
    }

    void Parameter::setType(std::string type)
    {
      m_type = type;
    }

    std::string Parameter::getType()
    {
      return m_type;
    }

    std::ostream&
    operator<<(std::ostream& os, const Parameter& p) {
      os << "Parameter: [name=" << p.name() << " val=";
      os << p.m_value;
      os << " description=" << p.getDescription() << " ]";
      return os;
    }
    
  }
  
}
