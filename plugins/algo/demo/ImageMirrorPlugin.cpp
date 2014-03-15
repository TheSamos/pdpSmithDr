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

#include <frontend/lib/ComputedImageViewAlgorithm.hpp>

#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>

#include <vector>

#include <iostream>

/*
 * It is simpler to implement this algo as a ComputedImageViewAlgorithm.
 * It invalidates getAt/setAt and iterators. If the algo is implemented as an ImageViewAlgorithm, it must overload these methods.
 */

template<typename T>
class ImageMirrorPlugin : public sd::frontend::ComputedImageViewAlgorithm<T> {

public:

  ImageMirrorPlugin() : sd::frontend::ComputedImageViewAlgorithm<T>(m_name, m_input, m_parameters),
			m_horizontal(false) {}

  virtual ~ImageMirrorPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual ImageMirrorPlugin* clone() {
    return new ImageMirrorPlugin;
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

    sd::core::ImageView_<T>* image = static_cast<sd::core::ImageView_<T>*>(parents[0]);
#ifdef DEBUG
    std::cout << "\tWorking with image: " << image->name() << std::endl;
#endif

    if (image->channelType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
      std::cerr << image->channelType().toString() << " != " << sd::core::DataType_<T>().toString() << std::endl;
      std::cout << "Bad image type! Abort!\n";
#endif
      return false;
    }

    sd::frontend::Parameter p;
    this->getParams("horizontal", p);
    m_horizontal = p.getBoolean();
#ifdef DEBUG
    std::cout << "\t" << (m_horizontal ? "horizontal" : "vertical") << std::endl;
#endif
    
    // for a ComputedImageViewAlgorithm
    sd::Size sz = image->size();
    this->init(image->information());
    T* resultData = this->getData();
    std::fill(resultData, resultData+sz.dataSize(), 0);
    this->setMinMax(0, image->getMax());
    
    sd::Point p1(0, 0, 0);
    sd::Point p2(0, 0, 0);
    // compute (for a ComputedImageViewAlgorithm)
    if (m_horizontal) {
      for (size_t z = 0; z < sz.depth(); ++z) {
#ifdef DEBUG
	std::cerr << "Layer " << (int)z << " of " << sz.depth() << std::endl;
#endif
	p1.setZ(z); p2.setZ(z);
	for (size_t y = 0; y < sz.height(); ++y) {
	  p1.setY(y); p2.setY(y);
	  for (size_t x = 0; x < sz.width(); ++x) {
	    p1.setX(x); p2.setX(sz.width()-1-x);
	    this->setAt(p2, image->getAt(p1));
	  }
	}
      }
    }
    else {
      for (size_t z = 0; z < sz.depth(); ++z) {
	p1.setZ(z); p2.setZ(z);
#ifdef DEBUG
	std::cerr << "Layer " << (int)z << " of " << sz.depth() << std::endl;
#endif
	for (size_t y = 0; y < sz.height(); ++y) {
	  p1.setY(y); p2.setY(sz.height()-1-y);
	  for (size_t x = 0; x < sz.width(); ++x) {
	    p1.setX(x); p2.setX(x);
	    this->setAt(p2, image->getAt(p1));
	  }
	}
      }
    }


    return true;
  }

private:
  bool m_horizontal;
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::ParameterList m_parameters;
};

template<typename T> const std::string ImageMirrorPlugin<T>::m_name = "Demonstration:ImageMirrorPlugin";
template<typename T> const sd::frontend::AlgorithmInfo ImageMirrorPlugin<T>::m_input = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
																		       sd::core::ObjectDescription::ANY_CARD,
																		       sd::core::ObjectDescription::ANY_CARD));
template<typename T> const sd::frontend::ParameterList ImageMirrorPlugin<T>::m_parameters = sd::frontend::make_parameter_list("horizontal", false);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<sd::UINT8>);
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<sd::INT8>);
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<sd::UINT16>);
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<sd::INT16>);
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<float>);
  sd::frontend::registerAlgorithm(new ImageMirrorPlugin<double>);
}
