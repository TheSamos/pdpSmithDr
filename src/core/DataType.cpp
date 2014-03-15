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

#include "DataType.hpp"

#include <core/Vector3d.hpp>
#include <core/RGBCoordinates.hpp>
#include <core/DynamicDimensionalCoordinates.hpp>
#include <mesh/VertexFromMeshView.hpp>
#include <mesh/FaceFromMeshView.hpp>
#include <mesh/EdgeFromMeshView.hpp>
#include <mesh/Normal.hpp>
#include <mesh/TextureCoordinates.hpp>

#include <Clonable.hpp>

class ImageView;
class MeshView;

namespace sd {
  
  namespace core {
    
    DataType::id_type DataType::m_lastId = 0;
    
    std::map<std::string, DataType::id_type> DataType::m_nameMap;
    
    std::map<DataType::id_type, std::string> DataType::m_reverseNameMap;
    
    std::map<DataType::id_type, size_t> DataType::m_sizes;
    
    DataType
    DataType::getDataType(const std::string& str) {
      std::map<std::string, id_type>::const_iterator it = DataType::m_nameMap.find(str);
      if (it == DataType::m_nameMap.end())
	return DataType();
      return DataType((*it).second);
    }
    
    bool
    DataType::isSetDataType(const std::string& str) {
      return getDataType(str).isValid();
    }
    
    DataType::DataType()
      : m_id(0),
	m_isBasicType(false)
    {}
    
    bool
    DataType::isValid() const {
      return m_id != 0;
    }
    
    std::string
    DataType::toString() const {
      if (DataType::m_reverseNameMap.find(m_id)
	  != DataType::m_reverseNameMap.end())
	return DataType::m_reverseNameMap[m_id];
      return "UNDEFINED";
    }
    
    size_t
    DataType::size() const {
      if (DataType::m_sizes.find(m_id)
	  != DataType::m_sizes.end())
	return DataType::m_sizes[m_id];
      return 0;
    }
    
    DataType::id_type
    DataType::getNewId() {
      return ++m_lastId;
    }

    DataType::DataType(DataType::id_type id, bool isBasicType)
      : m_id(id),
	m_isBasicType(isBasicType)
    {}
    
    // Template class
    
    // Note: The first must be the most common/universal !
    
    std::vector<std::string> DataType_<bool>::m_stringValues = {"bool", "BOOL"};
    DataType::id_type DataType_<bool>::m_instanceId = DataType_<bool>::addToMap();

    std::vector<std::string> DataType_<INT8>::m_stringValues = {"char", "int8", "INT8"};
    DataType::id_type DataType_<INT8>::m_instanceId = DataType_<INT8>::addToMap();
    
    std::vector<std::string> DataType_<UINT8>::m_stringValues = {"uchar", "unsigned char", "uint8", "UINT8"};
    DataType::id_type DataType_<UINT8>::m_instanceId = DataType_<UINT8>::addToMap();
    
    std::vector<std::string> DataType_<INT16>::m_stringValues = {"short", "int16", "INT16"};
    DataType::id_type DataType_<INT16>::m_instanceId = DataType_<INT16>::addToMap();
    
    std::vector<std::string> DataType_<UINT16>::m_stringValues = {"ushort", "unsigned short", "uint16", "UINT16"};
    DataType::id_type DataType_<UINT16>::m_instanceId = DataType_<UINT16>::addToMap();
    
    std::vector<std::string> DataType_<INT32>::m_stringValues = {"int", "int32", "INT", "INT32"};
    DataType::id_type DataType_<INT32>::m_instanceId = DataType_<INT>::addToMap();
    
    std::vector<std::string> DataType_<UINT32>::m_stringValues = {"uint", "unsigned int", "uint32", "UINT", "UINT32"};
    DataType::id_type DataType_<UINT32>::m_instanceId = DataType_<UINT>::addToMap();
    
    std::vector<std::string> DataType_<INT64>::m_stringValues = {"long int", "long", "LONG", "INT64"};
    DataType::id_type DataType_<INT64>::m_instanceId = DataType_<INT64>::addToMap();
    
    std::vector<std::string> DataType_<UINT64>::m_stringValues = {"ulong", "unsigned long int", "unsigned long",
								  "ULONG", "UINT64"};
    DataType::id_type DataType_<UINT64>::m_instanceId = DataType_<UINT64>::addToMap();
    
    std::vector<std::string> DataType_<FLOAT>::m_stringValues = {"float", "float32", "FLOAT"};
    DataType::id_type DataType_<FLOAT>::m_instanceId = DataType_<FLOAT>::addToMap();
    
    std::vector<std::string> DataType_<DOUBLE>::m_stringValues = {"double", "double64", "float64", "DOUBLE"};
    DataType::id_type DataType_<DOUBLE>::m_instanceId = DataType_<DOUBLE>::addToMap();
    
    std::vector<std::string> DataType_<LONGDOUBLE>::m_stringValues = {"long double", "LONGDOUBLE"};
    DataType::id_type DataType_<LONGDOUBLE>::m_instanceId = DataType_<LONGDOUBLE>::addToMap();
    
    template<> unsigned int DataType_<std::string>::m_instanceId = getNewId();
    
    template<> unsigned int DataType_<ImageView*>::m_instanceId = getNewId();
    template<> unsigned int DataType_<MeshView*>::m_instanceId = getNewId();
    
    template<> unsigned int DataType_<void*>::m_instanceId = getNewId();
    template<> unsigned int DataType_<void>::m_instanceId = getNewId();
    
#define COMMA() ,
    
#define DECLARE_CONTAINER_AND_SIMPLE(T)					\
    unsigned int DataType_<T >::m_instanceId = getNewId();		\
    unsigned int DataType_<sd::core::DynamicDimensionalCoordinates<T> >::m_instanceId = getNewId();
    
#define DECLARE_COMPOSED(T)						\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::DynamicDimensionalCoordinates<T>) \
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::Vector3d<T>)			\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::Point3d<T>)			\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::VertexFromMeshView<T>)	\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::Normal<T>)			\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::RGBCoordinates<T>)		\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::TextureCoordinates<T>)	\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::EdgeFromMeshView<T>*)	\
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::FaceFromMeshView<T COMMA() 3>*) \
    DECLARE_CONTAINER_AND_SIMPLE(sd::core::FaceFromMeshView<T COMMA() 4>*)
    
    DECLARE_COMPOSED(bool)
    DECLARE_COMPOSED(sd::INT8)
    DECLARE_COMPOSED(sd::UINT8)
    DECLARE_COMPOSED(sd::INT16)
    DECLARE_COMPOSED(sd::UINT16)
    DECLARE_COMPOSED(sd::INT32)
    DECLARE_COMPOSED(sd::UINT32)
    DECLARE_COMPOSED(sd::INT64)
    DECLARE_COMPOSED(sd::UINT64)
    DECLARE_COMPOSED(sd::FLOAT)
    DECLARE_COMPOSED(sd::DOUBLE)
    DECLARE_COMPOSED(sd::LONGDOUBLE)
    
#undef DECLARE_COMPOSED
#undef DECLARE_CONTAINER_AND_SIMPLE
#undef COMMA
    
  }
  
}
