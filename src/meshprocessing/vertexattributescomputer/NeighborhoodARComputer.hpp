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

#ifndef SD_MESH_PROCESSING_NEIGHBORHOOD_AR_COMPUTER_HPP
#define SD_MESH_PROCESSING_NEIGHBORHOOD_AR_COMPUTER_HPP

#include <meshprocessing/vertexattributescomputer/VertexAttributesComputer.hpp>

namespace sd {

  namespace meshprocessing {
    
    namespace vertexattributescomputer {
      
      template<typename T, unsigned int N>
      class NeighborhoodARComputer : public VertexAttributesComputer<T, N> {
	
      public:
      
	NeighborhoodARComputer()
	  : VertexAttributesComputer<T, N>(),
	    m_minKey(sd::core::AttributeKey::getKeyAndCreateIfNotExists("MinAR")),
	    m_maxKey(sd::core::AttributeKey::getKeyAndCreateIfNotExists("MaxAR")),
	    m_meanKey(sd::core::AttributeKey::getKeyAndCreateIfNotExists("MeanAR")),
	    m_standardDeviationKey(sd::core::AttributeKey::getKeyAndCreateIfNotExists("StandardDeviationAR")) {}
	
	virtual std::string name() const { return "NeighborhoodARComputer";}
	
	virtual NeighborhoodARComputer<T, N>* clone() const { return new NeighborhoodARComputer<T, N>(*this);}
	
	virtual std::vector<sd::core::AttributeKey> computedAttributes() const {
	  return {m_minKey, m_maxKey, m_meanKey, m_standardDeviationKey};
	}

	virtual void compute(const sd::core::VertexFromMeshView<T>& vertexTarget,
			     const std::vector<std::vector<sd::core::VertexFromMeshView<T> > >& neighbors,
			     std::map<sd::core::AttributeKey, typename sd::core::DataType_<T>::work_type>& result) const {
	  T min=0., max=0., mean=0., standardDeviation=0.;
	  std::vector<T> values(0);
	  typename sd::core::DataType_<T>::work_type som = 0.;
	  typename sd::core::DataType_<T>::work_type somSquare = 0.;
	  typename std::vector<std::vector<sd::core::VertexFromMeshView<T> > >::const_iterator it = neighbors.begin();
	  const typename std::vector<std::vector<sd::core::VertexFromMeshView<T> > >::const_iterator end = neighbors.end();
	  for (; it!=end; ++it) {
	    const typename std::vector<sd::core::VertexFromMeshView<T> >& ring = (*it);
	    values.resize(values.size()+ring.size());
	    for (unsigned int i=0; i<ring.size(); ++i) {
	      const typename sd::core::Point3d<T>& a = ring[i];
	      const typename sd::core::Point3d<T>& b = ring[(i+1)%ring.size()];
	      const typename sd::core::Point3d<T>& c = vertexTarget;
	      sd::core::Vector3d<T> ca(c, a);
	      sd::core::Vector3d<T> ab(a, b);
	      sd::core::Vector3d<T> bc(b, c);
	      typename sd::core::DataType_<T>::work_type base = std::max(ab.norm(), std::max(bc.norm(), ca.norm()));
	      typename sd::core::DataType_<T>::work_type s = (ab.norm() + bc.norm() + ca.norm()) / 2.;
	      typename sd::core::DataType_<T>::work_type area = sqrt(s * (s-ab.norm()) * (s-bc.norm()) * (s-ca.norm()));
	      typename sd::core::DataType_<T>::work_type h = area*2./base;
	      T quality = h/base;
	      min = (i == 0) ? quality : std::min(min, quality);
	      max = (i == 0) ? quality : std::max(max, quality);
	      som += quality;
	      values[i] = quality;
	    }
	  }
	  mean = som/(typename sd::core::DataType_<T>::work_type)values.size();
	  for (unsigned int i=0; i<values.size(); ++i)
	    somSquare += (values[i]-mean)*(values[i]-mean);
	  standardDeviation = sqrt(somSquare/(typename sd::core::DataType_<T>::work_type)values.size());
	  
	  result[m_minKey] = min;
	  result[m_maxKey] = max;
	  result[m_meanKey] = mean;
	  result[m_standardDeviationKey] = standardDeviation;
	}
	
      private:
	
	sd::core::AttributeKey m_minKey, m_maxKey, m_meanKey, m_standardDeviationKey;
	
      };
      
    }
    
  }
  
}

// #include "NeighborhoodARComputer.hxx"

#endif /* ! SD_MESH_PROCESSING_NEIGHBORHOOD_AR_COMPUTER_HPP */
