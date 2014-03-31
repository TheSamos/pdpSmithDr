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

#ifndef SD_CORE_VECTOR3D_HPP
#define SD_CORE_VECTOR3D_HPP

#include "ThreeDimensionalCoordinates.hpp"

#include "Math.hpp"

namespace sd {
  
  namespace core {
    
    template<typename T>
    class Vector3d : public ThreeDimensionalCoordinates<T> {

    public:

      virtual ~Vector3d() {}
      
      Vector3d(sd::core::Vector<T>* data, unsigned int index, bool deleteOnRemove = false);

      Vector3d(const T& x=0, const T& y=0, const T& z=0);
      
      template<typename U = T>
      Vector3d(const Point3d<U>& p1, const Point3d<U>& p2);

      Vector3d(const Vector3d<T>& v);
      
      typename sd::core::DataType_<T>::work_type angle(const sd::core::Vector3d<T>& v) const;
      
      bool isCollinear(const sd::core::Vector3d<T>& v) const;
      
      typename sd::core::DataType_<T>::work_type norm() const;
      void normalize();
      Vector3d<T> normalized() const;

      void cross(const Vector3d<T>& v, Vector3d<T>& result) const;
      typename sd::core::DataType_<T>::work_type dot(const Vector3d<T>& v) const;

      const Vector3d<T>& operator+=(T val);
      const Vector3d<T>& operator-=(T val);
      const Vector3d<T>& operator*=(T val);
      const Vector3d<T>& operator/=(T val);

      const Vector3d<T>& operator+=(const Vector3d<T>& v);
      const Vector3d<T>& operator-=(const Vector3d<T>& v);
      const Vector3d<T>& operator^=(const Vector3d<T>& v);

    };

    template<typename T>
    Vector3d<T> operator+(const Vector3d<T>& v, T val);
    
    template<typename T>
    Vector3d<T> operator-(const Vector3d<T>& v, T val);
    
    template<typename T>
    Vector3d<T> operator*(const Vector3d<T>& v, T val);
    
    template<typename T>
    inline Vector3d<T> operator*(T val, const Vector3d<T>& v) {return v * val;}
    
    template<typename T>
    Vector3d<T> operator/(const Vector3d<T>& v, T val);

    template<typename T>
    Vector3d<T> operator+(const Vector3d<T>& v1, const Vector3d<T>& v2);

    template<typename T>
    Vector3d<T> operator-(const Vector3d<T>& v1, const Vector3d<T>& v2);

    template<typename T>
    typename sd::core::DataType_<T>::work_type operator*(const Vector3d<T>& v1, const Vector3d<T>& v2); // dot

    template<typename T>
    Vector3d<T> operator^(const Vector3d<T>& v1, const Vector3d<T>& v2); // cross
    
    template<typename T>
    typename sd::core::DataType_<T>::work_type angle(const sd::core::Vector3d<T>& v1,
						     const sd::core::Vector3d<T>& v2);
    
    template<typename T>
    bool areCollinear(const sd::core::Vector3d<T>& v1,
		      const sd::core::Vector3d<T>& v2);
    
  }
  
}

#include "Vector3d.hxx"

#endif /*! SD_CORE_VECTOR3D_HPP */
