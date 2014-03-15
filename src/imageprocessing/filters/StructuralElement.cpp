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

#include "StructuralElement.hpp"
#include <algorithm>

using namespace sd::imageprocessing;

StructuralElement::StructuralElement(const double* kernel, size_t width, size_t height, size_t depth)
  : m_kernel(), m_width(width), m_height(height), m_depth(depth)
{
  // :TODO: check if dimensions > 0
  if (kernel)
    m_kernel.assign(kernel, kernel+m_width * m_height * m_depth);
  else
    m_kernel.resize(m_width * m_height * m_depth);
}

StructuralElement::StructuralElement(const StructuralElement& f)
  : m_kernel(f.m_kernel), m_width(f.m_width), m_height(f.m_height), m_depth(f.m_depth)
{
}

StructuralElement::~StructuralElement()
{
}

StructuralElement&
StructuralElement::operator=(const StructuralElement& f)
{
  if (this != &f) {
    m_width = f.m_width;
    m_height = f.m_height;
    m_depth = f.m_depth;
    m_kernel = f.m_kernel;
  }

  return *this;
}

StructuralElement*
StructuralElement::buildSquare(size_t side)
{
  return buildRectangularCuboid(side, side, 1);
}

StructuralElement*
StructuralElement::buildRectangularCuboid(size_t side1, size_t side2, size_t side3)
{
  StructuralElement* s = new StructuralElement(NULL, side1, side2, side3);
  std::fill(s->m_kernel.begin(), s->m_kernel.end(), 1.);
  return s;
}

StructuralElement*
StructuralElement::buildCircle(size_t radius)
{
  return buildCylinder(radius, 1);
}

StructuralElement*
StructuralElement::buildCross(size_t side, bool diagonal)
{
  StructuralElement* s = new StructuralElement(NULL, side, side, 1);
  std::fill(s->m_kernel.begin(), s->m_kernel.end(), 0.);

  if (diagonal) {
    for (size_t i = 0; i < side; ++i) {
      s->m_kernel[i+i*side] = 1;
      s->m_kernel[i+(side-1-i)*side] = 1;
    }
  }
  else {
    for (size_t i = 0; i < side; ++i) {
      s->m_kernel[side/2+i*side] = 1;
      s->m_kernel[i+(side/2)*side] = 1;
    }
  }

  return s;
}

StructuralElement*
StructuralElement::buildCylinder(size_t radius, size_t height)
{
  size_t diameter = 2*radius + 1;
  StructuralElement* s = new StructuralElement(NULL, diameter, diameter, height);
  size_t r2 = radius * radius;

  for (size_t j = 0, idx = 0; j < diameter; ++j)
    for (size_t i = 0; i < diameter; ++i, ++idx) {
      size_t d2 = (i-radius)*(i-radius) + (j-radius)*(j-radius);
      s->m_kernel[idx] = (d2 <= r2 ? 1 : 0);
    }

  return s;
}

StructuralElement*
StructuralElement::buildSphere(size_t radius)
{
  size_t diameter = 2*radius + 1;
  StructuralElement* s = new StructuralElement(NULL, diameter, diameter, diameter);
  size_t r2 = radius * radius;

  for (size_t k = 0, idx = 0; k < diameter; ++k)
    for (size_t j = 0; j < diameter; ++j)
      for (size_t i = 0; i < diameter; ++i, ++idx) {
	size_t d2 = (i-radius)*(i-radius) + (j-radius)*(j-radius) + (k-radius)*(k-radius);
	s->m_kernel[idx] = (d2 <= r2 ? 1 : 0);
      }

  return s;
}

StructuralElement*
StructuralElement::buildEllipsoid(size_t radius1, size_t radius2, size_t radius3)
{
  size_t diameter1 = 2*radius1 + 1;
  size_t diameter2 = 2*radius2 + 1;
  size_t diameter3 = 2*radius3 + 1;
  StructuralElement* s = new StructuralElement(NULL, diameter1, diameter2, diameter3);
  double r1 = double(radius1 * radius1);
  double r2 = double(radius2 * radius2);
  double r3 = double(radius3 * radius3);

  for (size_t k = 0, idx = 0; k < diameter3; ++k)
    for (size_t j = 0; j < diameter2; ++j)
      for (size_t i = 0; i < diameter1; ++i, ++idx) {
	double d2 = double((i-radius1)*(i-radius1))/r1
	  + double((j-radius2)*(j-radius2))/r2
	  + double((k-radius3)*(k-radius3))/r3;
	s->m_kernel[idx] = (d2 <= 1 ? 1 : 0);
      }

  return s;
}
