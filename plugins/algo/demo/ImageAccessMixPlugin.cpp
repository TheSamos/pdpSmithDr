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
#include <frontend/lib/AccessImageViewAlgorithm.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include <vector>

#include <iostream>

template<typename T>
class ImageAccessMixPlugin : public sd::frontend::AccessImageViewAlgorithm<T> {

public:

  ImageAccessMixPlugin() : sd::frontend::AccessImageViewAlgorithm<T>(m_name, m_input) {}

  virtual ~ImageAccessMixPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual ImageAccessMixPlugin* clone() {
    return new ImageAccessMixPlugin;
  }

  virtual const sd::core::Pixel<T> getAt(const sd::Point& p) const {
    sd::core::Pixel<T> p1(m_image1->getAt(p));
    p1.unlink();
    assert(p1 == m_image1->getAt(p));
    const sd::core::Pixel<T> p2(m_image2->getAt(p));
    for (unsigned int i=0; i<p1.nbChannels(); ++i)
      p1.setAt(i, round((double)m_alpha * (double)p1[i] + (double)(1-m_alpha) * (double)p2[i]));
    return p1;
  }
  
  virtual bool run() {
#ifdef DEBUG
    std::cout << "Running " << m_name << ": \n";
#endif
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 2) {
#ifdef DEBUG
      std::cout << "\tExpecting 2 parents exactly, found " << parents.size() << ". Abort!\n";
#endif
      return false;
    }

    m_image1 = static_cast<sd::core::ImageView_<T>*>(parents[0]);
    m_image2 = static_cast<sd::core::ImageView_<T>*>(parents[1]);
    
#ifdef DEBUG
    std::cout << "\tWorking with image1: " << m_image1->name() << std::endl;
    std::cout << "\tWorking with image2: " << m_image2->name() << std::endl;

    std::cout << m_image1->size() << std::endl;
    std::cout << m_image2->size() << std::endl;

    std::cout << m_image1->information() << std::endl;
    std::cout << m_image2->information() << std::endl;
#endif

    if (m_image1->information() != m_image2->information()) {
#ifdef DEBUG
      std::cout << "Incompatible images! Abort!\n";
#endif
      return false;
    }

    if (m_image1->channelType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
      std::cout << "Bad image type! Abort!\n" << m_image1->channelType().toString() << " != " << sd::core::DataType_<T>().toString() <<std::endl;
#endif
      return false;
    }

    if (m_image1->channelType() != m_image2->channelType()) {
#ifdef DEBUG
      std::cout << "Incompatible image types! Abort!\n";
#endif
      return false;
    }

    sd::frontend::AccessImageViewAlgorithm<T>::init(m_image1->information());
    sd::frontend::Parameter p;
    this->getParams("alpha", p);
    m_alpha = p.getFloat();
    /*
    // here compute result...
    auto it1 = m_image1->begin();
    auto it2 = m_image2->begin();
    auto endR = this->end();
    for (auto itR = this->begin(); itR != endR; ++itR, ++it1, ++it2)
      *itR = alpha * *it1 + (1.-alpha) * *it2;
      */
#ifdef DEBUG
    std::cout << "\talpha=" << m_alpha << std::endl;
#endif
    return true;
  }

private:
  
  sd::core::ImageView_<T>* m_image1;
  sd::core::ImageView_<T>* m_image2;
  float m_alpha;
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::AlgorithmInfo m_output;
};

template<typename T> const std::string ImageAccessMixPlugin<T>::m_name = "Demonstration:ImageAccessMixPlugin";
template<typename T> const sd::frontend::AlgorithmInfo ImageAccessMixPlugin<T>::m_input
= sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
								 sd::core::ObjectDescription::ANY_CARD,
								 sd::core::ObjectDescription::ANY_CARD),
			  sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
								 sd::core::ObjectDescription::ANY_CARD,
								 sd::core::ObjectDescription::ANY_CARD));

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
  sd::frontend::ParameterList parameters;
  parameters.push_back(sd::frontend::Parameter("alpha", 0.5, 0., 1.));

  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<sd::UINT8>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<sd::INT8>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<sd::UINT16>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<sd::INT16>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<float>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMixPlugin<double>, parameters);
}
