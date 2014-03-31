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

#ifndef SD_MESH_PROCESSING_GENERATORS_UV_CYLINDER_GENERATOR_HPP
#define SD_MESH_PROCESSING_GENERATORS_UV_CYLINDER_GENERATOR_HPP

#include <meshprocessing/generator/MeshGenerator.hpp>

namespace sd {

  namespace meshprocessing {
    
    namespace generator {

      template<typename T, unsigned int N>
      class UVCylinderGenerator : MeshGenerator<T, N> {

      public:
	
	UVCylinderGenerator(T radius,
			    T height,
			    unsigned int segment,
			    unsigned int ring) : m_radius(radius),
						 m_height(height),
						 m_segment(segment),
						 m_ring(ring) {
	  assert(m_segment>=3 && m_ring>=2);
	}
	
	virtual ~UVCylinderGenerator() {}
	
	virtual std::string name() const {return "UV-Cylinder";}
	
	virtual sd::core::MeshView_<T, N>* getMesh() {
	  
	  sd::core::MeshView_<T, N>* mesh = new sd::core::MeshView_<T, N>();
	  
	  for(unsigned int j=0; j<m_ring; ++j) {
	    T height=((T)j/(T)(m_ring-1))*m_height-m_height/2.;	    
	    for(unsigned int i=0; i<m_segment; ++i) {
	      sd::core::VertexFromMeshView<T> vertex = mesh->addVertex(m_radius*cos(i*(2*M_PI/(m_segment*1.))), height, m_radius*sin(i*(2*M_PI/(m_segment*1.))));
	      sd::core::Vector3d<T> normal(sd::core::Point3d<T>(0., vertex.y(), 0.), vertex);
	      normal.normalize();
	      if (j==0) {
		normal += sd::core::Vector3d<T>(0., -1., 0.);
		normal.normalize();
	      } else if(j==m_ring-1) {
		normal += sd::core::Vector3d<T>(0., 1., 0.);
		normal.normalize();
	      }
	      vertex.setNormal(normal);
	    }
	  }
	  
	  for(unsigned int j=0; j<m_ring-1; ++j)
	    for(unsigned int i=0; i<m_segment; ++i) {
	      mesh->addFace(mesh->getVertexAt(j*m_segment+i),
			    mesh->getVertexAt((j+1)*m_segment+i),
			    mesh->getVertexAt(j*m_segment+((i+1)%m_segment)));
	      mesh->addFace(mesh->getVertexAt(j*m_segment+((i+1)%m_segment)),
			    mesh->getVertexAt((j+1)*m_segment+i),
			    mesh->getVertexAt((j+1)*m_segment+((i+1)%m_segment)));
	    }
	  
	  mesh->addVertex(0, -m_height/2., 0).setNormal(sd::core::Vector3d<T>(0., -1., 0.));
	  mesh->addVertex(0, m_height/2., 0).setNormal(sd::core::Vector3d<T>(0., 1., 0.));
	  
	  for(unsigned int i=0; i<m_segment; ++i) {
	    mesh->addFace(mesh->getVertexAt(i),
			  mesh->getVertexAt((i+1)%m_segment),
			  mesh->getVertexAt(m_segment*(m_ring)));
	    mesh->addFace(mesh->getVertexAt((m_ring-1)*m_segment+i),
			  mesh->getVertexAt(m_segment*m_ring+1),
			  mesh->getVertexAt((m_ring-1)*m_segment+(i+1)%m_segment));
	  }
	  
	  return mesh;
  
	}
	
      private:
	
	T m_radius;
	T m_height;
	unsigned int m_segment;
	unsigned int m_ring;
	
      };
      
    }
    
  }

}

#endif /*! SD_MESH_PROCESSING_GENERATORS_MESH_GENERATOR_HPP */
