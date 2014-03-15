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

#ifndef SD_CORE_ATOMIC_VALUE_HPP
#define SD_CORE_ATOMIC_VALUE_HPP

#include <core/MutexLocker.hpp>

namespace sd {
  
  namespace core {
    
    template<typename T>
    class AtomicValue {
      
    public:
      
      AtomicValue() : m_mutex(),
		      m_value()
      {}
      
      AtomicValue(T value) : m_mutex(),
			     m_value(value)
      {}
      
      AtomicValue& operator=(const AtomicValue& atomicValue) {
	MutexLocker<T> locker(m_mutex);
	m_value = atomicValue.m_value;
	return *this;
      }
      
      AtomicValue& operator=(T value) {
	MutexLocker<T> locker(m_mutex);
	m_value = value;
	return *this;
      }
      
      operator T () {
	MutexLocker<T> locker(m_mutex);
	return m_value;
      }
      
      void lock() {m_mutex.lock();}
      
      void unlock() {m_mutex.unlock();}
      
      T& ref() {return m_value;}
      
    private:
      
      AtomicValue(const AtomicValue& atomicValue) = delete;
      
    private:
      
      Mutex m_mutex;
      T m_value;
      
    };
    
  }  
  
}

#endif /*! SD_CORE_ATOMIC_VALUE_HPP */
