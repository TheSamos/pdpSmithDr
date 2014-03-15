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

#ifndef SD_CORE_VIEW_NODE_HPP
#define SD_CORE_VIEW_NODE_HPP

#include "Object.hpp"
#include "Types.hpp"
#include "ViewId.hpp"

#include "Attribute.hpp"
#include "AttributeKey.hpp"

#include <string>
#include <vector>
#include <typeinfo>

namespace sd {
  
  namespace core {

    class ViewNode : public Object {

    public:
      
      typedef sd::core::AttributeKey::attribute_key_id attribute_key_id;
      
    public:

      virtual ~ViewNode();

      const std::vector<ViewNode*>& getPreviousViewNodes() const;
      const std::vector<ViewNode*>& getNextViewNodes() const;
      void getParentsList(std::vector<ViewNode*>& parents);

      void addParent(ViewNode* parent);

      void disconnectChild(ViewNode* child);
      void disconnectChildren();

      ViewId& getId();
      const ViewId& getId() const;
      
      void setGhostView(bool value = true);
      bool isGhostView() const;
      
      ViewDAG* dag() {return m_dag;}
      const ViewDAG* dag() const {return m_dag;}
      void setDAG(ViewDAG* dag);
      
      bool isAttributeSet(const sd::core::AttributeKey& akey) const;
      std::vector<sd::core::AttributeKey> getGlobalAttributesKeys() const;
      const Attribute* getAttribute(const AttributeKey& akey) const;
      void setAttribute(const AttributeKey& akey, Attribute* attribute);
      
    protected:
      
      ViewNode(ObjectType sdType,
	       ViewNode* previousViewNode,
	       const std::string& name);
      
      ViewNode(ObjectType sdType,
	       std::vector<ViewNode*>& previousViewNodes,
	       const std::string& name);
      
      void removeChild(ViewNode* child);
      void removeParent(ViewNode* parent);
      void disconnectParent(ViewNode* parent);

      void addChild(ViewNode* child);

      bool isBranchRoot() const;

      void computeMyId();

      friend bool comparable(const ViewNode& bn1, const ViewNode& bn2);

      friend bool operator==(const ViewNode& bn1, const ViewNode& bn2);
      friend bool operator!=(const ViewNode& bn1, const ViewNode& bn2);

      friend bool operator<(const ViewNode& bn1, const ViewNode& bn2);
      friend bool operator>(const ViewNode& bn1, const ViewNode& bn2);

      friend bool operator<=(const ViewNode& bn1, const ViewNode& bn2);
      friend bool operator>=(const ViewNode& bn1, const ViewNode& bn2);

    private:
      
      ViewNode(const ViewNode&) = delete;

      inline const ViewNode* askableParent() const;
      inline ViewNode* askableParent();

    protected:

      ViewId m_id;
      std::vector<ViewNode*> m_previousViewNodes;
      std::vector<ViewNode*> m_nextViewNodes;
      
    private:
      
      bool m_isGhostView;
      ViewDAG* m_dag;
      std::map<attribute_key_id, Attribute*> m_attributes;
      
    };

  }
  
}

#endif /*! SD_CORE_VIEW_NODE_HPP */
