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
#include <frontend/lib/BlackBoxAlgorithm.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include <vector>

#include <iostream>

template<typename T>
class BuildCheckerboardImagePlugin : public sd::frontend::BlackBoxAlgorithm {
  
public:
  
  BuildCheckerboardImagePlugin()
    : sd::frontend::BlackBoxAlgorithm(m_name, m_output, m_input/*, m_parameters*/)
  {}

  virtual ~BuildCheckerboardImagePlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const {
    return m_name;
  }

  virtual BuildCheckerboardImagePlugin*
  clone() {
    return new BuildCheckerboardImagePlugin();
  }

  virtual bool
  run() {
#ifdef DEBUG
    std::cout << "Running " << m_name << ": \n";
#endif
    const std::vector<sd::core::ViewNode*>& parents = this->getPreviousViewNodes();
    if (parents.size() != 0) {
#ifdef DEBUG
      std::cout << "\tExpecting 0 parent exactly, found " << parents.size() << ". Abort!\n";
#endif
      return false;
    }

    // get parameters
    /*sd::frontend::Parameter pWidth, pHeight, pSqSize;
    this->getParams("width", pWidth, "height", pHeight, "sqSize", pSqSize);

    size_t width = pWidth.getInteger();
    size_t height = pHeight.getInteger();
    size_t sqSize = pSqSize.getInteger();*/

    sd::libqt::SimpleIntParameter *p1 = static_cast<sd::libqt::SimpleIntParameter *>(this->getXMLParams("width"));
    size_t width = p1->getValue();

    sd::libqt::SimpleIntParameter *p2 = static_cast<sd::libqt::SimpleIntParameter *>(this->getXMLParams("height"));
    size_t height = p2->getValue();

    sd::libqt::SimpleIntParameter *p3 = static_cast<sd::libqt::SimpleIntParameter *>(this->getXMLParams("sqSize"));
    size_t sqSize = p3->getValue();




#ifdef DEBUG
    std::cout << "\twidth=" << width << std::endl;
    std::cout << "\theight=" << height << std::endl;
    std::cout << "\tsqSize=" << sqSize << std::endl;
#endif

    // create result image
    sd::Size sz(width, height);
    sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS, sz);
    sd::core::Image<T> *result = new sd::core::Image<T>(info, "Checkerboard");
    T* resultData = new T[sz.dataSize()];
    std::fill(resultData, resultData+sz.dataSize(), 0);
    result->setData(resultData);
    result->setMinMax(0, 255);
    result->addParent(this);

    // here compute result...
    bool oddRow = false;
    bool oddColumn = false;
    int pxlR = 0, pxlC = 0;
    for (auto it = result->begin(); it != result->end(); ++it) {
      bool b = ( (oddRow && !oddColumn) || (!oddRow && oddColumn) );
      *it = (b ? 0 : 255);

      ++ pxlC;
      if (pxlC % width == 0) { // next pixel line
	oddColumn = false;
	pxlC = 0;
	++ pxlR;
	if (pxlR % sqSize == 0) { // next row
	  oddRow = !oddRow;
	  pxlR = 0;
	}
      }
      else if (pxlC % sqSize == 0) { // next square (same row)
	oddColumn = !oddColumn;
      }
    }

    return true;
}

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_input;
  static const sd::frontend::AlgorithmInfo m_output;
  //static const sd::frontend::ParameterList m_parameters;
};

template<typename T> const std::string BuildCheckerboardImagePlugin<T>::m_name = "Demonstration:BuildCheckerboardImagePlugin";
template<typename T> const sd::frontend::AlgorithmInfo BuildCheckerboardImagePlugin<T>::m_input = sd::frontend::make_info();
template<typename T> const sd::frontend::AlgorithmInfo BuildCheckerboardImagePlugin<T>::m_output = sd::frontend::make_info((sd::core::ImageView*) 0);
//template<typename T> const sd::frontend::ParameterList BuildCheckerboardImagePlugin<T>::m_parameters = sd::frontend::make_parameter_list("width", 128, "height", 128, "sqSize", 16);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{

    std::string parameters = "<parameters><parameter name=\"width\" type=\"int\"> \
                                  <default>128</default> \
                            </parameter> \
                            <parameter name=\"height\" type=\"int\"> \
                                  <default>128</default> \
                            </parameter> \
                            <parameter name=\"sqSize\" type=\"int\"> \
                                  <default>16</default> \
                            </parameter> \
                            </parameters>";
  sd::frontend::registerAlgorithm(new BuildCheckerboardImagePlugin<sd::UINT8>, parameters);
}
