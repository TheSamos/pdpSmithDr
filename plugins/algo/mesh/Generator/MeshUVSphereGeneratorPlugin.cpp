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

#include <frontend/lib/BlackBoxAlgorithm.hpp>

#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>

#include <vector>

#include <iostream>

#include <meshprocessing/generator/UVSphereGenerator.hpp>

template<typename T, unsigned int N>
class MeshUVSphereGeneratorPlugin : public sd::frontend::BlackBoxAlgorithm {

public:

  MeshUVSphereGeneratorPlugin() : sd::frontend::BlackBoxAlgorithm(m_name, m_output, m_input,
								 m_parameters)
  {}

  virtual ~MeshUVSphereGeneratorPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }
  
  virtual const std::string& name() const {
    return m_name;
  }

  virtual MeshUVSphereGeneratorPlugin* clone() {
    return new MeshUVSphereGeneratorPlugin;
  }
  
  virtual bool run() {
    #ifdef DEBUG
    std::cout << "Running " << m_name << ": \n";
#endif
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 0) {
#ifdef DEBUG
      std::cout << "\tExpecting 0 parent exactly, found " << parents.size() << ". Abort!\n";
#endif
      return false;
    }
    
    sd::frontend::Parameter p;
    this->getParams("Radius", p);
    const float radius = p.getFloat();
    
    this->getParams("Ring", p);
    const unsigned int ring = p.getInteger();
    
    this->getParams("Segment", p);
    const unsigned int segment = p.getInteger();
    
    if (segment<3 || ring<3)
      return false;
    
    sd::core::MeshView_<T, N>* mesh = sd::meshprocessing::generator::UVSphereGenerator<T, N>(radius, segment, ring).getMesh();
    mesh->setName(std::string("UVSphereR") + radius + "_" + segment + "x" + ring);
    mesh->addParent(this);
    
    return true;
  }
  
private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::AlgorithmInfo m_output;
  static const sd::frontend::ParameterList m_parameters;
  
};

template<typename T, unsigned int N>
const std::string MeshUVSphereGeneratorPlugin<T, N>::m_name = "Mesh:UVSphereGenerator";

template<typename T, unsigned int N>
const sd::frontend::AlgorithmInfo MeshUVSphereGeneratorPlugin<T, N>::m_input = sd::frontend::make_info();

template<typename T, unsigned int N>
const sd::frontend::AlgorithmInfo MeshUVSphereGeneratorPlugin<T, N>::m_output = sd::frontend::make_info((sd::core::MeshView*) 0);

template<typename T, unsigned int N>
const sd::frontend::ParameterList MeshUVSphereGeneratorPlugin<T, N>::m_parameters;

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::ParameterList params = sd::frontend::make_parameter_list("Radius", 2., "Ring", 50, "Segment", 50);
  
  sd::frontend::registerAlgorithm(new MeshUVSphereGeneratorPlugin<double, 3>, params);
  
}
