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

#include <vector>

#include <utils/sdRawUtils.hpp>
#include <utils/sdTypeUtils.hpp>

#include <fstream>
#include <iostream>

template<typename T, unsigned int N, typename U>
class MeshAttributeLoaderPlugin : public sd::frontend::MeshViewAlgorithm<T, N> {

public:

  MeshAttributeLoaderPlugin() : sd::frontend::MeshViewAlgorithm<T, N>(m_name, m_input, m_parameters)
  {}

  virtual ~MeshAttributeLoaderPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual MeshAttributeLoaderPlugin* clone() {
    return new MeshAttributeLoaderPlugin;
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
    
    sd::frontend::MeshViewAlgorithm<T, N>::build(); // needed before all operation...
    
    sd::frontend::Parameter p;
    this->getParams("fileURI", p);
    const std::string fileURI = p.getString();
    
    return loadSingle(fileURI) || loadMultiple(fileURI);
  }
  
private:
  
  bool loadSingle(const std::string& fileURI) {
    std::ifstream infile(fileURI.c_str(), std::ifstream::in);
    char buff[256];
    
    if (!infile.good()) {
      std::cerr << "Wrong file !" << std::endl;
      return false;
    }
    if (infile.eof()) {
      std::cerr << "Empty file !" << std::endl;
      return false;
    }
    
    infile.getline(buff, 256);
    if (std::string(buff).empty()) {
       std::cerr << "No name !" << std::endl;
      return false;
    }
    if (!sd::core::AttributeKey::containsKey(buff))
      sd::core::AttributeKey::addNewKey(buff);
    assert(sd::core::AttributeKey::containsKey(buff));
    sd::core::AttributeKey key = sd::core::AttributeKey::getKey(buff);
    
    typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = this->beginVerticesConstGeometry();
    const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = this->endVerticesConstGeometry();
    while (!infile.eof() && it != end) {
      infile.getline(buff, 256);
      sd::core::Vector<std::string> result;
      sd::raw::split(buff, ";", result);
      if (result.size() > 2) {
	this->removeVerticesAttributeKey(key);
	return false;
      }
      if (result.size() == 1) {
	(*it).template setAttribute<U>(key, sd::raw::readAcsii<U>(result[0]));
	++it;
      }
    }
    
    if (it != end)
      std::cerr << "Incomplete file !" << std::endl;
    
    infile.close();
    return true;
  }
  
  bool loadMultiple(const std::string& fileURI) {
    std::ifstream infile(fileURI.c_str(), std::ifstream::in);
    char buff[256];
    
    if (!infile.good()) {
      std::cerr << "Wrong file !" << std::endl;
      return false;
    }
    if (infile.eof()) {
      std::cerr << "Empty file !" << std::endl;
      return false;
    }
    
    infile.getline(buff, 256);
    if (!sd::core::AttributeKey::containsKey(buff))
      sd::core::AttributeKey::addNewKey(buff);
    assert(sd::core::AttributeKey::containsKey(buff));
    sd::core::AttributeKey key = sd::core::AttributeKey::getKey(buff);
    
    typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = this->beginVerticesConstGeometry();
    const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = this->endVerticesConstGeometry();
    while (!infile.eof() && it != end) {
      infile.getline(buff, 256);
      sd::core::Vector<std::string> result;
      sd::raw::split(buff, ";", result);
      sd::core::DynamicDimensionalCoordinates<U> tmp;
      for (unsigned int i=0; i<result.size(); ++i) {
	tmp.push_back(sd::raw::readAcsii<U>(result[i]));
      }
      (*it).template setAttribute<sd::core::DynamicDimensionalCoordinates<U> >(key, tmp);
      ++it;
    }
    
    if (it != end)
      std::cerr << "Incomplete file !" << std::endl;
    
    infile.close();
    return true;
  }

private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
  
};

template<typename T, unsigned int N, typename U>
const std::string MeshAttributeLoaderPlugin<T, N, U>::m_name = "Mesh:AttibuteIO:Loader";

template<typename T, unsigned int N, typename U>
const sd::frontend::AlgorithmInfo MeshAttributeLoaderPlugin<T, N, U>::m_input
= sd::frontend::make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N));

template<typename T, unsigned int N, typename U>
const sd::frontend::ParameterList MeshAttributeLoaderPlugin<T, N, U>::m_parameters;

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::ParameterList params = sd::frontend::make_parameter_list("fileURI", "");
  
#define DECARE2(T, U)							\
  sd::frontend::registerAlgorithm(new MeshAttributeLoaderPlugin<T, 3, U>, params); \
  sd::frontend::registerAlgorithm(new MeshAttributeLoaderPlugin<T, 4, U>, params);
  
#define DECLARE1(T)				\
  DECARE2(T, double)
  
  /*									\
									DECARE2(T, double) \
									DECARE2(T, long double)	\
									DECARE2(T, char) \
									DECARE2(T, unsigned char) \
									DECARE2(T, int)	\
									DECARE2(T, unsigned int)
  */
  
  DECLARE1(double)
    DECLARE1(long double)
    DECLARE1(float)
    
#undef DECARE1
#undef DECARE2
    
}
