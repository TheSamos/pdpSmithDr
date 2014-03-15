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

#include "SingleValueAccessor.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
RESTORE_CONTEXT()

namespace sd {
  namespace qtgui {
    namespace nativewidgets {

      QWidget*
      buildBooleanEditor(bool b, QWidget* parameterizer)
      {
	QCheckBox* cb = new QCheckBox();
	cb->setChecked(b);
	QObject::connect(cb, SIGNAL(stateChanged(int)),
			 parameterizer, SIGNAL(parametersChanged()));
	return cb;
      }

      QWidget*
      buildIntegerEditor(int value, int mini, int maxi, QWidget* parameterizer)
      {
	QSpinBox* spinBox = new QSpinBox;
	spinBox->setRange(mini, maxi);
	spinBox->setValue(value);
	if (mini == -UNDEFINED_VAL || maxi == +UNDEFINED_VAL)
	  spinBox->setSingleStep(1);
	else
	  spinBox->setSingleStep(std::max((maxi-mini)/20, 1));
	QObject::connect(spinBox, SIGNAL(valueChanged(int)),
			 parameterizer, SIGNAL(parametersChanged()));
	return spinBox;
      }

      QWidget*
      buildFloatEditor(double value, double mini, double maxi, QWidget* parameterizer)
      {
	QDoubleSpinBox* spinBox = new QDoubleSpinBox;
	spinBox->setRange(mini, maxi);
	spinBox->setValue(value);
	if (mini == -UNDEFINED_VAL || maxi == +UNDEFINED_VAL)
	  spinBox->setSingleStep(1);
	else
	  spinBox->setSingleStep(std::max((maxi-mini)/20, 0.01));
	spinBox->setDecimals(10);
	QObject::connect(spinBox, SIGNAL(valueChanged(double)),
			 parameterizer, SIGNAL(parametersChanged()));
	return spinBox;
      }

      QWidget*
      buildStringEditor(const std::string& value, QWidget* parameterizer)
      {
	QLineEdit* edit = new QLineEdit;
	edit->setText(QString::fromStdString(value));
	QObject::connect(edit, SIGNAL(textChanged(const QString&)),
			 parameterizer, SIGNAL(parametersChanged()));
	return edit;
      }

      bool
      booleanReader(QWidget* w)
      {
	return dynamic_cast<QCheckBox*>(w)->isChecked();
      }

      int
      integralReader(QWidget* w)
      {
	return dynamic_cast<QSpinBox*>(w)->value();
      }

      double
      floatingPointReader(QWidget* w)
      {
	return dynamic_cast<QDoubleSpinBox*>(w)->value();
      }

      std::string
      stringReader(QWidget* w)
      {
	return dynamic_cast<QLineEdit*>(w)->text().toStdString();
      }

    }
  }
}
