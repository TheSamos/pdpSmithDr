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
#include <frontend/lib/Saver.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include "SaverPgm.hpp"
#include "SaverPgm.hxx"

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
class SaverPgmPlugin : public sd::frontend::Saver
{

public:

  SaverPgmPlugin()
    : sd::frontend::Saver(m_name, m_input, m_parameters)
  {
  }

  virtual ~SaverPgmPlugin()
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

  virtual SaverPgmPlugin*
  clone()
  {
    return new SaverPgmPlugin;
  }

  virtual const std::string&
  formatName() const
  {
    static const std::string fName = "Portable Graymap/Bitmap/Color";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const
  {
    static const std::string ext = "*.pgm;*.pbm;*.ppm;*.pnm";
    return ext;
  }

  virtual Saver::Type
  saverType() const
  {
    return Saver::ImageSaver;
  }

  virtual bool
  save()
  {
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 1) {
      std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
      return false;
    }

    sd::core::ImageView_<T>* image = static_cast<sd::core::ImageView_<T>*>(parents[0]);

    if (image->channelType() != sd::core::DataType_<T>()) {
      std::cout << "Bad image type! Abort!\n";
      return false;
    }

    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;

    bool binary = true;
    this->getParams("binary", p);
    if (p.isBoolean())
      binary = p.getAs<bool>();

    sd::pgm::SaverPgm<T> saver;
    bool successful = saver.savePgmImage(image, filename, binary);

    return successful;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
};

template<typename T> const std::string SaverPgmPlugin<T>::m_name = "SaverPgmPlugin";
template<typename T> const sd::frontend::AlgorithmInfo SaverPgmPlugin<T>::m_input = sd::frontend::make_info((sd::core::ImageView*) 0);
template<typename T> const sd::frontend::ParameterList SaverPgmPlugin<T>::m_parameters = sd::frontend::make_parameter_list("filename", "", "binary", true);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
  sd::frontend::registerSaver(new SaverPgmPlugin<sd::UINT8>);
  sd::frontend::registerSaver(new SaverPgmPlugin<sd::UINT16>);
}
