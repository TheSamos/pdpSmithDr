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

#include "UnmodifiableListWidget.hpp"
 
#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
RESTORE_CONTEXT()

#include <vector>
#include <cassert>

// :GLITCH: Using default Qt objects, all integrals are int and all floating points are double --> may change Parameter type (such as long-->int or short-->int)

namespace sd {
  
  namespace qtgui {
    
    namespace nativewidgets {

      namespace {

	QWidget*
	buildSelector(const frontend::Parameter& p, QWidget* parameterizer) {
	  QComboBox* comboBox = new QComboBox(parameterizer);
	  int currentIndex = 0;
	  int index = 0;
	  if (p.isInteger()) {
	    int value = p.getInteger();
	    const std::vector<int>& values = p.getPredefinedValuesAsVectorOf<int>();
	    for (auto it = values.begin(); it != values.end(); ++it, ++index) {
	      comboBox->addItem(QString::number(*it));
	      if (*it == value)
		currentIndex = index;
	    }
	  }
	  else if (p.isFloat()) {
	    double value = p.getFloat();
	    const std::vector<double>& values = p.getPredefinedValuesAsVectorOf<double>();
	    for (auto it = values.begin(); it != values.end(); ++it, ++index) {
	      comboBox->addItem(QString::number(*it));
	      if (*it == value)
		currentIndex = index;
	    }
	  }
	  else if (p.isBoolean()) {
	    // Does it make sense?
	    bool value = p.getBoolean();
	    const std::vector<bool>& values = p.getPredefinedValuesAsVectorOf<bool>();
	    for (auto it = values.begin(); it != values.end(); ++it, ++index) {
	      comboBox->addItem(QString::fromAscii(*it ? "true" : "false"));
	      if (*it == value)
		currentIndex = index;
	    }
	  }
	  else if (p.isString()) {
	    std::string value = p.getString();
	    const std::vector<std::string>& values = p.getPredefinedValuesAsVectorOf<std::string>();
	    for (auto it = values.begin(); it != values.end(); ++it, ++index) {
	      comboBox->addItem(QString::fromStdString(*it));
	      if (*it == value)
		currentIndex = index;
	    }
	  }
	  else
	    assert(!"Should never get here!");

	  comboBox->setCurrentIndex(currentIndex);
	  QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)),
			   parameterizer, SIGNAL(parametersChanged()));
	  return comboBox;
	}

      }

      UnmodifiableListWidget::UnmodifiableListWidget(frontend::Parameter& p)
	: QtParametrizationWidget(p) {}

      UnmodifiableListWidget::~UnmodifiableListWidget() {}

      void
      UnmodifiableListWidget::build(QWidget* parameterizer) {
	m_widget = buildSelector(m_p, parameterizer);

	m_layout = new QHBoxLayout;
	m_layout->addWidget(new QLabel(m_p.name().c_str()));
	m_layout->addWidget(m_widget);
      }

      namespace {

	void
	selectorReader(QWidget* w, frontend::Parameter& p) {
	  QComboBox* comboBox = dynamic_cast<QComboBox*>(w);
	  QString currentText = comboBox->currentText();
	  if (p.isBoolean()) {
	    p = (currentText.compare("true") == 0);
	  }
	  else if (p.isInteger()) {
	    p = currentText.toInt();
	  }
	  else if (p.isFloat()) {
	    p = currentText.toDouble();
	  }
	  else if (p.isString()) {
	    p = currentText.toStdString();
	  }
	  else
	    assert(!"Should never get here!");
	}

      }

      void
      UnmodifiableListWidget::updateParameter() {
	selectorReader(m_widget, m_p);
      }

      frontend::Parameter& UnmodifiableListWidget::updateXMLParameter() {
      	return m_p;
      }

    }
    
  }
  
}
