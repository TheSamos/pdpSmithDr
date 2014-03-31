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

#ifndef SD_ALGORITHM_CONFIGURATOR_DIALOG_HPP
#define SD_ALGORITHM_CONFIGURATOR_DIALOG_HPP

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QCheckBox>
RESTORE_CONTEXT()

#include <core/ViewNode.hpp>
#include <frontend/lib/Any.hpp>
//#include <frontend/lib/Parameter.hpp>
#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>

#include "AlgorithmParameterizer.hpp"
#include "GraphicView/Widget.hpp"

#include <string>
#include <vector>
#include <map>

class QComboBox;

namespace sd {
  
  namespace qtgui {

    class AlgorithmConfiguratorDialog : public QDialog {
      
      Q_OBJECT

    public:

      AlgorithmConfiguratorDialog();

      ~AlgorithmConfiguratorDialog();

      bool addInputs(const std::vector<frontend::Algorithm*>& algos, std::vector<frontend::ViewInfoPtr>& views);

      //void addParameters(const frontend::ParameterList&);
      void addParameters(const std::string&);

      //std::vector<frontend::ParameterList> getModifiedParameters();

      std::map<std::string, std::string> getModifiedXMLParameters();

      void updatePreviews(const std::vector<core::ViewNode*>& nodes);
      
      void buildPreviews(const frontend::AlgorithmInfo& info, int nb = -1);
      
      void removePreviews();
      
      void updateInputs(std::vector<frontend::Algorithm*> algos);

      bool isPreviewActive() const;

      const std::vector<frontend::Algorithm*>& getCurrentAvailableAlgos() const {return m_currentAvailableAlgos;}
      
    signals:

      void viewSet(frontend::ViewInfoPtr*);
      void parametersChanged();
      void previewOnOff();

    private slots:

      void setView(QString);

    private:

      std::vector<frontend::Algorithm*>
      getAlgorithmsAcceptViewsFirst(const std::vector<frontend::Algorithm*>& algos,
				    const std::vector<frontend::ViewInfoPtr>& selectedViews) const;
      
    protected:

      QVBoxLayout* m_mainLayout;
      QVBoxLayout* m_inputLayout;
      QVBoxLayout* m_parametersLayout;
      QVBoxLayout* m_previewLayout;
      QHBoxLayout* m_graphicViewsLayout;
      QCheckBox* m_previewCB;
      AlgorithmParameterizer* m_parameterizer;
      std::vector<graphicView::Widget*> m_previews;
      
      std::vector<std::pair<QComboBox*, std::vector<QComboBox*> > > m_inputs;
      
    private:

      // the following map allows to get the corresponding data pointer
      // for the widget that sets the value (useful for multiple data of
      // the same type)
      std::map<QObject*, frontend::ViewInfoPtr*> m_views;
      std::vector<frontend::Algorithm*> m_currentAvailableAlgos;
      
    };

  }
}

#endif /*! SD_ALGORITHM_CONFIGURATOR_DIALOG_HPP */
