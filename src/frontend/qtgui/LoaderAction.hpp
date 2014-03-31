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

#ifndef SD_LOADER_ACTION_HPP
#define SD_LOADER_ACTION_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QAction>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/Query.hpp>
#include <frontend/lib/Loader.hpp>

#include "AlgorithmConfiguratorDialog.hpp"

#include <string>
#include <vector>

namespace sd {
  namespace qtgui {

    class LoaderAction
      : public QAction
    {
      Q_OBJECT

      public:

      LoaderAction(const std::string& actionName,
		   QObject* parent);

      ~LoaderAction();

    public slots:

      void runLoader();

    private slots:

      void inputChanged();
      void parametersChanged();
      void previewOnOff();

    private:

      void configure();
      void process(bool);

    private:

      const std::string m_name;
      AlgorithmConfiguratorDialog* m_dialog;
      frontend::Query<frontend::Loader> m_prototypes;
      bool m_firstCall;

    };

  }
}

#endif /*! SD_LOADER_ACTION_HPP */
