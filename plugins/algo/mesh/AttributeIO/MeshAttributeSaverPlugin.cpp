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

#include <fstream>
#include <iostream>

template<typename T, unsigned int N, typename U>
class MeshAttributeSaverPlugin : public sd::frontend::BlackBoxAlgorithm {

public:

  MeshAttributeSaverPlugin() : sd::frontend::BlackBoxAlgorithm(m_name,
							       sd::frontend::AlgorithmInfo(),
							       m_input,
							       m_parameters)
  {}

  virtual ~MeshAttributeSaverPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual MeshAttributeSaverPlugin* clone() {
    return new MeshAttributeSaverPlugin;
  }
  
  virtual bool run() {
#ifdef DEBUG
    std::cerr << "Running " << m_name << ": \n";
#endif
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 1) {
#ifdef DEBUG
      std::cerr << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
#endif
      return false;
    }

    sd::core::MeshView_<T, N>* meshp = static_cast<sd::core::MeshView_<T, N>*>(parents[0]);
#ifdef DEBUG
    std::cerr << "\tWorking with mesh: " << meshp->name() << std::endl;
#endif

    if (meshp->getVertexDataType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
      std::cerr << meshp->getVertexDataType().toString() << " != " << sd::core::DataType_<T>().toString() << std::endl;
      std::cout << "Bad mesh type! Abort!\n";
#endif
      return false;
    }
    
    sd::frontend::Parameter p;
    this->getParams("akey:0-0", p);
    std::string keyName = p.getString();
    if (!sd::core::AttributeKey::containsKey(keyName)
	|| !(meshp->verticesAttributeContainsKey(sd::core::AttributeKey::getKey(keyName))))
      return false;
    
    this->getParams("fileURI", p);
    const std::string fileURI = p.getString();
    
    sd::core::AttributeKey key = sd::core::AttributeKey::getKey(keyName);
    std::ofstream outfile(fileURI.c_str());
    if (!outfile) {
#ifdef DEBUG
      std::cerr << "MeshAttributeSaverPlugin::run() : unable to open file: " << fileURI << std::endl;
#endif
      return false;
    }
    
    outfile << keyName << std::endl;
    
    typename sd::core::MeshView_<T, N>::vertices_const_iterator it = meshp->beginVertices();
    const typename sd::core::MeshView_<T, N>::vertices_const_iterator end = meshp->endVertices();
    for (; it != end; ++it) {
      if ((*it).isAttributeSet(key)) {
	outfile << this->template implode<typename sd::core::DataType_<U>::sub_type>((*it).template getAttribute<U>(key)) << std::endl;
      }
    }
    
    outfile << std::endl;
    
    outfile.close();
    
    return true;
  }
  
private:
  
  template<typename V>
  std::string implode(const V& value) const {
    return std::string("") + value;
  }
  
  template<typename V>
  std::string implode(const sd::core::DynamicDimensionalCoordinates<V>& values) const {
    std::string result = "";
    for (unsigned int i=0; i<values.size(); ++i) {
      result = result + values[i];
      if (i != values.size()-1)
	result += ";";
    }
    return result;
  }
  
private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
  
};

template<typename T, unsigned int N, typename U>
const std::string MeshAttributeSaverPlugin<T, N, U>::m_name = "Mesh:AttibuteIO:Saver";

template<typename T, unsigned int N, typename U>
const sd::frontend::AlgorithmInfo MeshAttributeSaverPlugin<T, N, U>::m_input
= sd::frontend::make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N,
								sd::core::AttributeDescription("Attribute",
											       sd::core::AttributeDescription::VERTICES,
											       false,
											       sd::core::DataType_<U>())));

template<typename T, unsigned int N, typename U>
const sd::frontend::ParameterList MeshAttributeSaverPlugin<T, N, U>::m_parameters;

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::ParameterList params = sd::frontend::make_parameter_list("fileURI", "");
  
#define DECARE2(T, U)							\
  sd::frontend::registerAlgorithm(new MeshAttributeSaverPlugin<T, 3, U>, params); \
  sd::frontend::registerAlgorithm(new MeshAttributeSaverPlugin<T, 3, sd::core::DynamicDimensionalCoordinates<U> >, params); \
  sd::frontend::registerAlgorithm(new MeshAttributeSaverPlugin<T, 4, U>, params); \
  sd::frontend::registerAlgorithm(new MeshAttributeSaverPlugin<T, 4, sd::core::DynamicDimensionalCoordinates<U> >, params); 
  
#define DECLARE1(T)				\
  DECARE2(T, float)				\
    DECARE2(T, double)				\
    DECARE2(T, long double)			\
    DECARE2(T, char)				\
    DECARE2(T, unsigned char)			\
    DECARE2(T, int)				\
    DECARE2(T, unsigned int)
  
  DECLARE1(double)
    DECLARE1(long double)
    DECLARE1(float)
    
#undef DECARE1
#undef DECARE2
    
}
