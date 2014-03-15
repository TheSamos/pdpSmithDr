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

#ifndef SD_CORE_SHARED_OBJECT_HPP
#define SD_CORE_SHARED_OBJECT_HPP


namespace sd {
  
  namespace core {

    template<typename A>
    class SharedObject {
      
    public:
      
      virtual ~SharedObject();
      
      SharedObject();
  
      SharedObject(const A& a);
  
      SharedObject(A* data);
  
      SharedObject(const sd::core::SharedObject<A>& so);
      
      sd::core::SharedObject<A>& operator=(const A& a);
      
      sd::core::SharedObject<A>& operator=(A* data);
      
      sd::core::SharedObject<A>& operator=(const sd::core::SharedObject<A>& so);
      
      inline A& operator->() {return *m_data;}
      inline const A& operator->() const {return *m_data;}
      
      inline A& operator*() {return *m_data;}
      inline const A& operator*() const {return *m_data;}
      
      inline bool isTheLast() const {return (*m_counter == 1);}
      
      inline A* data() {return m_data;}
      inline const A* data() const {return m_data;}
      
    private:
      
      void dropData();
      
    private:
  
      A* m_data;
      unsigned int* m_counter;
  
    };

  }
  
}

template<typename A>
inline bool operator==(sd::core::SharedObject<A> so1, sd::core::SharedObject<A> so2);

template<typename A>
inline bool operator!=(sd::core::SharedObject<A> so1, sd::core::SharedObject<A> so2);

#include "SharedObject.hxx"

#endif /* ! SD_CORE_SHARED_OBJECT_HPP */
