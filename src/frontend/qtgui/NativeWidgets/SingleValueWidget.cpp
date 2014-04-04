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

namespace sd
{
namespace qtgui
{
namespace nativewidgets
{

namespace
{

QWidget *
buildEditor(sd::libqt::SDRParameter *p, QWidget *parameterizer)
{
    std::string name = p->getName();
    if (p->getDataType() == sd::libqt::Bool) {
      sd::libqt::SimpleBoolParameter *param = static_cast<sd::libqt::SimpleBoolParameter *>(p);
      return buildBooleanEditor(param->getDefault(), parameterizer);
    }
    else
    
    if (p->getDataType() == sd::libqt::Int)
    {
        int mini, maxi;
        sd::libqt::SimpleIntParameter *param = static_cast<sd::libqt::SimpleIntParameter *>(p);
        std::cout << "building sinlgevalue int ------  " << std::endl;
        if(param->isMinDefined() && param->isMaxDefined()){
          mini = param->getMin();
          maxi = param->getMax();
      }
      else{
        mini = -UNDEFINED_VAL;
        maxi = UNDEFINED_VAL;
      }
        return buildIntegerEditor(param->getDefault(), mini, maxi, parameterizer);
    }
    else if (p->getDataType() == sd::libqt::Float)
    {
        sd::libqt::SimpleFloatParameter *param = static_cast<sd::libqt::SimpleFloatParameter *>(p);
        std::cout << "building sinlgevalue float " << std::endl;
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

SingleValueWidget::SingleValueWidget(sd::libqt::SDRParameter *p)
    : QtParametrizationWidget(p)
{
}

SingleValueWidget::~SingleValueWidget()
{
}

void
SingleValueWidget::build(QWidget *parameterizer)
{
    m_widget = buildEditor(m_p, parameterizer);

    m_layout = new QHBoxLayout;
    m_layout->addWidget(new QLabel(m_p->getName().c_str()));
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

sd::libqt::SDRParameter *
SingleValueWidget::updateXMLParameter()
{

if (m_p->getDataType() == sd::libqt::Float)
    {
    	sd::libqt::SimpleFloatParameter *p = static_cast<sd::libqt::SimpleFloatParameter *>(m_p);
    	p->setValue(floatingPointReader(m_widget));
      m_p->print();
        return m_p;
    }
    else if(m_p->getDataType() == sd::libqt::Bool){
      sd::libqt::SimpleBoolParameter *p = static_cast<sd::libqt::SimpleBoolParameter *>(m_p);
      p->setValue(booleanReader(m_widget));
      m_p->print();
        return m_p;
    }


    else if(m_p->getDataType() == sd::libqt::String)
    {
    	sd::libqt::SimpleStringParameter *p = static_cast<sd::libqt::SimpleStringParameter *>(m_p);
    	p->setValue(stringReader(m_widget));
       m_p->print();
        return m_p;
    }
    else if(m_p->getDataType() == sd::libqt::Int)
    {
      sd::libqt::SimpleIntParameter *p = static_cast<sd::libqt::SimpleIntParameter *>(m_p);
      p->setValue(integralReader(m_widget));
      m_p->print();
        return m_p;
    }

    else
    {
      std::cout << "SingleValueWidget: Unknown type." << std::endl;
        assert(!"Should never get here!");
    }
}

}
}
}
