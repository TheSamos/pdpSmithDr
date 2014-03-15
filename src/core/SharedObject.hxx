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

#ifndef SD_CORE_SHARED_OBJECT_HXX
#define SD_CORE_SHARED_OBJECT_HXX

#include <iostream>

template<typename A>
sd::core::SharedObject<A>::~SharedObject() {
  dropData();
}

template<typename A>
sd::core::SharedObject<A>::SharedObject()
  : m_data(/*(A*)NULL*/new A()),
    m_counter(new unsigned int(1)) {}

template<typename A>
sd::core::SharedObject<A>::SharedObject(const A& a)
  : m_data(new A(a)),
    m_counter(new unsigned int(1)) {}

template<typename A>
sd::core::SharedObject<A>::SharedObject(A* data)
  : m_data(data),
    m_counter(new unsigned int(1)) {}

template<typename A>
sd::core::SharedObject<A>::SharedObject(const sd::core::SharedObject<A>& so)
  : m_data(so.m_data),
    m_counter(so.m_counter) {
  ++(*m_counter);
}

template<typename A>
sd::core::SharedObject<A>&
sd::core::SharedObject<A>::operator=(const A& a) {
  dropData();
  m_data = new A(a);
  m_counter = new unsigned int(1);
  return *this;
}

template<typename A>
sd::core::SharedObject<A>&
sd::core::SharedObject<A>::operator=(A* data) {
  dropData();
  m_data = data;
  m_counter = new unsigned int(1);
  return *this;
}

template<typename A>
sd::core::SharedObject<A>&
sd::core::SharedObject<A>::operator=(const sd::core::SharedObject<A>& so) {
  if (&so == this)
    return *this;
  dropData();
  m_data = so.m_data;
  m_counter = so.m_counter;
  ++(*m_counter);
  return *this;
}

template<typename A>
void
sd::core::SharedObject<A>::dropData() {
  if (isTheLast()) {
    delete m_data;
    delete m_counter;
  } else {
    --(*m_counter);
  }
}

template<typename A>
bool operator==(sd::core::SharedObject<A> so1, sd::core::SharedObject<A> so2) {
  return *so1 == *so2;
}

template<typename A>
bool operator!=(sd::core::SharedObject<A> so1, sd::core::SharedObject<A> so2) {
  return *so1 != *so2;
}

#endif /* ! SD_CORE_SHARED_OBJECT_HXX */
