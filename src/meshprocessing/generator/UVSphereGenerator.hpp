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

#ifndef SD_MESH_PROCESSING_GENERATORS_UV_SPHERE_GENERATOR_HPP
#define SD_MESH_PROCESSING_GENERATORS_UV_SPHERE_GENERATOR_HPP

#include <meshprocessing/generator/MeshGenerator.hpp>

namespace sd {

  namespace meshprocessing {
    
    namespace generator {

      template<typename T, unsigned int N>
      class UVSphereGenerator : MeshGenerator<T, N> {

      public:
	
	UVSphereGenerator(T radius,
			  unsigned int segment,
			  unsigned int ring) : m_radius(radius),
					       m_segment(segment),
					       m_ring(ring) {
	  assert(m_segment>=3 && m_ring>=3);
	}
	
	virtual ~UVSphereGenerator() {}
	
	virtual std::string name() const {return "UV-Sphere";}
	
	virtual sd::core::MeshView_<T, N>* getMesh() {
	  
	  sd::core::MeshView_<T, N>* mesh = new sd::core::MeshView_<T, N>();
	  
	  for(unsigned int j=1; j<m_ring; ++j) {
	    T angle=j*M_PI/((T)m_ring*1.);
	    T radius=sin(angle)*m_radius;
	    T height=-cos(angle)*m_radius;
	    
	    for(unsigned int i=0; i<m_segment; ++i)
	      mesh->addVertex(radius*cos(i*(2*M_PI/(m_segment*1.))), height, radius*sin(i*(2*M_PI/(m_segment*1.))));
	  }
	  
	  for(unsigned int j=0; j<m_ring-2; ++j)
	    for(unsigned int i=0; i<m_segment; ++i) {
	      mesh->addFace(mesh->getVertexAt(j*m_segment+i),
			    mesh->getVertexAt((j+1)*m_segment+i),
			    mesh->getVertexAt(j*m_segment+((i+1)%m_segment)));
	      mesh->addFace(mesh->getVertexAt(j*m_segment+((i+1)%m_segment)),
			    mesh->getVertexAt((j+1)*m_segment+i),
			    mesh->getVertexAt((j+1)*m_segment+((i+1)%m_segment)));
	    }
	  
	  mesh->addVertex(0, -m_radius, 0);
	  mesh->addVertex(0, m_radius, 0);
	  
	  for(unsigned int i=0; i<m_segment; ++i) {
	    mesh->addFace(mesh->getVertexAt(i),
			  mesh->getVertexAt((i+1)%m_segment),
			  mesh->getVertexAt(m_segment*(m_ring-1)));
	    mesh->addFace(mesh->getVertexAt((m_ring-2)*m_segment+i),
			  mesh->getVertexAt(m_segment*(m_ring-1)+1),
			  mesh->getVertexAt((m_ring-2)*m_segment+(i+1)%m_segment));
	  }
	
	  typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator itV =  mesh->beginVerticesConstGeometry();
	  const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator endV =  mesh->endVerticesConstGeometry();
	  for (; itV!=endV; ++itV) {
	    sd::core::Vector3d<T> v(sd::core::Point3d<T>(0., 0., 0.), *itV);
	    v.normalize();
	    (*itV).setNormal(v);
	  }
	
	  return mesh;
  
	}
	
      private:
	
	T m_radius;
	unsigned int m_segment;
	unsigned int m_ring;
	
      };
      
    }
    
  }

}

#endif /*! SD_MESH_PROCESSING_GENERATORS_MESH_GENERATOR_HPP */
