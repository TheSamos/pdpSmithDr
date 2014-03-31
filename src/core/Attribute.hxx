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

#ifndef SD_CORE_MESH_ATTRIBUTE_HXX
#define SD_CORE_MESH_ATTRIBUTE_HXX

#include <cassert>

template<typename K>
sd::core::AttributeKeyType
sd::core::Attribute_<K>::getAttributeKeyType() const {
  return DataType_<K>();
}

// ==============================================================

template<typename K, typename T>
sd::core::AttributeMap<K, T>::AttributeMap(const std::map<K, T>& data)
  : m_attribute(data)
{}

template<typename K, typename T>
sd::core::AttributeMap<K, T>*
sd::core::AttributeMap<K, T>::clone() const {
  return new AttributeMap<K, T>(m_attribute);
}

template<typename K, typename T>
sd::core::AttributeType
sd::core::AttributeMap<K, T>::getAttributeType() const {
  return DataType_<T>();
}

template<typename K, typename T>
std::map<K, T>&
sd::core::AttributeMap<K, T>::getData() {
  return m_attribute;
}

template<typename K, typename T>
const std::map<K, T>&
sd::core::AttributeMap<K, T>::getData() const {
  return m_attribute;
}

template<typename K, typename T>
size_t
sd::core::AttributeMap<K, T>::size() const {
  return m_attribute.size();
}

template<typename K, typename T>
bool
sd::core::AttributeMap<K, T>::containsKey(const K& key) const {
  return m_attribute.find(key) != m_attribute.end();
}

template<typename K, typename T>
void
sd::core::AttributeMap<K, T>::clear() {
  m_attribute.clear();
}

template<typename K, typename T>
T&
sd::core::AttributeMap<K, T>::operator[](const K& key) {
  return m_attribute[key];
}

template<typename K, typename T>
bool
sd::core::AttributeMap<K, T>::import(const K& key1, AttributeCollection<K>* att2, const K& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeMap<K, T>* att2Map = (AttributeMap<K, T>*)att2;
    (*this)[key1] = (*att2Map)[key2];
    return true;
  }
  return false;
}

template<typename K, typename T>
bool
sd::core::AttributeMap<K, T>::swap(const K& key1, AttributeCollection<K>* att2, const K& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeMap<K, T>* att2Map = (AttributeMap<K, T>*)att2;
    T t = (*this)[key1];
    (*this)[key1] = (*att2Map)[key2];
    (*att2Map)[key2] = t;
    return true;
  }
  return false;
}

// ==============================================================

template<typename K, typename T>
sd::core::AttributeMapPtr<K, T>::AttributeMapPtr(const std::map<K, T>& data,
						 bool removeOnDelete)
  : sd::core::AttributeMap<K, T>(data),
    m_removeOnDelete(removeOnDelete)
{assert(!m_removeOnDelete || T::isClonable());}

template<typename K, typename T>
sd::core::AttributeMapPtr<K, T>::~AttributeMapPtr() {
  clear();
}

template<typename K, typename T>
sd::core::AttributeMapPtr<K, T>::AttributeMapPtr(const sd::core::AttributeMapPtr<K, T>& amp)
  : AttributeMap<K, T>(amp),
    m_removeOnDelete(amp.m_removeOnDelete) {
  assert(!m_removeOnDelete || T::isClonable());
  if (m_removeOnDelete) {
    iterator it = (*this).getData().begin();
    const iterator end = (*this).getData().end();
    for (; it != end; ++it)
      (*it).second = (*it).second->clone();
  }
}

template<typename K, typename T>
sd::core::AttributeMapPtr<K, T>*
sd::core::AttributeMapPtr<K, T>::clone() const {
  return new AttributeMapPtr<K, T>(*this);
}

template<typename K, typename T>
void
sd::core::AttributeMapPtr<K, T>::clear() {
  if (m_removeOnDelete) {
    iterator it = (*this).getData().begin();
    const iterator end = (*this).getData().end();
    for (; it != end; ++it)
      delete (*it).second;
  }
  AttributeMap<K, T>::clear();
}

template<typename K, typename T>
void
sd::core::AttributeMapPtr<K, T>::setRemoveOnDelete(bool r) {
  assert(!m_removeOnDelete || T::isClonable());
  m_removeOnDelete = r;
}

