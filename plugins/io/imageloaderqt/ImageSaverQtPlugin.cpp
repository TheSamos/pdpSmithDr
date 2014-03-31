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

#include <core/ObjectDescription.hpp>
#include <core/image/ImageView.hpp>
#include <core/image/ImageView_.hpp>
#include <frontend/lib/Saver.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>

#include "ImageSaverQt.hpp"

#include <string>

#include <QImageReader>

template<typename T>
class ImageSaverQtPlugin : public sd::frontend::Saver {

public:

  ImageSaverQtPlugin()
    : sd::frontend::Saver(m_name, m_input, m_parameters) {}

  virtual ~ImageSaverQtPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const {
    return m_name;
  }

  virtual ImageSaverQtPlugin*
  clone() {
    return new ImageSaverQtPlugin;
  }

  virtual const std::string&
  formatName() const {
    static const std::string fName = "Standard 2D image formats";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const {
    static std::string ext = "";
    if (ext.empty()) {
      QList<QByteArray> list = QImageReader::supportedImageFormats();
      for (int i=0; i<list.size(); ++i) {
	 ext = ext + "*." + QString(list[i]).toStdString();
	 if (i!=list.size()-1)
	   ext += ";";
      }
    }
    return ext;
  }

  virtual Saver::Type
  saverType() const {
    return Saver::ImageSaver;
  }

  virtual bool
  save() {
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

    /*std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams("filename", p);
    if (p.isString())
      filename = p.getAs<std::string>();
    if (filename.empty())
      return false;*/
    
    /*sd::io::ImageSaverQt<T> imageSaver;
    bool successful = imageSaver.saveImage(image, filename);*/

    //return successful;
      return true;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const std::string m_parameters;
};

template<typename T> const std::string ImageSaverQtPlugin<T>::m_name = "ImageSaverQtPlugin";
template<typename T> const sd::frontend::AlgorithmInfo ImageSaverQtPlugin<T>::m_input = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
																			2,
																			sd::core::ObjectDescription::ANY_CARD)
														 );
//template<typename T> const sd::frontend::ParameterList ImageSaverQtPlugin<T>::m_parameters = sd::frontend::make_parameter_list("filename", "");
template<typename T> const std::string ImageSaverQtPlugin<T>::m_parameters = "filename";
// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
  sd::frontend::registerSaver(new ImageSaverQtPlugin<sd::UINT8>);
}
