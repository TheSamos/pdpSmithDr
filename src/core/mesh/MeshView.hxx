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

#ifndef SD_CORE_MESH_VIEW_HXX
#define SD_CORE_MESH_VIEW_HXX

#include <cassert>

inline sd::core::MeshView::MeshView(sd::core::ViewNode* previousViewNode, const std::string& name)
  : ViewNode(MeshObjectType, previousViewNode, name) {}

inline sd::core::MeshView::MeshView(std::vector<sd::core::ViewNode*>& previousViewNodes, const std::string& name)
  : ViewNode(MeshObjectType, previousViewNodes, name) {}

inline sd::core::MeshView::~MeshView() {}

#endif /* ! SD_CORE_MESH_VIEW_HXX */