template<typename K, typename T>
bool
sd::core::AttributeMapPtr<K, T>::import(const K& key1, AttributeCollection<K>* att2, const K& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeMapPtr<K, T>* att2Map = dynamic_cast<AttributeMapPtr<K, T>*>(att2);
    if ((*this).m_removeOnDelete || (*att2Map).m_removeOnDelete)
      (*this)[key1] = (*att2Map)[key2]->clone();
    else
      (*this)[key1] = (*att2Map)[key2];
    return true;
  }
  return false;
}

template<typename K, typename T>
bool
sd::core::AttributeMapPtr<K, T>::swap(const K& key1, AttributeCollection<K>* att2, const K& key2) {
  AttributeMapPtr<K, T>* att2Map = dynamic_cast<AttributeMapPtr<K, T>*>(att2);
  if (att2Map &&
      ((*this).m_removeOnDelete && (*att2Map).m_removeOnDelete) ||
      (!(*this).m_removeOnDelete && !(*att2Map).m_removeOnDelete))
    return AttributeMap<K, T>::swap(key1, att2, key2);
  return false;
}

// ==============================================================


template<typename T>
sd::core::AttributeVect<T>::AttributeVect(unsigned int size)
  : m_attribute(size)
{}

template<typename T>
sd::core::AttributeVect<T>::AttributeVect(const std::vector<T>& data)
  : m_attribute(data)
{}

template<typename T>
sd::core::AttributeVect<T>*
sd::core::AttributeVect<T>::clone() const {
  return new AttributeVect<T>(m_attribute);
}

template<typename T>
void
sd::core::AttributeVect<T>::clear() {
  m_attribute.clear();
}

template<typename T>
sd::core::AttributeType
sd::core::AttributeVect<T>::getAttributeType() const {
  return DataType_<T>();
}

template<typename T>
std::vector<T>&
sd::core::AttributeVect<T>::getData() {
  return m_attribute;
}

template<typename T>
const std::vector<T>&
sd::core::AttributeVect<T>::getData() const {
  return m_attribute;
}

template<typename T>
size_t
sd::core::AttributeVect<T>::size() const {
  return m_attribute.size();
}

template<typename T>
bool
sd::core::AttributeVect<T>::containsKey(const UINT& key) const {
  return key < m_attribute.size();
}

template<typename T>
T&
sd::core::AttributeVect<T>::operator[](UINT key) {
  return m_attribute[key];
}

template<typename T>
bool
sd::core::AttributeVect<T>::import(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeVect<T>* att2Vect = (AttributeVect<T>*)att2;
    if (m_attribute.size() <= key1)
      m_attribute.resize(key1+1);
    assert(key1 < m_attribute.size());
    assert(key1 < (*this).size());
    assert(key2 < (*att2Vect).size());
    m_attribute[key1] = (*att2Vect)[key2];
    return true;
  }
  return false;
}

template<typename T>
bool
sd::core::AttributeVect<T>::swap(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeVect<T>* att2Vect = (AttributeVect<T>*)att2;
    assert(key1 < (*this).size());
    assert(key2 < (*att2Vect).size());
    T t = (*this)[key1];
    (*this)[key1] = (*att2Vect)[key2];
    (*att2Vect)[key2] = t;
    return true;
  }
  return false;
}

// ==============================================================

template<typename T>
sd::core::AttributeVectPtr<T>::AttributeVectPtr(unsigned int size,
						 bool removeOnDelete)
  : sd::core::AttributeVect<T>(size),
    m_removeOnDelete(removeOnDelete)
{}

template<typename T>
sd::core::AttributeVectPtr<T>::AttributeVectPtr(const std::vector<T>& data,
						 bool removeOnDelete)
  : sd::core::AttributeVect<T>(data),
    m_removeOnDelete(removeOnDelete)
{}

template<typename T>
sd::core::AttributeVectPtr<T>::AttributeVectPtr(const sd::core::AttributeVectPtr<T>& avp)
  : AttributeVect<T>(avp),
    m_removeOnDelete(avp.m_removeOnDelete) {
  if (m_removeOnDelete) {
    iterator it = (*this).getData().begin();
    const iterator end = (*this).getData().end();
    for (; it != end; ++it)
      (*it) = (*it)->clone();
  }
}

