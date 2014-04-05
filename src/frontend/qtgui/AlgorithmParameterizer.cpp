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

#include "AlgorithmParameterizer.hpp"
#include <frontend/lib/ParametrizationWidget.hpp>
#include <frontend/lib/ParametrizationWidgetFactoryManager.hpp>
#include <frontend/libqt/QtParametrizationWidget.hpp>
#include "NativeWidgets/SingleValueWidget.hpp"
#include "NativeWidgets/MultipleValuesWidget.hpp"
#include "NativeWidgets/UnmodifiableListWidget.hpp"
#include <frontend/libqt/QtXMLParamParser.hpp>
#include <frontend/libqt/QtXMLValueGenerator.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
RESTORE_CONTEXT()

#include <cassert>

// :GLITCH: Using default Qt objects, all integrals are int and all floating points are double --> may change Parameter type (such as long-->int or short-->int)

namespace sd
{

namespace qtgui
{

namespace
{

libqt::QtParametrizationWidget *
buildEditor(sd::libqt::SDRParameter *p, QWidget *parameterizer)
{
    std::string widgetFactoryName = p->getWidgetName();
    bool useSpecificWidget = !(widgetFactoryName.empty());
    if (useSpecificWidget)
    {
        auto factory = frontend::queryWidgetFactory(widgetFactoryName);
        if (factory)
        {
            auto widget = factory->createWidget(p);
            if (widget)
            {
                libqt::QtParametrizationWidget *native = dynamic_cast<libqt::QtParametrizationWidget *>(widget);
                if (native)
                {
                    native->build(parameterizer);
                    return native;
                }
                delete widget;
            }
        }
        else
        {
            std::cerr << "Unable to get widget \"" << widgetFactoryName << "\". Use default widget.\n\n";
        }
    }

    // default widgets
    libqt::QtParametrizationWidget *widget = NULL;
    /*if (p.hasPredefinedValues())
    {
        widget = new nativewidgets::UnmodifiableListWidget(p);
    }
    else if (p.isAtomic())
    {
        widget = new nativewidgets::SingleValueWidget(p);
    }
    else if (p.isVector())
    {
        widget = new nativewidgets::MultipleValuesWidget(p);
    }
    else
        assert(!"Should never get here!");*/

    widget = new nativewidgets::SingleValueWidget(p);
    assert(widget != NULL);
    widget->build(parameterizer);
    return widget;
}

}

/*
 * AlgorithmParameterizer
 */
/*AlgorithmParameterizer::AlgorithmParameterizer(std::vector<frontend::ParameterList> &parameters, QWidget *p)
    : QTabWidget(p), m_parameters(parameters), m_widgets()
{
    m_widgets.resize(parameters.size());
    for (size_t i = 0; i < parameters.size(); ++i)
    {
        if (!parameters[i].empty())
        {
            addTab(buildListWidget(i), tr("Parameters set ") + QString("#%1").arg(i));
        }
    }
}*/

AlgorithmParameterizer::AlgorithmParameterizer(const std::string& xml_parameters, QWidget* parent) 
    : QTabWidget(parent), /*m_parameters(),*/ m_widgets()
    {

      //std::cout << "Before parsing" << std::endl;

      sd::libqt::QtXMLParamParser defaultParamParser(xml_parameters);
      sd::libqt::ParameterList p = defaultParamParser.getParameterList();
      //std::cout << "Before parsing" << p[0]->getName() << std::endl;



      /*for(int i = 0; i < p.size(); i++)
        p[i]->print();*/

      /*std::vector<frontend::ParameterList> paramList();
      paramList.pushback(p);*/

      m_parameters.push_back(p);

      m_widgets.resize(m_parameters.size());
      for (size_t i = 0; i < m_parameters.size(); ++i)
              addTab(buildListWidget(i), tr("XML parameter set"));
          
      
    }

AlgorithmParameterizer::~AlgorithmParameterizer()
{
    for (size_t i = 0; i < m_widgets.size(); ++i)
        for (auto itr = m_widgets[i].begin(); itr != m_widgets[i].end(); ++itr)
            delete itr->second;
}

QWidget *
AlgorithmParameterizer::buildListWidget(size_t listNo)
{
    QVBoxLayout *pageLayout = new QVBoxLayout(this);

    auto it = m_parameters[listNo].begin();
    auto itEnd = m_parameters[listNo].end();
    for ( ; it != itEnd; ++it)
    {
        sd::libqt::SDRParameter *p = *it;
        std::string pName = p->getName();

        WidgetList &wList = m_widgets[listNo];
        if (wList[pName])
            delete wList[pName];
        wList[pName] = buildEditor(p, this);
        pageLayout->addLayout(wList[pName]->layout());
    }

    QWidget *page = new QWidget;
    page->setLayout(pageLayout);

    return page;
    //return nullptr;
}

/*std::vector<ParameterList>
AlgorithmParameterizer::getModifiedParameters()
{
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (!m_parameters[i].empty())
        {
            for (auto itr = m_parameters[i].begin(); itr != m_parameters[i].end(); ++itr)
            {
                std::string pName = itr->name();
                m_widgets[i][pName]->updateParameter();
            }
        }
    }

    return m_parameters;
}*/

std::map<std::string, std::string>
AlgorithmParameterizer::getModifiedXMLParameters()
{
    std::map<std::string, std::string> mapXml;
    sd::libqt::QtXMLValueGenerator valueGenerator;
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (!m_parameters[i].empty())
        {
            for (auto itr = m_parameters[i].begin(); itr != m_parameters[i].end(); ++itr)
            {
                std::string pName = (*itr)->getName();
                sd::libqt::SDRParameter *tmp_param = m_widgets[i][pName]->updateXMLParameter();

                mapXml[pName] = valueGenerator.simpleXMLValue(tmp_param);
            }
        }
    }

    return mapXml;
}


}

}
