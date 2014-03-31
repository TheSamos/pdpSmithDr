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

#include <vector>
#include <ostream>

#include "Types.hpp"

#ifndef SD_CORE_VIEW_ID_HPP
#define SD_CORE_VIEW_ID_HPP

namespace sd {
  
  namespace core {

    // NOTE : id=1 < id=2 : so '<' == 'is parent'

    class ViewDAG;
    
    class ViewId {

    public:

      friend class ViewDAG;

      ViewId();
      
      bool isNull() const;

      bool isBranchRoot() const;

      std::string toString() const;
      
      // return 1 == greater, -1 == lesser, 0 == equals, -2 and 2 non comparable
      static char compare(const ViewId& vi1, const ViewId& Vi2);

    private:

      ViewId(unsigned int size, bool isBranchRoot);

      ViewId(const ViewId& vi, bool isBranchRoot);

      friend std::ostream& operator<<(std::ostream& os, const ViewId& v);

    private:
      
      typedef sd::UINT16 view_id_data_type;
      std::vector<view_id_data_type> m_id;

      bool m_isBranchRoot;

    };

    bool comparable(const ViewId& vi1, const ViewId& vi2);
    
    std::ostream& operator<<(std::ostream& os, const ViewId& v);
    
    bool operator==(const ViewId& vi1, const ViewId& vi2);
    bool operator!=(const ViewId& vi1, const ViewId& vi2);

    bool operator<(const ViewId& vi1, const ViewId& vi2);
    bool operator>(const ViewId& vi1, const ViewId& vi2);

    bool operator<=(const ViewId& vi1, const ViewId& vi2);
    bool operator>=(const ViewId& vi1, const ViewId& vi2);

  }
  
}

#endif /* ! SD_CORE_VIEW_ID_HPP */
