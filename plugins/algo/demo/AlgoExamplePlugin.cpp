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
#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <PluginDefs.hpp>

#include <string>

#include <iostream>

template<typename T>
class AlgoExamplePlugin : public sd::frontend::ComputedImageViewAlgorithm<T> {

public:

  AlgoExamplePlugin()  : sd::frontend::ComputedImageViewAlgorithm<T>(m_name)
  {}

  virtual ~AlgoExamplePlugin() {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const {
    return m_name;
  }

  virtual AlgoExamplePlugin*
  clone() {
    return new AlgoExamplePlugin;
  }

  virtual bool
  run() {
#ifdef DEBUG
    std::cout << "Running " << m_name << std::endl;
#endif
    return true;
  }

private:
  static const std::string m_name;
};

template<typename T> const std::string AlgoExamplePlugin<T>::m_name = "Demonstration:AlgoExamplePlugin";

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  /*sd::frontend::ParameterList parameters;
  sd::frontend::Parameter p("date", "");
  p.setWidgetFactoryName("QtWidgetExamplePlugin");
  parameters.push_back(p);*/
  sd::libqt::SimpleStringParameter* spldble = new sd::libqt::SimpleStringParameter("hello");

  std::string parameters = "<parameters><parameter name=\"date\" type=\"string\"><default>31/04/2014</default><min>00/00/00</min><max>100/100/100</max><widget>QtWidgetExamplePlugin</widget></parameter></parameters>";
  sd::frontend::registerAlgorithm(new AlgoExamplePlugin<double>, parameters);
  sd::frontend::registerAlgorithm(new AlgoExamplePlugin<float>, parameters);
}
