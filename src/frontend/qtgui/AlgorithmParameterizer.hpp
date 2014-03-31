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

#ifndef SD_ALGORITHM_PARAMETERIZER_HPP
#define SD_ALGORITHM_PARAMETERIZER_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QTabWidget>
RESTORE_CONTEXT()

#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/Parameter.hpp>
#include <frontend/libqt/QtParametrizationWidget.hpp>

#include <string>
#include <map>
#include <vector>

namespace sd {
  namespace qtgui {

    class AlgorithmParameterizer
      : public QTabWidget
    {
      Q_OBJECT

      public:

      typedef std::map<std::string, libqt::QtParametrizationWidget*> WidgetList;

      public:

      AlgorithmParameterizer(std::vector<frontend::ParameterList>& parameters,
			     QWidget* parent);

      AlgorithmParameterizer(const std::string& xml_parameters,
           QWidget* parent);

      ~AlgorithmParameterizer();

      std::vector<frontend::ParameterList> getModifiedParameters();
      std::map<std::string, std::string> getModifiedXMLParameters();

    signals:

      void parametersChanged();

    private:

      void populateList(size_t listNo, const frontend::ParameterList& pl);

      QWidget* buildListWidget(size_t listNo);

    private:

      std::vector<frontend::ParameterList> m_parameters;

      std::vector<WidgetList> m_widgets;

    };

  }
}

#endif /*! SD_ALGORITHM_PARAMETERIZER_HPP */
