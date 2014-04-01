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

#include <core/image/ImageView.hpp>
#include <core/image/ImageView_.hpp>
#include <frontend/libqt/QtParametrizationWidget.hpp>
#include <frontend/lib/ParametrizationWidgetFactory.hpp>
#include <frontend/lib/ParametrizationWidgetFactoryManager.hpp>

#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <PluginDefs.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QLabel>
#include <QHBoxLayout>
#include <QDate>
#include <QDateTimeEdit>
RESTORE_CONTEXT()

#include <string>

class QtWidgetExample : public sd::libqt::QtParametrizationWidget
{

public:

    QtWidgetExample(sd::libqt::SDRParameter* p) : sd::libqt::QtParametrizationWidget(p) {}

    virtual ~QtWidgetExample() {}

    virtual void
    build(QWidget *parameterizer)
    {
        // build appropriate widget
        sd::libqt::SimpleStringParameter *date_p = static_cast<sd::libqt::SimpleStringParameter *>(m_p);
        m_widget = new QDateTimeEdit(QDate::fromString(date_p->getDefault().c_str()));
        QObject::connect(m_widget, SIGNAL(dateChanged(const QDate &)),
                         parameterizer, SIGNAL(parametersChanged()));

        // build the widget layout
        m_layout = new QHBoxLayout;
        m_layout->addWidget(new QLabel(date_p->getName().c_str()));
        m_layout->addWidget(m_widget);
    }

    virtual void
    updateParameter()
    {
        // here insert code to modify m_p using m_widget "value"
        sd::libqt::SimpleStringParameter *date_p = static_cast<sd::libqt::SimpleStringParameter *>(m_p);
        date_p->setValue(((QDateTimeEdit *) m_widget)->date().toString().toStdString());
        //std::cout << "Modified parameter:" << m_p << std::endl;
        std::cout << "Modified parameter:" << std::endl;
        m_p->print();
    }

};

class QtWidgetExampleFactory : public sd::frontend::ParametrizationWidgetFactory
{

public:

    QtWidgetExampleFactory()
        : sd::frontend::ParametrizationWidgetFactory() {}

    virtual ~QtWidgetExampleFactory() {}

    virtual const std::string &
    name() const
    {
        return m_name;
    }

    virtual QtWidgetExample *
    createWidget(sd::libqt::SDRParameter* p) const
    {
        return new QtWidgetExample(p);
    }

private:

    static const std::string m_name;

};

const std::string QtWidgetExampleFactory::m_name = "QtWidgetExamplePlugin";

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
    sd::frontend::registerWidgetFactory(new QtWidgetExampleFactory);
}
