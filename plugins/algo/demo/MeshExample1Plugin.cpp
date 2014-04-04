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
#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <PluginDefs.hpp>

#include <vector>

#include <iostream>

/*
 * It is simpler to implement this algo as a MeshViewAlgorithm.
 */

template<typename T, unsigned int N>
class MeshExample1Plugin : public sd::frontend::MeshViewAlgorithm<T, N> {

public:

  MeshExample1Plugin() : sd::frontend::MeshViewAlgorithm<T, N>(m_name, m_input/*, m_parameters*/)
  {}

  virtual ~MeshExample1Plugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual MeshExample1Plugin* clone() {
    return new MeshExample1Plugin;
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
    
    if (!sd::core::AttributeKey::containsKey("-3"))
      sd::core::AttributeKey::addNewKey("-3");
    const sd::core::AttributeKey& akey_3 = sd::core::AttributeKey::getKey("-3");
    
    if (!sd::core::AttributeKey::containsKey("-2"))
      sd::core::AttributeKey::addNewKey("-2");
    const sd::core::AttributeKey& akey_2 = sd::core::AttributeKey::getKey("-2");
    
    if (!sd::core::AttributeKey::containsKey("-1"))
      sd::core::AttributeKey::addNewKey("-1");
    const sd::core::AttributeKey& akey_1 = sd::core::AttributeKey::getKey("-1");
    
    if (!sd::core::AttributeKey::containsKey("0"))
      sd::core::AttributeKey::addNewKey("0");
    const sd::core::AttributeKey& akey0 = sd::core::AttributeKey::getKey("0");
    
    if (!sd::core::AttributeKey::containsKey("1"))
      sd::core::AttributeKey::addNewKey("1");
    const sd::core::AttributeKey& akey1 = sd::core::AttributeKey::getKey("1");
    
    if (!sd::core::AttributeKey::containsKey("2"))
      sd::core::AttributeKey::addNewKey("2");
    const sd::core::AttributeKey& akey2 = sd::core::AttributeKey::getKey("2");
    
    if (!sd::core::AttributeKey::containsKey("3"))
      sd::core::AttributeKey::addNewKey("3");
    const sd::core::AttributeKey& akey3 = sd::core::AttributeKey::getKey("3");
    
    if (!sd::core::AttributeKey::containsKey("nan"))
      sd::core::AttributeKey::addNewKey("nan");
    const sd::core::AttributeKey& akeynan = sd::core::AttributeKey::getKey("nan");
    
    typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = (*this).beginVerticesConstGeometry();
    const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = (*this).endVerticesConstGeometry();
    for (; it!=end; ++it) {
      (*it).template setAttribute<T>(akey_3, (T)-3.);
      (*it).template setAttribute<T>(akey_2, (T)-2.);
      (*it).template setAttribute<T>(akey_1, (T)-1.);
      (*it).template setAttribute<T>(akey0, (T)0.);
      (*it).template setAttribute<T>(akey1, (T)1.);
      (*it).template setAttribute<T>(akey2, (T)2.);
      (*it).template setAttribute<T>(akey3, (T)3.);
      (*it).template setAttribute<T>(akeynan, (T)std::numeric_limits<T>::quiet_NaN());
    }
    return true;
  }

private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  //static const sd::frontend::ParameterList m_parameters;
  
};

template<typename T, unsigned int N>
const std::string MeshExample1Plugin<T, N>::m_name = "Demonstration:MeshExample1Plugin";
template<typename T, unsigned int N>
const sd::frontend::AlgorithmInfo MeshExample1Plugin<T, N>::m_input
= sd::frontend::make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N));
/*template<typename T, unsigned int N>
const sd::frontend::ParameterList MeshExample1Plugin<T, N>::m_parameters = sd::frontend::make_parameter_list();*/

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<double, 3>);
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<float, 3>);
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<sd::UINT8, 3>);
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<sd::INT8, 3>);
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<sd::UINT16, 3>);
  sd::frontend::registerAlgorithm(new MeshExample1Plugin<sd::INT16, 3>);
}
