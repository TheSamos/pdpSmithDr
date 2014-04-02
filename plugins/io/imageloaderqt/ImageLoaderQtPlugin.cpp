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

#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>

#include <PluginDefs.hpp>
#include <Types.hpp>
#include <utils/sdTypeUtils.hpp>

#include "ImageLoaderQt.hpp"

#ifdef DEBUG
#include <iostream>
#endif

#include <string>

#include <QImageReader>

template<typename T>
class ImageLoaderQtPlugin : public sd::frontend::Loader
{

public:

    ImageLoaderQtPlugin()
        : sd::frontend::Loader(m_name, m_output/*, m_parameters*/)
    {}

    virtual ~ImageLoaderQtPlugin()
    {
#ifdef DEBUG
        std::cout << "~" << m_name << std::endl;
#endif
    }

    virtual const std::string &
    name() const
    {
        return m_name;
    }

    virtual ImageLoaderQtPlugin *
    clone()
    {
        return new ImageLoaderQtPlugin;
    }

    virtual const std::string &
    formatName() const
    {
        static const std::string fName = "Standard 2D image formats";
        return fName;
    }

    virtual const std::string &
    fileExtensionFilters() const
    {
        static std::string ext = "";
        if (ext.empty())
        {
            QList<QByteArray> list = QImageReader::supportedImageFormats();
            for (int i = 0; i < list.size(); ++i)
            {
                ext = ext + "*." + QString(list[i]).toStdString();
                if (i != list.size() - 1)
                    ext += ";";
            }
        }
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

        std::cout << "Before Load" << std::endl;
        std::string filename = "";
        //sd::frontend::Parameter p;
        sd::libqt::SimpleStringParameter *p = 
        static_cast<sd::libqt::SimpleStringParameter *>(this->getXMLParams("filename"));

        std::cout << "Before getValue" << std::endl;
        filename = p->getValue();

        std::cout << "In Load: filename, " << filename << std::endl;

        if (filename.empty())
          return false;

        sd::io::ImageLoaderQt<T> loader;
        sd::core::ImageView_<T>* img = loader.loadImage(filename);
        if (!img)
          return false;
        

        img->addParent(this);
        std::cout << "added the parent" << std::endl;

        return true;
    }

private:
    static const std::string m_name;
    static const sd::frontend::AlgorithmInfo m_output;
    //static const std::string m_parameters;
};

template<typename T> const std::string ImageLoaderQtPlugin<T>::m_name = "ImageLoaderQtPlugin";
template<typename T> const sd::frontend::AlgorithmInfo ImageLoaderQtPlugin<T>::m_output = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>()));
//template<typename T> const sd::frontend::ParameterList ImageLoaderQtPlugin<T>::m_parameters = sd::frontend::make_parameter_list("filename", "");
//template<typename T> const std::string ImageLoaderQtPlugin<T>::m_parameters = "<parameters><parameter name=\"filename\" type=\"string\"></parameter></parameters>";
//<widget>ImageLoaderQtPlugin</widget>
// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
    std::string parameters = "<parameters><parameter name=\"filename\" type=\"string\"></parameter></parameters>";
    sd::frontend::registerLoader(new ImageLoaderQtPlugin<sd::UINT8>(), parameters);
}
