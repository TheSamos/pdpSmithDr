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

#ifndef SD_CORE_LINE3D_HPP
#define SD_CORE_LINE3D_HPP

#include "Math.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Line3d {
      
    public:
      
      inline Line3d(const sd::core::Point3d<T>& point, const sd::core::Vector3d<T>& direction)
	: m_point(point),
	  m_direction(direction) {
	m_direction.normalize();
      }
      
      inline Line3d(const sd::core::Point3d<T>& point1, const sd::core::Point3d<T>& point2)
	: m_point(point1),
	  m_direction(point1, point2) {
	m_direction.normalize();
      }
      
      inline const Point3d<T>& point() const {return m_point;}
      
      inline const Vector3d<T>& direction() const {return m_direction;}
      
      inline bool isCoplanar(const Line3d<T>& line) const;
      
      inline bool isSame(const Line3d<T>& line) const;
      
      inline bool contains(const Point3d<T>& point) const;
      
      inline Point3d<T> intersection(const Line3d<T>& line) const;
      
      inline Point3d<T> orthogonalProjection(const Point3d<T>& point) const;
      
      inline T euclideanDistance(const Point3d<T>& point) const;
      
      inline Line3d<T>& operator+=(const sd::core::Vector3d<T>& vector);
      
    private:
      
      sd::core::Point3d<T> m_point;
      sd::core::Vector3d<T> m_direction;
      
    };

    
    template<typename T>
    inline bool operator==(const Line3d<T>& line1, const Line3d<T>& line2) {
      return line1.isSame(line2);
    }
    
    template<typename T>
    inline bool operator!=(const Line3d<T>& line1, const Line3d<T>& line2) {
      return !line1.isSame(line2);
    }
      
  }
  
}

#include "Line3d.hxx"

#endif /*! SD_CORE_LINE3D_HPP */
