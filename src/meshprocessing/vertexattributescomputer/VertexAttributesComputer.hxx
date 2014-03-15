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

#ifndef SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HXX
#define SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HXX
      
template<typename T, unsigned int N>
sd::meshprocessing::vertexattributescomputer::VertexAttributesComputer<T, N>::VertexAttributesComputer() : m_data() {}

template<typename T, unsigned int N>
void
sd::meshprocessing::vertexattributescomputer::VertexAttributesComputer<T, N>::compute(sd::core::MeshView_<T, N>* mesh,
										      sd::meshprocessing::generator::NeighborsDealerFromMesh<T, N>* dealer,
										      T firstScale, T step, unsigned int nbSteps) {
  
  sd::meshprocessing::generator::NeighborsDealerFromMesh<T, N>* dealerUsed = dealer->clone();
  
  typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = mesh->beginVerticesConstGeometry();
  const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator begin = it;
  const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = mesh->endVerticesConstGeometry();
  for (; it != end; ++it) {
    sd::core::VertexFromMeshView<T> target = *it;
    dealerUsed->set(mesh, target, firstScale, step, nbSteps);
    std::map<sd::core::AttributeKey, core::DynamicDimensionalCoordinates<T> > ringData;
    
    typename generator::NeighborsDealer<T>::NeighborsDealerConstIterator itD = dealerUsed->begin();
    const typename generator::NeighborsDealer<T>::NeighborsDealerConstIterator endD = dealerUsed->end();
    for (unsigned int i=0; itD != endD; ++itD) {
      std::vector<std::vector<sd::core::VertexFromMeshView<T> > > neighbors(1);
      neighbors[0] = *itD;
      std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type> result;
      if (dealerUsed->getValidations()[i]) {
	this->compute(target, neighbors, result);
      } else {
	const std::vector<sd::core::AttributeKey> ca = computedAttributes();
	std::vector<sd::core::AttributeKey>::const_iterator it = ca.begin();
	const std::vector<sd::core::AttributeKey>::const_iterator end = ca.end();
	for (; it!=end; ++it)
	  result[*it] = std::numeric_limits<T>::quiet_NaN();
      }
      
      if (nbSteps == 1) {
	typename std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>::iterator itR = result.begin();
	const typename std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>::iterator endR = result.end();
	for (; itR!=endR; ++itR) {
	  target.template setAttribute<T>((*itR).first, (*itR).second);
	  if (!std::isnan((*itR).second)) {
	    if (it == begin || m_data[(*itR).first].first > (*itR).second)
	      m_data[(*itR).first].first = (*itR).second;
	    if (it == begin || m_data[(*itR).first].second < (*itR).second)
	      m_data[(*itR).first].second = (*itR).second;
	  }
	}
      } else {
	typename std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>::iterator itR = result.begin();
	const typename std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>::iterator endR = result.end();
	for (; itR!=endR; ++itR)
	  ringData[(*itR).first].push_back((*itR).second);
      }
    }
    
    if (nbSteps > 1) {
      typename std::map<sd::core::AttributeKey, core::DynamicDimensionalCoordinates<T> >::iterator itR = ringData.begin();
      const typename std::map<sd::core::AttributeKey, core::DynamicDimensionalCoordinates<T> >::iterator endR = ringData.end();
      for (; itR!=endR; ++itR)
	target.template setAttribute<core::DynamicDimensionalCoordinates<T> >((*itR).first, (*itR).second);
    }
	 
  }

  delete dealerUsed;
}
      
#endif /* ! SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HXX */
