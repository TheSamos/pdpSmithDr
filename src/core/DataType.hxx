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

#ifndef SD_CORE_DATA_TYPE_HXX
#define SD_CORE_DATA_TYPE_HXX

#include <Types.hpp>

namespace sd {
  
  namespace core {

    template<typename T>
    class Point3d;
    template<typename T>
    class Vector3d;
    template<typename T>
    class RGBCoordinates;
    template<typename T>
    class DynamicDimensionalCoordinates;
    template<typename T>
    class VertexFromMeshView;
    template<typename T, unsigned int N>
    class FaceFromMeshView;
    template<typename T>
    class EdgeFromMeshView;
    template<typename T>
    class Normal;
    template<typename T>
    class TextureCoordinates;


    inline DataType::~DataType() {}

    inline bool
    operator==(const DataType& t1, const DataType& t2) {
      return t1.m_id == t2.m_id;
    }

    inline bool
    operator!=(const DataType& t1, const DataType& t2) {
      return !(t1 == t2);
    }
    
    inline bool
    operator<(const DataType& t1, const DataType& t2) {
      return t1.m_id < t2.m_id;
    }

    inline bool
    operator>(const DataType& t1, const DataType& t2) {
      return t1.m_id > t2.m_id;
    }

    // Template class
    
    template<typename T>
    class DataType_ : public DataType {
      
    public:

      typedef T type;
      typedef T sub_type;
      typedef T work_type;
      
      DataType_() : DataType(m_instanceId, false) {}

    private:
      
      static id_type m_instanceId;
    
    };

#define SD_DEF_DATA_CONTAINER(T, CONTAINER)		\
    template<>						\
    class DataType_<CONTAINER > : public DataType {	\
							\
    public:						\
							\
    typedef CONTAINER type;				\
    typedef T sub_type;					\
    typedef CONTAINER work_type;			\
							\
    DataType_() : DataType(m_instanceId, false) {}	\
							\
    private:						\
							\
    static id_type m_instanceId;			\
							\
    };

#define COMMA() ,

#define SD_DEF_DATA_CONTAINERS(T, U)					\
    SD_DEF_DATA_CONTAINER(T, U)						\
    SD_DEF_DATA_CONTAINER(U, sd::core::DynamicDimensionalCoordinates<U >)

#define SD_DEF_DATA_TYPE(T, U)						\
    template<>								\
    class DataType_<T> : public DataType {				\
    									\
    public:								\
    									\
    typedef T type;							\
    typedef T sub_type;							\
    typedef U work_type;						\
									\
    DataType_() : DataType(m_instanceId, true) {}			\
       									\
    private:								\
									\
    inline static int addToMap() {					\
      id_type instanceId = getNewId();					\
      std::vector<std::string>::const_iterator it = m_stringValues.begin(); \
      const std::vector<std::string>::const_iterator end = m_stringValues.end(); \
      for (; it != end; ++it)						\
	m_nameMap[*it] = instanceId;					\
      m_reverseNameMap[instanceId] = m_stringValues[0];			\
      m_sizes[instanceId] = sizeof(T);					\
      return instanceId;						\
    }									\
									\
    private:								\
    									\
    static id_type m_instanceId;					\
    static std::vector<std::string> m_stringValues;			\
									\
    };									\
    SD_DEF_DATA_CONTAINERS(T, sd::core::DynamicDimensionalCoordinates<T>) \
    SD_DEF_DATA_CONTAINERS(T, sd::core::Vector3d<T>)			\
    SD_DEF_DATA_CONTAINERS(T, sd::core::Point3d<T>)			\
    SD_DEF_DATA_CONTAINERS(T, sd::core::VertexFromMeshView<T>)		\
    SD_DEF_DATA_CONTAINERS(T, sd::core::Normal<T>)			\
    SD_DEF_DATA_CONTAINERS(T, sd::core::RGBCoordinates<T>)		\
    SD_DEF_DATA_CONTAINERS(T, sd::core::TextureCoordinates<T>)		\
    SD_DEF_DATA_CONTAINERS(T, sd::core::EdgeFromMeshView<T>*)		\
    SD_DEF_DATA_CONTAINER(T, sd::core::FaceFromMeshView<T COMMA() 3>*)	\
    SD_DEF_DATA_CONTAINER(sd::core::FaceFromMeshView<T COMMA() 3>*,	\
			  sd::core::DynamicDimensionalCoordinates<sd::core::FaceFromMeshView<T COMMA() 3>* >) \
    SD_DEF_DATA_CONTAINER(T, sd::core::FaceFromMeshView<T COMMA() 4>*)	\
    SD_DEF_DATA_CONTAINER(sd::core::FaceFromMeshView<T COMMA() 4>*,	\
			  sd::core::DynamicDimensionalCoordinates<sd::core::FaceFromMeshView<T COMMA() 4>* >)

    SD_DEF_DATA_TYPE(bool, bool)
    SD_DEF_DATA_TYPE(INT8, INT)
    SD_DEF_DATA_TYPE(UINT8, UINT)
    SD_DEF_DATA_TYPE(INT16, INT)
    SD_DEF_DATA_TYPE(UINT16, UINT)
    SD_DEF_DATA_TYPE(INT32, INT64)
    SD_DEF_DATA_TYPE(UINT32, UINT64)
    SD_DEF_DATA_TYPE(INT64, INT64)
    SD_DEF_DATA_TYPE(UINT64, UINT64)
    SD_DEF_DATA_TYPE(FLOAT, DOUBLE)
    SD_DEF_DATA_TYPE(DOUBLE, DOUBLE)
    SD_DEF_DATA_TYPE(LONGDOUBLE, LONGDOUBLE)
    
#undef SD_DEF_DATA_TYPE
#undef SD_DEF_DATA_CONTAINERS
#undef SD_DEF_DATA_CONTAINER
#undef COMMA

    template<typename T>
    bool
    DataType::isOfType() const {
      return *this == DataType_<T>();
    }
    
  }
}

#endif /*! SD_CORE_DATA_TYPE_HXX */
