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

#ifndef SD_CORE_COORDINATES_HXX
#define SD_CORE_COORDINATES_HXX

#include <cassert>
#include <cstdlib>//NULL
#include <cstring>

template<typename T>
sd::core::Coordinates<T>::~Coordinates() {
  if (m_isStandAlone) {
    delete m_data;
  }
}

template<typename T>
sd::core::Coordinates<T>::Coordinates(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove)
  : m_data(data),
    m_index(index),
    m_isStandAlone(deleteOnRemove) {}

template<typename T>
template<typename ...VALUES>
sd::core::Coordinates<T>::Coordinates(unsigned int size, VALUES ...values)
  : m_data(new sd::core::Vector<T>(size)),
    m_index(0),
    m_isStandAlone(true) {
  fill(size, 0u, std::forward<VALUES>(values)...);
}

template<typename T>
template<typename ...VALUES>
sd::core::Coordinates<T>::Coordinates(unsigned int* size, VALUES ...values)
  : m_data(NULL),
    m_index(0),
    m_isStandAlone(true) {
  fill(0, 0u, std::forward<VALUES>(values)...);
  (*size) = m_data->size();
}

template<typename T>
bool
sd::core::Coordinates<T>::resize(unsigned int newSize, T defaultValue) {
  if (!m_isStandAlone)
    return false;
  m_data->resize(newSize, defaultValue);
  return true;
}

template<typename T>
void
sd::core::Coordinates<T>::reset(unsigned int n, T defaultValue) {
  std::vector<T> tmp(n, defaultValue);
  std::copy(tmp.data(), tmp.data()+n, m_data->data()+m_index);
}

template<typename T>
void
sd::core::Coordinates<T>::copyTo(unsigned int n, T* dest) const {
  std::copy(m_data->data()+m_index, m_data->data()+m_index+n, dest);
}

template<typename T>
void
sd::core::Coordinates<T>::copy(unsigned int n, const Coordinates<T>& c) {
  assert(c.m_data != 0);
  if (this == &c)
    return;
  if (m_isStandAlone) {
    delete m_data;
    m_data = NULL;
  }
  if (!m_data) {
    m_data = new sd::core::Vector<T>(n);
    m_isStandAlone = true;
    m_index = 0;
  }
  std::copy(c.m_data->data()+c.m_index, c.m_data->data()+c.m_index+n, m_data->data());
  assert(m_data != 0);
}

template<typename T>
void
sd::core::Coordinates<T>::nocopy(const Coordinates<T>& c) {
  assert(c.m_data != 0);
  if (this == &c)
    return;
  if (m_isStandAlone) {
    delete m_data;
    m_data = NULL;
  }
  if (c.m_isStandAlone) {
    m_data = new sd::core::Vector<T>(*(c.m_data));
  } else {
    m_data = c.m_data;
  }
  m_index = c.m_index;
  m_isStandAlone = c.m_isStandAlone;
  if (m_isStandAlone)
    assert(m_index == 0);
  assert(m_data != 0);
}

template<typename T>
const T&
sd::core::Coordinates<T>::operator() (unsigned int i) const {
  assert(m_index+i < m_data->size());
  return data()[i];
}

template<typename T>
const T&
sd::core::Coordinates<T>::operator[] (unsigned int i) const {
  assert(m_index+i < m_data->size());
  return data()[i];
}
/*
template<typename T>
T&
sd::core::Coordinates<T>::operator[] (unsigned int i) {
  assert(m_index+i < m_data->size());
  return data()[i];
}
*/
template<typename T>
const T&
sd::core::Coordinates<T>::getAt(unsigned int i) const {
  assert(m_index+i < m_data->size());
  return data()[i];
}

template<typename T>
const T&
sd::core::Coordinates<T>::setAt(unsigned int i, const T& value) {
  assert(m_index+i < m_data->size());
  return data()[i] = value;
}

template<typename T>
void
sd::core::Coordinates<T>::unlink() {
  if (m_isStandAlone)
    return;
  sd::core::Vector<T>* oldData = m_data;
  const unsigned int n = nbDims();
  m_data = new sd::core::Vector<T>(n);
  std::copy(oldData->data()+m_index, oldData->data()+m_index+n, m_data->data());
  m_index = 0;
  m_isStandAlone = true;
}

template<typename T>
void
sd::core::Coordinates<T>::fill(unsigned int /*sizeMax*/, unsigned int /*idx*/) {
  if (!m_data) {
    m_data = new sd::core::Vector<T>();
  }
}

template<typename T>
void
sd::core::Coordinates<T>::fill(unsigned int sizeMax, unsigned int idx, T value) {
  if (sizeMax != 0 && idx >= sizeMax) {
    if (!m_data) {
      m_data = new sd::core::Vector<T>(sizeMax);
    }
  } else {
    if (!m_data) {
      m_data = new sd::core::Vector<T>(idx+1);
    }
    (*m_data)[idx] = value;
  }
}

template<typename T>
template<typename ...VALUES>
void
sd::core::Coordinates<T>::fill(unsigned int sizeMax, unsigned int idx, T value, VALUES ...rest) {
  if (sizeMax != 0 && idx >= sizeMax) {
    if (!m_data) {
      m_data = new sd::core::Vector<T>(sizeMax);
    }
  } else {
    fill(sizeMax, idx+1, std::forward<VALUES>(rest)...);
    (*m_data)[idx] = value;
  }
}
      
#endif /*! SD_CORE_COORDINATES_HXX */
