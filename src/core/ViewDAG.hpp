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

#ifndef SD_CORE_DAG_VIEW_HPP
#define SD_CORE_DAG_VIEW_HPP

#include "ViewId.hpp"

#include <map>
#include <vector>

namespace sd {

  namespace core {
    
    class ViewNode;
    
    class ViewDAG {

    public:
      
      virtual ~ViewDAG();
      
      ViewDAG();
      
      inline static ViewDAG& instance() {return *m_instance;}
      
      inline static void setInstance(ViewDAG* instance) {
	delete m_instance;
	m_instance = instance;
      }
      
      // Accessors
      
      std::map<ViewId, ViewNode*>& views() {return m_views;}
      
      std::vector<ViewNode*>& ghostNodes() {return m_ghostNodes;}
      
      std::vector<ViewNode*> roots();
      
    private:

      friend class ViewNode;

      // Creators
      
      ViewId getNewRootId();

      ViewId getIncrementedId(const ViewId& parent);

      ViewId getNewBranchId(const ViewId& parent);

      ViewId getNewBranchId(const std::vector<const ViewId*>& parents);
      
      void add(ViewNode* node);
      
      // void remove(ViewNode* node); // execute delete
      
      void release(ViewNode* node); // do not delete
      
    private:
      
      std::map<ViewId, ViewNode*> m_views;
      std::vector<ViewNode*> m_ghostNodes;
      unsigned int m_lastCard;
      
    private:
      
      static ViewDAG* m_instance;
      
    };
    
  }
  
}

#endif /* ! SD_CORE_DAG_VIEW_HPP */
