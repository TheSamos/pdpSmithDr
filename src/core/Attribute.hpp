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

#ifndef SD_CORE_MESH_ATTRIBUTE_HPP
#define SD_CORE_MESH_ATTRIBUTE_HPP

#include <core/Clonable.hpp>

#include <core/DataType.hpp>

#include <vector>
#include <map>

namespace sd {
  
  namespace core {
    
    typedef DataType AttributeKeyType;
    typedef DataType AttributeType;
    
    class Attribute : public Clonable {
      
    public:

      virtual ~Attribute() {}

      virtual Attribute* clone() const = 0;
      
      virtual void clear() = 0;
      
      virtual AttributeKeyType getAttributeKeyType() const = 0;
      
      virtual AttributeType getAttributeType() const = 0;
      
      virtual size_t size() const = 0;
      
    };
    
    template<typename K>
    class Attribute_ : public Attribute {

    public:

      virtual ~Attribute_() {}

      virtual Attribute_<K>* clone() const = 0;
      
      virtual AttributeKeyType getAttributeKeyType() const;
      
      virtual AttributeType getAttributeType() const = 0;
      
      virtual size_t size() const = 0;
      
      virtual void clear() = 0;
      
    };
    
    template<typename K>
    class AttributeCollection : public Attribute_<K> {
      
    public:

      virtual ~AttributeCollection() {}

      virtual AttributeCollection<K>* clone() const = 0;
      
      virtual AttributeType getAttributeType() const = 0;
      
      virtual size_t size() const = 0;
      
      virtual void clear() = 0;
      
      virtual bool isVector() = 0;
      
      virtual bool containsKey(const K& key) const = 0;
      
      virtual bool import(const K& key1, AttributeCollection<K>* att2, const K& key2) = 0;
      
      virtual bool swap(const K& key1, AttributeCollection<K>* att2, const K& key2) = 0;
      
    };
    
    template<typename K, typename T>
    class AttributeMap : public AttributeCollection<K> {

    public:
      
      typedef typename std::map<K, T>::iterator iterator;
      typedef typename std::map<K, T>::const_iterator const_iterator;
      
      AttributeMap(const std::map<K, T>& data = std::map<K, T>());
      
      inline virtual ~AttributeMap() {}

      inline virtual AttributeMap<K, T>* clone() const;
      
      inline virtual void clear();
      
      inline virtual AttributeType getAttributeType() const;
      
      inline std::map<K, T>& getData();
      inline const std::map<K, T>& getData() const;
      
      inline virtual size_t size() const;
      
      inline virtual bool containsKey(const K& key) const;
      
      inline T& operator[](const K& key);
      
      inline virtual bool isVector() {return false;}
      
      inline virtual bool import(const K& key1, AttributeCollection<K>* att2, const K& key2);
      
      inline virtual bool swap(const K& key1, AttributeCollection<K>* att2, const K& key2);
      
    private:
      
      std::map<K, T> m_attribute;
      
    };

    template<typename K, typename T>
    class AttributeMapPtr : public AttributeMap<K, T> {

    public:
      
      typedef typename AttributeMap<K, T>::iterator iterator;
      typedef typename AttributeMap<K, T>::const_iterator const_iterator;
      
      AttributeMapPtr(const std::map<K, T>& data = std::map<K, T>(), bool deleteOnRemove = true);
      
      explicit AttributeMapPtr(const AttributeMapPtr<K, T>& amp);
      
      virtual ~AttributeMapPtr();
      
      virtual AttributeMapPtr<K, T>* clone() const;
      
      virtual void clear();
      
      void setRemoveOnDelete(bool r);
      
      virtual bool import(const K& key1, AttributeCollection<K>* att2, const K& key2);
      
      virtual bool swap(const K& key1, AttributeCollection<K>* att2, const K& key2);
      
    private:
      
      bool m_removeOnDelete;
      
    };

    template<typename T>
    class AttributeVect : public AttributeCollection<UINT> {

    public:
      
      typedef typename std::vector<T>::iterator iterator;
      typedef typename std::vector<T>::const_iterator const_iterator;
      
      AttributeVect(unsigned int size = 0);
      
      AttributeVect(const std::vector<T>& data);
      
      inline virtual ~AttributeVect() {}

      inline virtual AttributeVect<T>* clone() const;
      
      inline virtual void clear();
      
      inline virtual AttributeType getAttributeType() const;
      
      inline std::vector<T>& getData();
      inline const std::vector<T>& getData() const;
      
      inline virtual size_t size() const;
      
      inline virtual bool containsKey(const UINT& key) const;
      
      inline T& operator[](UINT key);
      
      inline virtual bool isVector() {return true;}
      
      inline virtual bool import(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2);
      
      inline virtual bool swap(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2);
      
    private:
      
      std::vector<T> m_attribute;
      
    };

    template<typename T>
    class AttributeVectPtr : public AttributeVect<T> {

    public:
      
      typedef typename AttributeVect<T>::iterator iterator;
      typedef typename AttributeVect<T>::const_iterator const_iterator;
      
      AttributeVectPtr(unsigned int size = 0, bool removeOnDelete = true);
      
      AttributeVectPtr(const std::vector<T>& data, bool removeOnDelete = true);
      
      explicit AttributeVectPtr(const AttributeVectPtr<T>& avp);
      
      virtual ~AttributeVectPtr();
      
      virtual AttributeVectPtr<T>* clone() const;
      
      virtual void clear();
      
      void setRemoveOnDelete(bool r);
      
      virtual bool import(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2);
      
      virtual bool swap(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2);
      
    private:
      
      bool m_removeOnDelete;
      
    };

    template<typename T>
    class AttributeUnary : public Attribute_<void> {

    public:
      
      AttributeUnary(const T& value);
      
      virtual ~AttributeUnary();

      inline virtual AttributeType getAttributeType() const;
      
      virtual AttributeUnary<T>* clone() const;
      
      virtual void clear();
      
      T& operator*();
      
      const T& operator*() const;
      
      inline virtual size_t size() const {return 1;}
      
    private:
      
      T m_attribute;
      
    };

    template<typename T>
    class AttributeUnaryPtr : public AttributeUnary<T> {

    public:
      
      AttributeUnaryPtr(T value, bool removeOnDelete = true);
      
      explicit AttributeUnaryPtr(const AttributeUnaryPtr<T>& aup);
       
      virtual ~AttributeUnaryPtr();
      
      virtual AttributeUnaryPtr<T>* clone() const;
      
      virtual void clear();
      
      void setRemoveOnDelete(bool r);
      
    private:
      
      bool m_removeOnDelete;
      
    };

  }

}

#include "Attribute.hxx"

#endif /*! SD_CORE_MESH_ATTRIBUTE_HPP */
