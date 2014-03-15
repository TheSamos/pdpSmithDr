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

#ifndef SD_CORE_MUTEX_LOCKER_HPP
#define SD_CORE_MUTEX_LOCKER_HPP

#include <mutex>

namespace sd {
  
  namespace core {
    
    typedef std::mutex Mutex;
    
    template<typename T>
    class MutexLocker {
      
    public:
      
      MutexLocker(Mutex& mutex)
	: m_mutex(mutex) {
	m_mutex.lock();
      }
      
      ~MutexLocker() {
	m_mutex.unlock();
      }
      
    private:
      
      MutexLocker(const MutexLocker&) = delete;
      
    private:
      
      Mutex& m_mutex;
      
    };
    
  }
  
}

#endif /*! SD_CORE_MUTEX_LOCKER_HPP */
