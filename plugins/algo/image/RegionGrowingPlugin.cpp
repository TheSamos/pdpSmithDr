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

#include <core/ViewNode.hpp>
#include <core/image/ImageView.hpp>
#include <core/image/Image.hpp>
#include <frontend/lib/ComputedImageViewAlgorithm.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <imageprocessing/RegionGrowing.hpp>

#include <vector>
#include <iostream>

template<typename T>
class RegionGrowingPlugin : public sd::frontend::ComputedImageViewAlgorithm<sd::UINT8> {

public:

  RegionGrowingPlugin() : sd::frontend::ComputedImageViewAlgorithm<sd::UINT8>(m_name, m_input) {}

  virtual ~RegionGrowingPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const
  {
    return m_name;
  }

  virtual RegionGrowingPlugin*
  clone()
  {
    return new RegionGrowingPlugin;
  }

  virtual bool
  run()
  {
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

    sd::core::ImageView_<T>* image = static_cast<sd::core::ImageView_<T>*>(parents[0]);
#ifdef DEBUG
    std::cout << "\tWorking with image: " << image->name() << std::endl;
#endif

    if (image->channelType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
      std::cout << "Bad image type! Abort!\n";
#endif
      return false;
    }

    sd::frontend::Parameter p;
    this->getParams("dimension", p);
    std::string dim = p.getString();
    if (dim != "2d" && dim != "3d")
      return false;

    this->getParams("connexity8_26", p);
    bool connexity8_26 = p.getBoolean();

    this->getParams("seedX", p);
    int seedX = p.getInteger();
    this->getParams("seedY", p);
    int seedY = p.getInteger();
    this->getParams("seedZ", p);
    int seedZ = p.getInteger();
    sd::Point seed(seedX, seedY, seedZ);

    this->getParams("inf. tolerance", p);
    double tolInf = p.getFloat();
    this->getParams("sup. tolerance", p);
    double tolSup = p.getFloat();

    sd::Size sz = image->size();
    this->init(image->information());
    sd::UINT8 valOn = 255;
    sd::UINT8 valOff = 0;
    sd::UINT8* resultData = this->getData();
    std::fill(resultData, resultData+sz.dataSize(), valOff);
    this->setMinMax(std::min(valOff, valOn),
		      std::max(valOff, valOn));
    
    sd::imageprocessing::RegionGrowing<T> rg;

    if (dim == "2d") {
      sd::imageprocessing::FloodFill2dSet ffset;
      rg.floodFill2d(*image, seed, tolInf, tolSup, ffset, connexity8_26);
      for (auto itr = ffset.begin(); itr != ffset.end(); ++itr) {
	sd::UINT16 x = std::get<0>(*itr);
	sd::UINT16 y = std::get<1>(*itr);
	sd::Point pt(x, y, seed.z());
	this->setAt(pt, valOn);
      }
    }
    else { // dim == "3d"
      sd::imageprocessing::FloodFill3dSet ffset;
      rg.floodFill3d(*image, seed, tolInf, tolSup, ffset, connexity8_26);
      for (auto itr = ffset.begin(); itr != ffset.end(); ++itr) {
	sd::UINT16 x = std::get<0>(*itr);
	sd::UINT16 y = std::get<1>(*itr);
	sd::UINT16 z = std::get<2>(*itr);
	sd::Point pt(x, y, z);
	this->setAt(pt, valOn);
      }
    }

    return true;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
};

template<typename T> const std::string RegionGrowingPlugin<T>::m_name = "RegionGrowingPlugin";
template<typename T> const sd::frontend::AlgorithmInfo RegionGrowingPlugin<T>::m_input = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
																			 sd::core::ObjectDescription::ANY_CARD,
																			 sd::core::ObjectDescription::ANY_CARD));

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  const std::vector<std::string> availableDimensions = { "2d", "3d" };
  sd::frontend::Parameter dimension("dimension", availableDimensions[0], availableDimensions);

  sd::frontend::ParameterList parameters;
  parameters.push_back(dimension);
  const sd::frontend::Any undefined = sd::frontend::Parameter::undefined;
  parameters.push_back(sd::frontend::Parameter("connexity8_26", false));
  parameters.push_back(sd::frontend::Parameter("seedX", 0, 0, undefined));
  parameters.push_back(sd::frontend::Parameter("seedY", 0, 0, undefined));
  parameters.push_back(sd::frontend::Parameter("seedZ", 0, 0, undefined));
  parameters.push_back(sd::frontend::Parameter("inf. tolerance", 1., 0., undefined));
  parameters.push_back(sd::frontend::Parameter("sup. tolerance", 1., 0., undefined));

  sd::frontend::registerAlgorithm(new RegionGrowingPlugin<sd::UINT8>, parameters);
  sd::frontend::registerAlgorithm(new RegionGrowingPlugin<sd::UINT16>, parameters);
  sd::frontend::registerAlgorithm(new RegionGrowingPlugin<float>, parameters);
  sd::frontend::registerAlgorithm(new RegionGrowingPlugin<double>, parameters);
}
