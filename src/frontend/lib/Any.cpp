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

#include "Any.hpp"
#include <algorithm>

using namespace sd::frontend;

Any::Any()
  : m_content(NULL)
{}

Any::Any(const Any& x)
  : m_content(x.m_content ? x.m_content->clone() : 0)
{}

Any::~Any() {
  if (m_content)
    delete m_content;
}

Any&
Any::swap(Any& rhs) {
  std::swap(m_content, rhs.m_content);
  return *this;
}

Any&
Any::operator=(const Any& rhs) {
  Any x(rhs);
  return swap(x);
}
/*
const std::type_info&
Any::typeInfo() const {
  return (m_content) ? m_content->typeInfo() : typeid(void);
}
*/
Any::operator const void *() const {
  return m_content;
}

std::ostream&
Any::operator<<(std::ostream& o) const {
  m_content ? (o << (*m_content).toString()) : (o << 0);
  return o;
}
