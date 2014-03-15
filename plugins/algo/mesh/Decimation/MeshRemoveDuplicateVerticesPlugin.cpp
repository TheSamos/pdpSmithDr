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

#include <frontend/lib/MeshViewAlgorithm.hpp>

#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>

// #include <meshprocessing/tree/Octree.hpp>

#include <vector>

#include<cstdlib>

#include <iostream>

template<typename T, unsigned int N>
class MeshRemoveDuplicateVerticesPlugin : public sd::frontend::MeshViewAlgorithm<T, N> {

public:

  MeshRemoveDuplicateVerticesPlugin() : sd::frontend::MeshViewAlgorithm<T, N>(m_name, m_input, m_parameters)
  {}

  virtual ~MeshRemoveDuplicateVerticesPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual MeshRemoveDuplicateVerticesPlugin* clone() {
    return new MeshRemoveDuplicateVerticesPlugin;
  }
  
  virtual bool run() {
#ifdef DEBUG
    std::cout << "Running " << m_name << ": \n";
#endif
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 1) {
#ifdef DEBUG
      std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
#endif
      return false;
    }

    sd::core::MeshView_<T, N>* meshp = static_cast<sd::core::MeshView_<T, N>*>(parents[0]);
#ifdef DEBUG
    std::cout << "\tWorking with mesh: " << meshp->name() << std::endl;
#endif

    if (meshp->getVertexDataType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
      std::cerr << meshp->getVertexDataType().toString() << " != " << sd::core::DataType_<T>().toString() << std::endl;
      std::cout << "Bad mesh type! Abort!\n";
#endif
      return false;
    }
    
    sd::frontend::MeshViewAlgorithm<T, N>::build(); // needed before all operation...
    
    sd::frontend::Parameter p;
    this->getParams("Epsilon", p);
    const double epsilon = p.getFloat();
    /*
    sd::meshprocessing::tree::Octree<T, N> tree((sd::core::MeshView_<T, N>*)this, 1);
    
    typename sd::core::MeshView_<T, N>::vertices_modifier_iterator it = (*this).beginVerticesModifier();
    const typename sd::core::MeshView_<T, N>::vertices_modifier_iterator end = (*this).endVerticesModifier();
    for (; it != end; ++it) {
      std::vector<typename sd::core::VertexFromMeshView<T> > set;
      tree.computeNeigborsInBall(*it, epsilon, set, false);
      typename std::vector<typename sd::core::VertexFromMeshView<T> >::iterator itSet = set.begin();
      const typename std::vector<typename sd::core::VertexFromMeshView<T> >::iterator endSet = set.end();
      for (; itSet != endSet; ++itSet) {
	if (*itSet != *it)
	  (*this).removeVertex(*itSet);
      }
    }
    */
    return true;
  }
  
private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
  
};

template<typename T, unsigned int N>
const std::string MeshRemoveDuplicateVerticesPlugin<T, N>::m_name = "Mesh:RemoveDuplicateVertices";
template<typename T, unsigned int N>
const sd::frontend::AlgorithmInfo MeshRemoveDuplicateVerticesPlugin<T, N>::m_input
= sd::frontend::make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N));
template<typename T, unsigned int N>
const sd::frontend::ParameterList MeshRemoveDuplicateVerticesPlugin<T, N>::m_parameters = sd::frontend::make_parameter_list();

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::ParameterList params = sd::frontend::make_parameter_list("Epsilon", .000001);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<sd::UINT8, 3>, params);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<sd::INT8, 3>, params);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<sd::UINT16, 3>, params);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<sd::INT16, 3>, params);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<float, 3>, params);
  sd::frontend::registerAlgorithm(new MeshRemoveDuplicateVerticesPlugin<double, 3>, params);
}
