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
#include <PluginDefs.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
RESTORE_CONTEXT()

#include <string>

#include "OpenFileButtonPlugin.hpp"

namespace sd
{
namespace plugins
{
namespace widgets
{
namespace qt
{

OpenFileDialog::OpenFileDialog(const QString &filename, QWidget *p)
    : QWidget(), m_button(0), m_parameterizer(p)
{
    m_button = new QPushButton(filename);
    QObject::connect(m_button, SIGNAL(clicked()), this, SLOT(selectFile()));

    QHBoxLayout *widgetLayout = new QHBoxLayout;
    widgetLayout->addWidget(m_button);
    setLayout(widgetLayout);
}

static QString currentPath;

void
OpenFileDialog::selectFile()
{
    currentPath = QFileDialog::getOpenFileName(NULL,
                  QObject::tr("Open file"),
                  currentPath, "*");

    m_button->setText(currentPath);
    update();
}

std::string
OpenFileDialog::getFilename() const
{
    return m_button->text().toStdString();
}

}
}
}
}

class OpenFileButton : public sd::libqt::QtParametrizationWidget
{

public:

    OpenFileButton(sd::libqt::SDRParameter* p) : sd::libqt::QtParametrizationWidget(p) {}

    virtual ~OpenFileButton() {}

    virtual void
    build(QWidget *parameterizer)
    {
        //std::string filename = m_p.getString();
        /*m_widget = new sd::plugins::widgets::qt::OpenFileDialog(filename.c_str(), parameterizer);
        QObject::connect(m_widget, SIGNAL(update()), parameterizer, SIGNAL(parametersChanged()));

        // build the widget layout
        m_layout = new QHBoxLayout;
        m_layout->addWidget(new QLabel(m_p.name().c_str()));
        m_layout->addWidget(m_widget);*/
    }

    virtual void
    updateParameter()
    {
        //m_p = ((sd::plugins::widgets::qt::OpenFileDialog *) m_widget)->getFilename();
    }

};

class OpenFileButtonFactory : public sd::frontend::ParametrizationWidgetFactory
{

public:

    OpenFileButtonFactory() : sd::frontend::ParametrizationWidgetFactory() {}

    virtual ~OpenFileButtonFactory() {}

    virtual const std::string &
    name() const
    {
        return m_name;
    }

    virtual OpenFileButton *
    createWidget(sd::libqt::SDRParameter* p) const
    {
        return new OpenFileButton(p);
    }

private:

    static const std::string m_name;

};

const std::string OpenFileButtonFactory::m_name = "OpenFileButtonPlugin";

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
    sd::frontend::registerWidgetFactory(new OpenFileButtonFactory);
}
