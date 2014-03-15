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

#include "SaverObj.hpp"

#include <iostream>
#include <fstream>
#include <string>

template<typename T, unsigned int N>
class SaverObjPlugin : public sd::frontend::Saver {

public:

  SaverObjPlugin()
    : sd::frontend::Saver(m_name, m_input, m_parameters)
  {}

  virtual ~SaverObjPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const {
    return m_name;
  }

  virtual SaverObjPlugin*
  clone() {
    return new SaverObjPlugin;
  }

  virtual const std::string&
  formatName() const {
    static const std::string fName = "Obj format";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const {
    static const std::string ext = "*.obj;";
    return ext;
  }

  virtual Saver::Type
  saverType() const {
    return Saver::MeshSaver;
  }

  virtual bool
  save() {
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 1) {
      std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
      return false;
    }
    
    //work only with face degree = 3 for now
    sd::core::MeshView_<T, N>* mesh = static_cast<sd::core::MeshView_<T, N>*>(parents[0]);
 
    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;
    
    bool saveVertexNormals = true;
    this->getParams("VertexNormals", p);
    if (p.isBoolean())
      saveVertexNormals = p.getAs<bool>();
    
    bool saveVertexColors = true;
    this->getParams("VertexColors", p);
    if (p.isBoolean())
      saveVertexColors = p.getAs<bool>();
    
    bool saveFaceNormals = false;
    this->getParams("FaceNormals", p);
    if (p.isBoolean())
      saveFaceNormals = p.getAs<bool>();
    
    bool saveFaceColors = false;
    this->getParams("FaceColors", p);
    if (p.isBoolean())
      saveFaceColors = p.getAs<bool>();
    
    sd::obj::SaverObj<T, N> saver;
    bool successful = saver.saveObjImage(mesh, filename,
					 saveVertexNormals, saveVertexColors,
					 saveFaceNormals, saveFaceColors);

    return successful;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
};

template<typename T, unsigned int N> const std::string SaverObjPlugin<T, N>::m_name = "SaverObjPlugin";
template<typename T, unsigned int N> const sd::frontend::AlgorithmInfo SaverObjPlugin<T, N>::m_input = sd::frontend::make_info((sd::core::MeshView*) NULL);
template<typename T, unsigned int N> const sd::frontend::ParameterList SaverObjPlugin<T, N>::m_parameters = sd::frontend::make_parameter_list("filename", "", "VertexNormals;VertexColors;FaceNormals;FaceColors", true);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::registerSaver(new SaverObjPlugin<sd::DOUBLE, 3>);
  sd::frontend::registerSaver(new SaverObjPlugin<sd::DOUBLE, 4>);
  sd::frontend::registerSaver(new SaverObjPlugin<sd::FLOAT, 3>);
  sd::frontend::registerSaver(new SaverObjPlugin<sd::FLOAT, 4>);
}
