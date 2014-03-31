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

#ifndef SD_ALGORITHM_ACTION_HPP
#define SD_ALGORITHM_ACTION_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QAction>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/Query.hpp>
#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/ViewManager.hpp>
#include <frontend/libqt/QtXMLParamParser.hpp>
//#include <frontend/lib/Parameter.hpp>

#include "AlgorithmConfiguratorDialog.hpp"

#include <string>
#include <vector>

namespace sd {
  namespace qtgui {

    class AlgorithmAction : public QAction {
      Q_OBJECT

    public:

      AlgorithmAction(const std::string& actionName,
		      const std::string& algoName,
		      QObject* parent);

      ~AlgorithmAction();

      void setXMLParameterized(bool xml_parameterized);

    public slots:

      void runAlgorithm();

    private slots:

      void inputChanged();
      void parametersChanged();
      void previewOnOff();

    private:

      void configure();
      bool configurationComplete();
      void process(bool);
      std::vector<frontend::Algorithm*> getCompatibleAlgorithms() const;

    private:

      const std::string m_name;
      std::vector<frontend::ViewInfoPtr> m_inputViews;
      AlgorithmConfiguratorDialog* m_dialog;
      frontend::Query<frontend::Algorithm> m_prototypes;
      bool m_firstCall;
      bool m_xml_parameterized = false;

    };

  }
}

#endif /*! SD_ALGORITHM_ACTION_HPP */

