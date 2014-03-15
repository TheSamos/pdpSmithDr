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

#ifndef SD_CORE_BOUNDING_BOX_HPP
#define SD_CORE_BOUNDING_BOX_HPP

#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace sd {
  
  namespace core {

    template<typename T>
    class BoundingBox {

    public:

      BoundingBox();
      
      BoundingBox(const T& x, const T& y, const T& z,
                  const T& w, const T& h, const T& d);

      BoundingBox(const Point3d<T>& origin,
                  const T& w, const T& h, const T& d);
      
      BoundingBox(const Point3d<T>& corner0,
                  const Point3d<T>& corner1);

      BoundingBox(const BoundingBox<T>& bb);
      
      inline const Point3d<T>& corner0() const;
      inline const Point3d<T>& corner1() const;

      inline const Point3d<T>& setCorner0(const Point3d<T>& p);
      inline const Point3d<T>& setCorner1(const Point3d<T>& p);

      inline const Point3d<T>& origin() const;

      inline T x() const;
      inline T y() const;
      inline T z() const;

      inline T width() const;
      inline T height() const;
      inline T depth() const;

      inline const Point3d<T>& setOrigin(const Point3d<T>& p);

      inline void setWidth(T w);
      inline void setHeight(T h);
      inline void setDepth(T d);

      inline void add(const Point3d<T>& p);

      inline bool contains(const Point3d<T>& p) const;
      inline bool intersects(const BoundingBox<T>& bb) const;
      inline bool includes(const BoundingBox<T>& bb) const;
      
      inline bool isValid() const;

    private:

      Point3d<T> m_corner0;
      Point3d<T> m_corner1;
      bool m_isValid;

    };

  }
  
}

#include "BoundingBox.hxx"

#endif /*! SD_CORE_BOUNDING_BOX_HPP */
