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

#ifndef SD_IMAGE_PROCESSING_STRUCTURAL_ELEMENT_HPP
#define SD_IMAGE_PROCESSING_STRUCTURAL_ELEMENT_HPP

#include <vector>
#include <cstdio>

namespace sd {
  namespace imageprocessing {

    class StructuralElement
    {

    public:

      StructuralElement(const double* kernel, size_t width, size_t height, size_t depth);
      StructuralElement(const StructuralElement& f);
      virtual ~StructuralElement();

      StructuralElement& operator=(const StructuralElement& f);

      inline const std::vector<double>& getKernel() const { return m_kernel; }
      inline size_t getWidth() const { return m_width; }
      inline size_t getHeight() const { return m_height; }
      inline size_t getDepth() const { return m_depth; }

      static StructuralElement* buildSquare(size_t side);
      static StructuralElement* buildCircle(size_t radius);
      static StructuralElement* buildCross(size_t side, bool diagonal);
      static StructuralElement* buildRectangularCuboid(size_t side1, size_t side2, size_t side3);
      static StructuralElement* buildCylinder(size_t radius, size_t height);
      static StructuralElement* buildSphere(size_t radius);
      static StructuralElement* buildEllipsoid(size_t radius1, size_t radius2, size_t radius3);

    private:

      std::vector<double> m_kernel;
      size_t m_width;
      size_t m_height;
      size_t m_depth;

    };

  }
}

#endif /* #! SD_IMAGE_PROCESSING_STRUCTURAL_ELEMENT_HPP */
