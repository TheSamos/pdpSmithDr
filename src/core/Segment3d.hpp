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

#ifndef SD_CORE_SEGMENT3D_HPP
#define SD_CORE_SEGMENT3D_HPP

#include "Math.hpp"

#include "Line.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Segment3d {
      
    public:
      
      inline Segment3d(const sd::core::Point3d<T>& point1,
		       const sd::core::Point3d<T>& point2)
	: m_point1(point1), m_point2(point2) {}
      
      inline const sd::core::Point3d<T>& point1() const {return m_point1;}
      
      inline const sd::core::Point3d<T>& point2() const {return m_point2;}
      
      inline sd::core::Point3d<T> midpoint() const
      {return sd::core::Point3d<T>((point1.x() + point2.x()) / 2.,
				   (point1.y() + point2.y()) / 2.,
				   (point1.z() + point2.z()) / 2.);}
      
      inline bool line(const Segment3d<T>& segment) const
      {return sd::core::Line3d<T>(m_point1, m_point2);}
      
      inline bool isSame(const Segment3d<T>& segment) const;
      
      inline bool contains(const Point3d<T>& point) const;
      
      inline sd::core::Point3d<T> intersection(const Segment3d<T>& segment) const;
      
      // insegment Point3d<T> orthogonalProjection(const Point3d<T>& point) const;
      
      inline T norm() const;
      
      inline sd::core::Segment3d<T>& operator+=(const sd::core::Vector3d<T>& vector);
      
    public:
      
      static inline sd::core::Point3d<T> midpoint(const sd::core::Point3d<T>& point1,
						  const sd::core::Point3d<T>& point2)
      {return sd::core::Point3d<T>((point1.x() + point2.x()) / 2.,
				   (point1.y() + point2.y()) / 2.,
				   (point1.z() + point2.z()) / 2.);}
      
    private:
      
      sd::core::Point3d<T> m_point1;
      sd::core::Point3d<T> m_point2;
      
    };
    
    template<typename T>
    inline bool operator==(const Segment3d<T>& segment1,
			   const Segment3d<T>& segment2) {
      return segment1.isSame(segment2);
    }
    
    template<typename T>
    inline bool operator!=(const Segment3d<T>& segment1,
			   const Segment3d<T>& segment2) {
      return !segment1.isSame(segment2);
    }
      
  }
  
}

#include "Segment3d.hxx"

#endif /*! SD_CORE_SEGMENT3D_HPP */
