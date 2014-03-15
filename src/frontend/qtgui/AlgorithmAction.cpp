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

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QString>
#include <QMessageBox>
RESTORE_CONTEXT()

#include "MainWindow.hpp"
#include "AlgorithmAction.hpp"

#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/Any.hpp>
#include <frontend/lib/BlackBoxAlgorithm.hpp>

#include "GraphicViewsManager.hpp"

namespace sd {
  namespace qtgui {

    AlgorithmAction::AlgorithmAction(const std::string& actionName,
				     const std::string& algoName,
				     QObject* p)
      : QAction(QString::fromStdString(actionName), p),
	m_name(algoName),
	m_inputViews(),
	m_dialog(NULL),
	m_prototypes(),
	m_firstCall(true) {}

    AlgorithmAction::~AlgorithmAction() {
      if (m_dialog)
	delete m_dialog;
    }

    void
    AlgorithmAction::configure() {
      if (!m_prototypes.empty()) {
	m_firstCall = false;
	m_dialog = new AlgorithmConfiguratorDialog;

	connect(m_dialog, SIGNAL(viewSet(frontend::ViewInfoPtr*)), this, SLOT(inputChanged()));
	connect(m_dialog, SIGNAL(parametersChanged()), this, SLOT(parametersChanged()));
	connect(m_dialog, SIGNAL(previewOnOff()), this, SLOT(previewOnOff()));
	
	m_dialog->addInputs(getCompatibleAlgorithms(), m_inputViews);
	
	// add a selector for each parameter (int, float...)
	m_dialog->addParameters(m_prototypes.first()->expectedParameters());
      }
    }

    namespace {

      void
      configurationError(const QString& msg) {
	QMessageBox::critical(&MainWindow::instance(),
			      QObject::tr("Algorithm configuration error"),
			      msg);
      }

    }

    void
    AlgorithmAction::inputChanged() {
      m_dialog->updateInputs(getCompatibleAlgorithms());
      if (m_dialog->isPreviewActive())
	process(true);
    }

    void
    AlgorithmAction::parametersChanged() {
      if (m_dialog->isPreviewActive())
	process(true);
    }

    void
    AlgorithmAction::runAlgorithm() {
      if (m_name.empty())
	return;

      m_prototypes = frontend::queryAlgorithm(m_name);
      if (m_prototypes.empty()) {
	configurationError(tr("Algorithm not found: %1").arg(m_name.c_str()));
	return;
      }
      // m_prototypes can contain several prototypes of the same algorithm:
      // there is one prototype per accepted data type (int, float...)
      
      std::vector<frontend::Algorithm*> algos = getCompatibleAlgorithms();
      
      if (algos.size() == 0) {
	configurationError(tr("No existing view to select"));
	return;
      }
      
      if (m_firstCall)
	configure();

      m_dialog->updateInputs(algos);
      m_dialog->buildPreviews(algos.front()->output());
      
      if (m_dialog->isPreviewActive())
	process(true);

      int status = m_dialog->exec();
      m_dialog->removePreviews();
      if (status == QDialog::Rejected)
	return;

      process(false);
    }

    bool
    AlgorithmAction::configurationComplete() {
      return std::count_if(m_inputViews.begin(), m_inputViews.end(),
			   [&](const frontend::ViewInfoPtr& view) { return view; }
			   ) == (int) m_inputViews.size();
    }

    void
    AlgorithmAction::process(bool preview) {
      if (!m_dialog)
	return;

      // get modifications made by the user
      auto parameters = m_dialog->getModifiedParameters();

      if (!configurationComplete())
	return;

      bool noInput = m_inputViews.empty();

      // try to instantiate algorithm
      if (noInput) {
	auto algoPtr = m_prototypes.create();
	if (!algoPtr) {
	  configurationError(tr("Unable to create algorithm 1"));
	  return;
	}

	algoPtr->setParams(parameters);
	bool successful = algoPtr->run();
	if (!successful) {
	  configurationError(tr("Unable to run algorithm"));
	  return;
	}

	if (preview) {
	  m_dialog->updatePreviews(algoPtr->getOutputs());
	  // delete algoPtr; // not connected to view graph
	} else {
	  // add outputs to view graph
	  auto output = algoPtr->getOutputs();
	  for (auto it = output.begin(); it != output.end(); ++it) {
	    std::string err;
	    bool ok = frontend::ViewManager::instance().createView(*it, err);
	    if (!ok) {
	      configurationError(tr("Unable to get algorithm outputs"));
	      return;
	    }
	  }
	} // !preview
	
      } else { // have input
	frontend::ViewInfoPtr firstView = m_inputViews[0];
	frontend::Algorithm* algoPtr = NULL;
	
	const unsigned int nbAlgos = m_dialog->getCurrentAvailableAlgos().size();
	
	for (unsigned int i=0; i<nbAlgos && !algoPtr; ++i) {
	  algoPtr = m_dialog->getCurrentAvailableAlgos()[i]->clone();
	  algoPtr->setInputNodes(m_inputViews);
	  algoPtr->setParams(parameters);
	  if (algoPtr->run()) {
	    dynamic_cast<core::ViewNode*>(algoPtr)->setGhostView(preview);
	    dynamic_cast<core::ViewNode*>(algoPtr)->setDAG(frontend::Algorithm::dag());
	  } else {
	    //delete algoPtr;
	    algoPtr = NULL;
	  }
	}
	
	if (!algoPtr) {
	  configurationError(tr("Unable to create algorithm"));
	  return;
	}
	
	// get algorithm result: the algorithm itself
	std::string name = algoPtr->name();
	core::ViewNode* result = dynamic_cast<core::ViewNode*>(algoPtr);
	if (result->name() == "")
	  result->setName(name);
	
	if (preview) {
	  std::vector<core::ViewNode*> out;
	  out.push_back(result);
	  m_dialog->updatePreviews(out);
	} else {
	  std::string err;
	  /*bool ok = */frontend::ViewManager::instance().createView(result, err);
	  /*if (!ok) {
	    configurationError(tr("Unable to get algorithm outputs"));
	    return;
	    }*/
	}
      }
    }

