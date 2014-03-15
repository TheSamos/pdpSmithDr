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

#include <core/image/ImageView.hpp>
#include <core/image/ImageView_.hpp>
#include <frontend/lib/Loader.hpp>
#include <frontend/lib/LoaderManager.hpp>

#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include "LoaderPgm.hpp"
#include "LoaderPgm.hxx"

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
class LoaderSdp3dPlugin : public sd::frontend::Loader
{

public:

  LoaderSdp3dPlugin()
    : sd::frontend::Loader(m_name, m_output, m_parameters)
  {
  }

  virtual ~LoaderSdp3dPlugin()
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

  virtual LoaderSdp3dPlugin*
  clone()
  {
    return new LoaderSdp3dPlugin;
  }

  virtual const std::string&
  formatName() const
  {
    static const std::string fName = "SmithDR 3D Portable Graymap/Bitmap/Color";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const
  {
    static const std::string ext = "*.sdp3d";
    return ext;
  }

  virtual Loader::Type
  loaderType() const
  {
    return Loader::ImageLoader;
  }

  virtual bool
  load()
  {
    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;

    sd::pgm::LoaderPgm<T> loader;
    sd::core::ImageView_<T>* img = loader.loadPgmImage(filename);
    if (!img)
      return false;

    img->addParent(this);

    return true;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_output;
  static const sd::frontend::ParameterList m_parameters;
};

template<typename T> const std::string LoaderSdp3dPlugin<T>::m_name = "LoaderSdp3dPlugin";
template<typename T> const sd::frontend::AlgorithmInfo LoaderSdp3dPlugin<T>::m_output = sd::frontend::make_info((sd::core::ImageView*) 0);
template<typename T> const sd::frontend::ParameterList LoaderSdp3dPlugin<T>::m_parameters = sd::frontend::make_parameter_list("filename", "");

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
  sd::frontend::registerLoader(new LoaderSdp3dPlugin<sd::UINT8>);
  sd::frontend::registerLoader(new LoaderSdp3dPlugin<sd::UINT16>);
}
