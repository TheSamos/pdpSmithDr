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

#include <core/mesh/MeshView_.hpp>

#include <frontend/lib/Loader.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <PluginDefs.hpp>

#include "LoaderPly.hpp"

template<typename T, unsigned int N>
class LoaderPlyPlugin : public sd::frontend::Loader {
public:
  
  LoaderPlyPlugin()
    : sd::frontend::Loader(m_name, m_output, m_parameters)
  {}

  virtual ~LoaderPlyPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }
  
  virtual const std::string&
  name() const {
    return m_name;
  }
  
  virtual LoaderPlyPlugin*
  clone() {
    return new LoaderPlyPlugin;
  }

  virtual const std::string&
  formatName() const {
    static const std::string fName = "PLY";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const {
    static const std::string ext = "*.ply;*.PLY";
    return ext;
  }

  virtual Loader::Type
  loaderType() const {
    return Loader::MeshLoader;
  }

  virtual bool
  load() {
    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;
    
    sd::ply::LoaderPly<T, N> loader;
    sd::core::MeshView_<T, N> *mesh = loader.loadPlyMesh(filename);
    if (!mesh)
      return false;

    mesh->addParent(this);

    return true;
  }  

private:
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_output;
  static const sd::frontend::ParameterList m_parameters;
  
};


template<typename T, unsigned int N>
const std::string LoaderPlyPlugin<T, N>::m_name = "LoaderPlyPlugin";
template<typename T, unsigned int N>
const sd::frontend::AlgorithmInfo LoaderPlyPlugin<T, N>::m_output = sd::frontend::make_info((sd::core::MeshView*) 0);
template<typename T, unsigned int N>
const sd::frontend::ParameterList LoaderPlyPlugin<T, N>::m_parameters = sd::frontend::make_parameter_list("filename", "");


// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::registerLoader(new LoaderPlyPlugin<double, 3>);
  sd::frontend::registerLoader(new LoaderPlyPlugin<double, 4>);
  sd::frontend::registerLoader(new LoaderPlyPlugin<float, 3>);
  sd::frontend::registerLoader(new LoaderPlyPlugin<float, 4>);
}

