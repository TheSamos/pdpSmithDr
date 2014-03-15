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

#ifndef SD_CORE_DATA_TYPE_HPP
#define SD_CORE_DATA_TYPE_HPP

#include <vector>
#include <map>
#include <string>

namespace sd {
  namespace core {

    class DataType;
    
    class DataType {

    public:

      inline virtual ~DataType();
      
      static DataType getDataType(const std::string& str);
      
      static bool isSetDataType(const std::string& str);

      DataType();
      
      bool isValid() const;
      
      inline bool isBasicType() const {return m_isBasicType;}
      
      std::string toString() const;
      
      size_t size() const;
      
      template<typename T>
      bool isOfType() const;
      
    protected:

      typedef unsigned int id_type;
      
      DataType(id_type id, bool isBasicType = false);
      static id_type getNewId();

      friend bool operator==(const DataType& t1, const DataType& t2);
      friend bool operator<(const DataType& t1, const DataType& t2);
      friend bool operator>(const DataType& t1, const DataType& t2);
      
    private:
      
      static void staticInit();
      
    protected:
      
      static std::map<std::string, id_type> m_nameMap;
      static std::map<id_type, std::string> m_reverseNameMap;
      static std::map<id_type, size_t> m_sizes;
      
    private:

      static id_type m_lastId;
      id_type m_id;
      bool m_isBasicType;

    };

    bool operator==(const DataType& t1, const DataType& t2);
    bool operator!=(const DataType& t1, const DataType& t2);
    bool operator<(const DataType& t1, const DataType& t2);
    bool operator>(const DataType& t1, const DataType& t2);

  }
}

#include "DataType.hxx"

#endif /*! SD_CORE_DATA_TYPE_HPP */
