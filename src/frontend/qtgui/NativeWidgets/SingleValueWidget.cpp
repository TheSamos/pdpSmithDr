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

#include "SingleValueWidget.hpp"
#include "SingleValueAccessor.hpp"

#include <SmithDRDefs.hpp>
#include <frontend/libqt/SimpleParameter.hpp>

NO_QT_WARNINGS()
#include <QLabel>
#include <QHBoxLayout>
RESTORE_CONTEXT()

#include <cassert>

// :GLITCH: Using default Qt objects, all integrals are int and all floating points are double --> may change Parameter type (such as long-->int or short-->int)

namespace sd {
  namespace qtgui {
    namespace nativewidgets {

      namespace {

	QWidget*
	buildEditor(sd::libqt::SDRParameter* p, QWidget* parameterizer)
	{
	  /*if (p.getDataType() == {
	    return buildBooleanEditor(p.getAs<bool>(), parameterizer);
	  }
	  else*/
	  std::string name = p->getName();
	   if (p->getDataType() == sd::libqt::Int) {
	  	sd::libqt::SimpleIntParameter* param = static_cast<sd::libqt::SimpleIntParameter*>(p);
	    /*int mini = p.isMinDefined() ? p.getMin<int>() : -UNDEFINED_VAL;
	    int maxi = p.isMaxDefined() ? p.getMax<int>() : +UNDEFINED_VAL;*/
	    int mini = param->getMin();
	    int maxi = param->getMax();
	    return buildIntegerEditor(param->getDefault(), mini, maxi, parameterizer);
	  }
	  else if (p->getDataType() == sd::libqt::Float) {
	  	sd::libqt::SimpleFloatParameter* param = static_cast<sd::libqt::SimpleFloatParameter*>(p);
	    /*double mini = p.isMinDefined() ? p.getMin<double>() : -UNDEFINED_VAL;
	    double maxi = p.isMaxDefined() ? p.getMax<double>() : +UNDEFINED_VAL;*/
	    float mini = param->getMin();
	    float maxi = param->getMax();
	    return buildFloatEditor(param->getDefault(), mini, maxi, parameterizer);
	  }/*
	  else if (p.isString()) {
	    return buildStringEditor(p.getAs<std::string>(), parameterizer);
	  }*/

	  assert(!"Should never get here!");
	  return NULL;
	}

      }

      SingleValueWidget::SingleValueWidget(sd::libqt::SDRParameter* p)
	: QtParametrizationWidget(p)
      {
      }

      SingleValueWidget::~SingleValueWidget()
      {
      }

      void
      SingleValueWidget::build(QWidget* parameterizer)
      {
	m_widget = buildEditor(m_p, parameterizer);

	m_layout = new QHBoxLayout;
	//m_layout->addWidget(new QLabel(m_p.name().c_str()));
	m_layout->addWidget(m_widget);
      }

      void
      SingleValueWidget::updateParameter()
      {
	/*if (m_p.isBoolean()) {
	  m_p = booleanReader(m_widget);
	}
	else if (m_p.isInteger()) {
	  m_p = integralReader(m_widget);
	}
	else if (m_p.isFloat()) {
	  m_p = floatingPointReader(m_widget);
	}
	else if (m_p.isString()) {
	  m_p = stringReader(m_widget);
	}
	else
	  assert(!"Should never get here!");*/
    }

          sd::libqt::SDRParameter*
      SingleValueWidget::updateXMLParameter()
      {
	/*if (m_p.isBoolean()) {
	  m_p = booleanReader(m_widget);
	  return m_p;
	}
	else if (m_p.isInteger()) {
	  m_p = integralReader(m_widget);
	  return m_p;
	}
	else if (m_p.isFloat()) {
	  m_p = floatingPointReader(m_widget);
	  return m_p;
	}
	else if (m_p.isString()) {
	  m_p = stringReader(m_widget);
	  return m_p;
	}
	else
	  assert(!"Should never get here!");*/
    }

    }
  }
}
