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
#include "LoaderAction.hpp"

#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/Algorithm.hpp>
#include <frontend/lib/Loader.hpp>
#include <frontend/lib/Any.hpp>

namespace sd
{
namespace qtgui
{

LoaderAction::LoaderAction(const std::string &actionName,
                           QObject *p)
    : QAction(QString::fromStdString(actionName), p),
      m_name(actionName),
      m_dialog(NULL),
      m_prototypes(),
      m_firstCall(true)
{
}

LoaderAction::~LoaderAction()
{
    if (m_dialog)
        delete m_dialog;
}

void
LoaderAction::configure()
{
    if (!m_prototypes.empty())
    {
        m_firstCall = false;
        m_dialog = new AlgorithmConfiguratorDialog;

        connect(m_dialog, SIGNAL(viewSet(frontend::ViewInfoPtr *)),
                this, SLOT(inputChanged()));
        connect(m_dialog, SIGNAL(parametersChanged()), this, SLOT(parametersChanged()));
        connect(m_dialog, SIGNAL(previewOnOff()), this, SLOT(previewOnOff()));

        // add a selector for each parameter (int, float...)
        //m_dialog->addParameters(m_prototypes.first()->expectedParameters());
    }
}

namespace
{

void
configurationError(const QString &msg)
{
    QMessageBox::critical(&MainWindow::instance(),
                          QObject::tr("Loader configuration error"),
                          msg);
}

}

void
LoaderAction::inputChanged()
{
    if (m_dialog->isPreviewActive())
        process(true);
}

void
LoaderAction::parametersChanged()
{
    if (m_dialog->isPreviewActive())
        process(true);
}

void
LoaderAction::runLoader()
{
    if (m_name.empty())
        return;

    m_prototypes = frontend::queryLoader(m_name);
    if (m_prototypes.empty())
    {
        configurationError(tr("Loader not found: %1").arg(m_name.c_str()));
        return;
    }
    // m_prototypes can contain several prototypes of the same loader:
    // there is one prototype per accepted data type (int, float...)

    auto input = m_prototypes.first()->input().get();

    auto views = frontend::ViewManager::instance().getViews();
    if (!input.empty() && views.empty())
    {
        configurationError(tr("No existing view to select"));
        return;
    }

    if (m_firstCall)
        configure();

    const frontend::Loader *loader = m_prototypes.first();
    m_dialog->buildPreviews(loader->output());

    if (m_dialog->isPreviewActive())
        process(true);

    int status = m_dialog->exec();
    if (status == QDialog::Rejected)
        return;

    process(false);
}

void
LoaderAction::process(bool preview)
{
    if (!m_dialog)
        return;

    // get modifications made by the user
    //auto parameters = m_dialog->getModifiedParameters();

    frontend::Loader *loader = m_prototypes.create();
    if (!loader)
    {
        configurationError(tr("Unable to find loader"));
        return;
    }
    //((frontend::Algorithm *) loader)->setParams(parameters);
    bool successful = loader->run();
    if (!successful)
    {
        configurationError(tr("Unable to run loader"));
        return;
    }

    if (preview)
    {
        m_dialog->updatePreviews(loader->getOutputs());
    }
    else
    {
        // add outputs to view graph
        auto output = loader->getOutputs();
        for (auto it = output.begin(); it != output.end(); ++it)
        {
            std::string err;
            bool ok = frontend::ViewManager::instance().createView(*it, err);
            if (!ok)
            {
                configurationError(tr("Unable to get loader outputs"));
                return;
            }
        }
    }
}

void
LoaderAction::previewOnOff()
{
    if (m_dialog->isPreviewActive())
        process(true);
}

}
}