    void
    AlgorithmAction::previewOnOff() {
      if (m_dialog->isPreviewActive())
	process(true);
    }

    std::vector<frontend::Algorithm*>
    AlgorithmAction::getCompatibleAlgorithms() const {
      std::vector<frontend::Algorithm*> result(0);
      
      const frontend::ViewManager::ViewsList& views = frontend::ViewManager::instance().getViews();
      sd::frontend::Query<sd::frontend::Algorithm>::const_iterator it = m_prototypes.begin();
      const sd::frontend::Query<sd::frontend::Algorithm>::const_iterator end = m_prototypes.end();
      sd::core::DynamicDimensionalCoordinates<uchar> check(0);
      for ( ; it != end; ++it) {
	check = sd::core::DynamicDimensionalCoordinates<uchar>(0);
	
	const std::vector<sd::frontend::Any>& inputs = (*it)->input().get();
	
	if (inputs.empty())
	  result.push_back(*it);
	
	if (!inputs.empty() && views.empty()) {
	  return result;
	}
	
	auto isImage = [](const frontend::ViewManager::ViewsList::value_type& v) {
	  return (v.second->getViewType() == frontend::ViewInfo::ImageType);
	};

	auto isMesh = [](const frontend::ViewManager::ViewsList::value_type& v) {
	  return (v.second->getViewType() == frontend::ViewInfo::MeshType);
	};

	for (size_t i = 0; i < inputs.size(); ++i) {
	  const frontend::Any& in = inputs[i];
	  
	  if (in.isA<sd::core::ObjectDescription>()) {
	    frontend::ViewManager::ViewsList::const_iterator itV = views.begin();
	    const frontend::ViewManager::ViewsList::const_iterator endV = views.end();
	    for ( ; itV != endV; ++itV) {
	      if (((*itV).second)->getViewType() == frontend::ViewInfo::ImageType) {
		const sd::core::ImageView* iv = ((frontend::ImageInfo*) ((*itV).second))->imagePtr.get();
		const sd::frontend::Any tmp((const sd::core::ImageView*) iv);
		const sd::core::ObjectDescription* ptr = (in.toPtr());
		if (tmp.isA(*ptr))
		  check.setAt(i, 1);
	      } else if (((*itV).second)->getViewType() == frontend::ViewInfo::MeshType) {
		const sd::core::MeshView* mv = ((frontend::MeshInfo*) ((*itV).second))->meshPtr.get();
		const sd::frontend::Any tmp((const sd::core::MeshView*) mv);
		const sd::core::ObjectDescription* ptr = (in.toPtr());
		if (tmp.isA(*ptr))
		  check.setAt(i, 1);
	      }
	    }
	  } else if (in.isA<core::ImageView*>())
	    check.setAt(i, std::count_if(views.begin(), views.end(), isImage) > 0 ? 1 : 0);
	  else if (in.isA<core::MeshView*>())
	    check.setAt(i, std::count_if(views.begin(), views.end(), isMesh) > 0 ? 1 : 0);
	  else {
#ifdef DEBUG
	    std::cerr << "ERROR : Undefined Type" << std::endl;
#endif
	  }
	}
	bool add = true;
	for (unsigned int j=0; j<check.nbDims(); ++j)
	  add &= check.getAt(j) == 1;
	if (add && check.nbDims()>0)
	  result.push_back(*it);
      }
      return result;
    }
    
  }
}
