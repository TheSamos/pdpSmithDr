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

#ifndef SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HPP
#define SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HPP

#include <core/Clonable.hpp>

#include <core/mesh/MeshView_.hpp>

#include <meshprocessing/generator/neighborsdealer/NeighborsDealerFromMesh.hpp>

namespace sd {

  namespace meshprocessing {
    
    namespace vertexattributescomputer {
      
      template<typename T, unsigned int N>
      class VertexAttributesComputer : public core::Clonable {
	
      public:
      
	VertexAttributesComputer();
	
	virtual std::string name() const = 0;
      
	virtual VertexAttributesComputer* clone() const = 0;
      
	virtual void compute(sd::core::MeshView_<T, N>* mesh,
			     sd::meshprocessing::generator::NeighborsDealerFromMesh<T, N>* dealer,
			     T firstScale = 1., T step = 1., unsigned int nbSteps = 1);
	
	inline typename sd::core::DataType_<T>::work_type minValue(const sd::core::AttributeKey& att) const
	{return m_data.find(att)->second.first;}
	
	inline typename sd::core::DataType_<T>::work_type maxValue(const sd::core::AttributeKey& att) const
	{return m_data.find(att)->second.second;}
	
	/*
	inline std::vector<sd::core::AttributeKey> computedAttributes() const {
	  std::vector<sd::core::AttributeKey> atts;
	  typename std::map<sd::core::AttributeKey, std::pair<typename sd::core::DataType_<T>::work_type,
							      typename sd::core::DataType_<T>::work_type> >::const_iterator it = m_data.begin();
          const typename std::map<sd::core::AttributeKey, std::pair<typename sd::core::DataType_<T>::work_type,
								    typename sd::core::DataType_<T>::work_type> >::const_iterator end = m_data.end();
	  for(; it!=end; ++it)
	    atts.push_back((*it).first);
	  
	  return atts;
	}
	*/
	virtual std::vector<sd::core::AttributeKey> computedAttributes() const = 0;
	
	virtual void compute(const sd::core::VertexFromMeshView<T>& vertexTarget,
			     const std::vector<std::vector<sd::core::VertexFromMeshView<T> > >& neighbors,
			     std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>& result) const = 0;
	
	
      protected:
	
	std::map<sd::core::AttributeKey, std::pair<typename sd::core::DataType_<T>::work_type,
						   typename sd::core::DataType_<T>::work_type> > m_data;
	
      };
      
    }
    
  }
  
}

#include "VertexAttributesComputer.hxx"

#endif /* ! SD_MESH_PROCESSING_MESH_VERTEX_ATTRIBUTES_COMPUTER_HPP */
