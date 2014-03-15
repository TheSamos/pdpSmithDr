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

#ifndef SD_SHARED_PTR_HPP
#define SD_SHARED_PTR_HPP

#if defined(HAVE_CPP0X_SHARED_PTR)
#include <memory>
namespace sd {
  using std::shared_ptr;
  using std::static_pointer_cast;
  using std::const_pointer_cast;
  using std::dynamic_pointer_cast;
}
#elif defined(HAVE_TR1_SHARED_PTR)
#include <tr1/memory>
namespace sd {
  using std::tr1::shared_ptr;
  using std::tr1::static_pointer_cast;
  using std::tr1::const_pointer_cast;
  using std::tr1::dynamic_pointer_cast;
}
#elif defined(HAVE_BOOST_TR1_SHARED_PTR)
#include <boost/tr1/memory.hpp>
namespace sd {
  using std::tr1::shared_ptr;
  using std::tr1::static_pointer_cast;
  using std::tr1::const_pointer_cast;
  using std::tr1::dynamic_pointer_cast;
}
#elif defined(HAVE_BOOST_SHARED_PTR)
#include <boost/shared_ptr.hpp>
namespace sd {
  using boost::shared_ptr;
  using boost::static_pointer_cast;
  using boost::const_pointer_cast;
  using boost::dynamic_pointer_cast;
}
#else
#error No shared_ptr implementation is available!
#endif

#ifndef HAVE_BOOST_SHARED_PTR
#include "SmithDRDefs.hpp"
NO_QT_WARNINGS()
#include <boost/intrusive_ptr.hpp>
RESTORE_CONTEXT()
namespace sd {
  using boost::intrusive_ptr;
}
#endif

#endif /*! SD_SHARED_PTR_HPP */
