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
#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <PluginDefs.hpp>

#include <vector>

#include <iostream>

/*
 * It is simpler to implement this algo as a ComputedImageViewAlgorithm.
 * It invalidates getAt/setAt and iterators. If the algo is implemented as an ImageViewAlgorithm, it must overload these methods.
 */

template<typename T>
class ImageAccessMirrorPlugin : public sd::frontend::AccessImageViewAlgorithm<T> {

public:

  ImageAccessMirrorPlugin() : sd::frontend::AccessImageViewAlgorithm<T>(m_name, m_input/*, m_parameters*/),
			      m_horizontal(true) {}

  virtual ~ImageAccessMirrorPlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string& name() const {
    return m_name;
  }

  virtual ImageAccessMirrorPlugin* clone() {
    return new ImageAccessMirrorPlugin;
  }
  
  virtual const sd::core::Pixel<T> getAt(const sd::Point& p) const {
    return m_iv->getAt(getTransformedPoint(p));
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
    m_iv = static_cast<sd::core::ImageView_<T>*>(parents[0]);
    sd::frontend::AccessImageViewAlgorithm<T>::init(m_iv->information());

    sd::libqt::SimpleBoolParameter *p = static_cast<sd::libqt::SimpleBoolParameter *>(this->getXMLParams("horizontal"));
    m_horizontal = p->getValue();
    return true;
  }

private:
  
  sd::Point
  getTransformedPoint(const sd::Point& p) const {
    sd::Point p2 = p;
    if (m_horizontal)
      p2.setAt(0, this->size().width()-1 - p[0]);
    else
      p2.setAt(1, this->size().height()-1 - p[1]);
    return p2;
  }
  
private:
  
  bool m_horizontal;
  const sd::core::ImageView_<T>* m_iv;
  
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  
};

template<typename T>
const std::string ImageAccessMirrorPlugin<T>::m_name = "Demonstration:ImageAccessMirrorPlugin";
template<typename T>
const sd::frontend::AlgorithmInfo ImageAccessMirrorPlugin<T>::m_input = 
		   sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
										  sd::core::ObjectDescription::ANY_CARD,
										  sd::core::ObjectDescription::ANY_CARD));

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {

    std::string parameters = "<parameters><parameter name=\"horizontal\" type=\"bool\"> \
                                  <default>false</default> \
                            </parameter></parameters>";
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<sd::UINT8>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<sd::INT8>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<sd::UINT16>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<sd::INT16>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<float>, parameters);
  sd::frontend::registerAlgorithm(new ImageAccessMirrorPlugin<double>, parameters);
}
