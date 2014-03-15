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

#ifndef SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HXX
#define SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HXX

template<typename T>
sd::core::EdgeFromMeshViewIterator<T>::EdgeFromMeshViewIterator(MeshView* askingView, 
								   std::map<MeshView*,
									    std::vector<std::vector<EdgeFromMeshView<T>* > > >* edges)
  : VertexBasedFromMeshViewIterator<T, 2, EdgeFromMeshView<T> >(askingView, edges)
{
}

template<typename T>
bool
sd::core::operator==(const sd::core::EdgeFromMeshViewIterator<T>& ei1,
		     const sd::core::EdgeFromMeshViewIterator<T>& ei2)
{
  return (static_cast<VertexBasedFromMeshViewIterator<T, 2, EdgeFromMeshView<T> > >(ei1)
	  == static_cast<VertexBasedFromMeshViewIterator<T, 2, EdgeFromMeshView<T> > >(ei2));
}
    
template<typename T>
bool
sd::core::operator!=(const sd::core::EdgeFromMeshViewIterator<T>& ei1,
		     const sd::core::EdgeFromMeshViewIterator<T>& ei2)
{
  return !(ei1 == ei2);
}

#endif /*! SD_CORE_EDGE_FROM_MESH_VIEW_ITERATOR_HXX */