template<typename T>
sd::core::AttributeVectPtr<T>::~AttributeVectPtr() {
  clear();
}

template<typename T>
sd::core::AttributeVectPtr<T>*
sd::core::AttributeVectPtr<T>::clone() const {
  return new AttributeVectPtr<T>(*this);
}

template<typename T>
void
sd::core::AttributeVectPtr<T>::clear() {
  if (m_removeOnDelete) {
    iterator it = (*this).getData().begin();
    const iterator end = (*this).getData().end();
    for (; it != end; ++it)
      delete *it;
  }
  AttributeVect<T>::clear();
}

template<typename T>
bool
sd::core::AttributeVectPtr<T>::import(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2) {
  if ((*this).getAttributeType() == (*att2).getAttributeType()) {
    AttributeVectPtr<T>* att2Vect = dynamic_cast<AttributeVectPtr<T>*>(att2);
    if (att2Vect &&
	((*this).m_removeOnDelete || (*att2Vect).m_removeOnDelete))
      (*this)[key1] = (*att2Vect)[key2]->clone();
    else
      (*this)[key1] = (*att2Vect)[key2];
    return true;
  }
  return false;
}

template<typename T>
bool
sd::core::AttributeVectPtr<T>::swap(const UINT& key1, AttributeCollection<UINT>* att2, const UINT& key2) {
  AttributeVectPtr<T>* att2Vect = dynamic_cast<AttributeVectPtr<T>*>(att2);
  if (att2Vect &&
      (((*this).m_removeOnDelete && (*att2Vect).m_removeOnDelete) ||
       (!(*this).m_removeOnDelete && !(*att2Vect).m_removeOnDelete)))
    return AttributeVect<T>::swap(key1, att2, key2);
  return false;
}

// ==============================================================

template<typename T>
sd::core::AttributeUnary<T>::AttributeUnary(const T& value)
  : m_attribute(value)
{}

template<typename T>
sd::core::AttributeUnary<T>::~AttributeUnary() {}

template<typename T>
sd::core::AttributeType
sd::core::AttributeUnary<T>::getAttributeType() const {
  return DataType_<T>();
}

template<typename T>
sd::core::AttributeUnary<T>*
sd::core::AttributeUnary<T>::clone() const {
  return new AttributeUnary<T>(*this);
}

template<typename T>
void
sd::core::AttributeUnary<T>::clear() {
  m_attribute = T();
}

template<typename T>
T&
sd::core::AttributeUnary<T>::operator*() {
  return m_attribute;
}

template<typename T>
const T&
sd::core::AttributeUnary<T>::operator*() const {
  return m_attribute;
}

// ==============================================================

template<typename T>
sd::core::AttributeUnaryPtr<T>::AttributeUnaryPtr(T value,
						  bool removeOnDelete)
  : AttributeUnary<T>(value),
    m_removeOnDelete(removeOnDelete)
{}

template<typename T>
sd::core::AttributeUnaryPtr<T>::AttributeUnaryPtr(const sd::core::AttributeUnaryPtr<T>& aup)
  : AttributeUnary<T>(aup),
    m_removeOnDelete(aup.m_removeOnDelete) {
  if (m_removeOnDelete)
    *(*this).getData() = (*this).getData()->clone();
}

template<typename T>
sd::core::AttributeUnaryPtr<T>::~AttributeUnaryPtr() {
  if (m_removeOnDelete)
    delete (**this);
}

template<typename T>
sd::core::AttributeUnaryPtr<T>*
sd::core::AttributeUnaryPtr<T>::clone() const {
  return new AttributeUnaryPtr<T>(*this);
}

template<typename T>
void
sd::core::AttributeUnaryPtr<T>::clear() {
  if (m_removeOnDelete)
    delete (**this);
   **this = NULL;
}

template<typename T>
void
sd::core::AttributeUnaryPtr<T>::setRemoveOnDelete(bool r) {
  m_removeOnDelete = r;
}

#endif /*! SD_CORE_MESH_ATTRIBUTE_HXX */
