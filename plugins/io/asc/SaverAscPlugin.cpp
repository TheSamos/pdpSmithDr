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

#include <core/mesh/MeshView.hpp>
#include <core/mesh/MeshView_.hpp>
#include <frontend/lib/Saver.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include "SaverAsc.hpp"
#include "SaverAsc.hxx"

#include <iostream>
#include <fstream>
#include <string>

template<typename T, unsigned int N>
class SaverAscPlugin : public sd::frontend::Saver
{

public:

  SaverAscPlugin()
    : sd::frontend::Saver(m_name, m_input, m_parameters)
  {
  }

  virtual ~SaverAscPlugin()
  {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const
  {
    return m_name;
  }

  virtual SaverAscPlugin*
  clone()
  {
    return new SaverAscPlugin;
  }

  virtual const std::string&
  formatName() const
  {
    static const std::string fName = "Asc nuage de points";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const
  {
    static const std::string ext = "*.asc;";
    return ext;
  }

  virtual Saver::Type
  saverType() const
  {
    return Saver::MeshSaver;
  }

  virtual bool
  save()
  {
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 1) {
      std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
      return false;
    }
    
    sd::core::MeshView_<T, N>* mesh = static_cast<sd::core::MeshView_<T, N>*>(parents[0]);
 
   
    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;
    bool saveVertexNormals = true;
    this->getParams("normals", p);
    if (p.isBoolean())
      saveVertexNormals = p.getAs<bool>();
    sd::asc::SaverAsc<T, N> saver;
    bool successful = saver.saveAsc(mesh, filename, saveVertexNormals);

    return successful;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
};

template<typename T, unsigned int N> const std::string SaverAscPlugin<T, N>::m_name = "SaverAscPlugin";
template<typename T, unsigned int N> const sd::frontend::AlgorithmInfo SaverAscPlugin<T, N>::m_input = sd::frontend::make_info((sd::core::MeshView*) 0);
template<typename T, unsigned int N> const sd::frontend::ParameterList SaverAscPlugin<T, N>::m_parameters = sd::frontend::make_parameter_list("filename", "", "normals", true);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::registerSaver(new SaverAscPlugin<sd::DOUBLE, 3>);
  sd::frontend::registerSaver(new SaverAscPlugin<sd::FLOAT, 3>);
}
