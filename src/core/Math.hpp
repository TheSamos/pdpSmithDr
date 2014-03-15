/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_CORE_MATH_HPP
#define SD_CORE_MATH_HPP

#include "DataType.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Point3d;
    
    template<typename T>
    class Vector3d;
    
    template<typename T>
    class Line3d;
    
    template<typename T>
    inline static T epsilon();
    
    template<typename T>
    inline static bool about_equals(const T& a, const T& b, T epsilon = epsilon<T>());
    
    template<typename T>
    inline static bool about_equals(const sd::core::Point3d<T>& a,
				    const sd::core::Point3d<T>& b, T epsilon = epsilon<T>());
    
    template<typename T>
    inline static bool about_equals(const sd::core::Vector3d<T>& a,
				    const sd::core::Vector3d<T>& b, T epsilon = epsilon<T>());
    
    template<typename T>
    inline static T abs(T val);
    
    template<typename T>
    inline static T acos(T val);
    
    template<typename T>
    inline static T asin(T val);

    template<typename T>
    typename sd::core::DataType_<T>::work_type euclideanDistance(const Point3d<T>& a,
								 const Point3d<T>& b);
    
    template<typename T>
    typename sd::core::Point3d<T> midpoint(const Point3d<T>& a,
					   const Point3d<T>& b);
    
    template<typename T>
    typename sd::core::DataType_<T>::work_type angle(const Point3d<T>& a,
						     const Point3d<T>& b,
						     const Point3d<T>& c);

    template<typename T>
    typename sd::core::DataType_<T>::work_type angle(const Vector3d<T>& v1,
						     const Vector3d<T>& v2,
						     const Vector3d<T>& normal);
    
    template<typename T>
    typename sd::core::DataType_<T>::work_type area(const Point3d<T>& a,
						    const Point3d<T>& b,
						    const Point3d<T>& c);
    
    template<typename T>
    typename sd::core::DataType_<T>::work_type voronoiArea(const sd::core::Point3d<T>& target,
							   const sd::core::Point3d<T>& prev,
							   const sd::core::Point3d<T>& current,
							   const sd::core::Point3d<T>& next);
    
    template<typename T>
    typename sd::core::DataType_<T>::work_type voronoiArea(const Point3d<T>& a,
							   const Point3d<T>& b,
							   const Point3d<T>& c);

    template<typename T>
    sd::core::Point3d<T> circumcenter(const sd::core::Point3d<T>& a,
				      const sd::core::Point3d<T>& b,
				      const sd::core::Point3d<T>& c);
    
    template<typename T>
    sd::core::Vector3d<T> computeRotation(const T angle,
					  const typename sd::core::Vector3d<T>& v1,
					  const typename sd::core::Vector3d<T>& axe);
    
  }
  
}

#include "Math.hxx"

#endif /*! SD_CORE_MATH_HPP */
