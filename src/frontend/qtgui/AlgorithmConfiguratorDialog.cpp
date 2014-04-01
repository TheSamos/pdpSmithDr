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

#include "AlgorithmConfiguratorDialog.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QHBoxLayout>
RESTORE_CONTEXT()

#include <core/image/ImageView.hpp>
#include <core/mesh/MeshView.hpp>
#include <frontend/lib/Any.hpp>
#include <frontend/lib/TypesInfo.hpp>
#include <frontend/lib/ViewManager.hpp>
#include "GraphicViewsManager.hpp"
#include "MainWindow.hpp"
#include "GraphicView/ImageSceneState.hpp"
#include "GraphicView/ImageScene.hpp"

namespace sd
{

namespace qtgui
{

AlgorithmConfiguratorDialog::AlgorithmConfiguratorDialog()
    : QDialog(&MainWindow::instance()), m_parameterizer(NULL)
{

    m_mainLayout = new QVBoxLayout;
    setLayout(m_mainLayout);

    m_inputLayout = new QVBoxLayout;
    m_mainLayout->addLayout(m_inputLayout);

    m_parametersLayout = new QVBoxLayout;
    m_mainLayout->addLayout(m_parametersLayout);

    m_previewLayout = new QVBoxLayout;
    m_mainLayout->addLayout(m_previewLayout);

    m_graphicViewsLayout = new QHBoxLayout;
    QVBoxLayout *pvwLayout = new QVBoxLayout;
    pvwLayout->addLayout(m_graphicViewsLayout);
    m_previewCB = new QCheckBox(tr("Preview"));
    connect(m_previewCB, SIGNAL(stateChanged(int)), this, SIGNAL(previewOnOff()));
    pvwLayout->addWidget(m_previewCB);
    m_previewCB->setVisible(false);
    m_previewLayout->addLayout(pvwLayout);

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    cancelButton->setDefault(false);
    cancelButton->setAutoDefault(false);
    QPushButton *okButton = new QPushButton("Ok");
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    okButton->setDefault(false);
    okButton->setAutoDefault(false);
    QDialogButtonBox *okCancelBox = new QDialogButtonBox(Qt::Horizontal);
    okCancelBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    okCancelBox->addButton(okButton, QDialogButtonBox::ActionRole);

    m_mainLayout->addWidget(okCancelBox);
}

AlgorithmConfiguratorDialog::~AlgorithmConfiguratorDialog()
{
    removePreviews();
}

void
AlgorithmConfiguratorDialog::buildPreviews(const frontend::AlgorithmInfo &info, int nb)
{
    QLayoutItem *child = NULL;
    while ((child = m_graphicViewsLayout->takeAt(0)) != 0)
    {
        delete child;
    }

    std::vector<sd::frontend::Any> types = info.get();
    size_t n = std::max(nb, (int) types.size());
    if (n <= 0)
        return;

    removePreviews();
    m_previews.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
        if (types[i].isA<core::ImageView *>()
                || (types[i].isA<sd::core::ObjectDescription>()
                    && types[i].isA(sd::core::ObjectDescription::ImageView())))
        {
            // :TODO: we may want to change this to have 3D previews
            //m_previews[i] = GraphicViewsManager::instance().createGraphicView(GraphicViewsManager::GraphicView2D);
            m_previews[i] = GraphicViewsManager::instance().createImageGraphicView(graphicView::ImageSceneState::State2dType);
            ((graphicView::ImageScene *) m_previews[i]->getScene())->setOrientation(Axial);
            m_previews[i]->setName("");
            //m_previews[i]->getImageScene().setOrientation(Z_AXIS);
            // :TODO: algorithm (graphical) info
            /*
              m_previews[i]->getImageScene().setPicking(true);
              connect(&m_previews[i]->getImageScene(), SIGNAL(picked(float,float,float)),
              this, SLOT(pointPicked(float,float,float)));
            */
        }
        else if (types[i].isA<core::MeshView *>()
                 || (types[i].isA<sd::core::ObjectDescription>()
                     && types[i].isA(sd::core::ObjectDescription::MeshView())))
        {
            m_previews[i] = GraphicViewsManager::instance().createMeshGraphicView(graphicView::MeshSceneState::FACE);
            m_previews[i]->setName("");
        }

        m_graphicViewsLayout->addWidget(m_previews[i]);
    }

    m_previewCB->setVisible(true);
}

void
AlgorithmConfiguratorDialog::removePreviews()
{
    std::cout << "AlgorithmConfiguratorDialog::removePreviews()\n";
    for (size_t i = 0; i < m_previews.size(); ++i)
    {
        GraphicViewsManager::instance().remove(m_previews[i]);
        delete m_previews[i];
    }
    m_previews.clear();
}

/**
   Get a subset of algorithms from @a algos
   which accept @a selectedViews as first parameters.

   For example, if we have
   algo1 that needs V1,V2,V3,V4
   algo2 that needs V3,V1,V2
   If selectedViews contains V1,V2, it will return only algo1.
*/
std::vector<frontend::Algorithm *>
AlgorithmConfiguratorDialog::getAlgorithmsAcceptViewsFirst(const std::vector<frontend::Algorithm *> &algos,
        const std::vector<frontend::ViewInfoPtr> &selectedViews) const
{
    std::vector<frontend::Algorithm *> result;
    sd::core::DynamicDimensionalCoordinates<uchar> check(0);
    std::vector<sd::frontend::Algorithm *>::const_iterator it = algos.begin();
    const std::vector<sd::frontend::Algorithm *>::const_iterator end = algos.end();
    for ( ; it != end; ++it)
    {
        check = sd::core::DynamicDimensionalCoordinates<uchar>(0);
        const std::vector<sd::frontend::Any> &inputs = (*it)->input().get();
        for (size_t i = 0; i < inputs.size() && i < selectedViews.size(); ++i)
        {
            const frontend::Any &in = inputs[i];
            frontend::ViewInfoPtr view = selectedViews[i];

            if (in.isA<sd::core::ObjectDescription>())
            {
                if (view->getViewType() == frontend::ViewInfo::ImageType)
                {
                    sd::core::ImageView *iv = ((frontend::ImageInfo *) & (*view))->imagePtr.get();
                    sd::frontend::Any tmp((const sd::core::ImageView *) iv);
                    const sd::core::ObjectDescription *ptr = (in.toPtr());
                    if (tmp.isA(*ptr))
                        check.setAt(i, 1);
                }
                else if (view->getViewType() == frontend::ViewInfo::MeshType)
                {
                    sd::core::MeshView *mv = ((frontend::MeshInfo *) & (*view))->meshPtr.get();
                    sd::frontend::Any tmp((const sd::core::MeshView *) mv);
                    const sd::core::ObjectDescription *ptr = (in.toPtr());
                    if (tmp.isA(*ptr))
                        check.setAt(i, 1);
                }
            }
            else if (in.isA<core::ImageView *>())
                check.setAt(i, (view->getViewType() == frontend::ViewInfo::ImageType) ? 1 : 0);
            else if (in.isA<core::MeshView *>())
                check.setAt(i, (view->getViewType() == frontend::ViewInfo::MeshType) ? 1 : 0);
            else
            {
#ifdef DEBUG
                std::cerr << "ERROR : Undefined Type" << std::endl;
#endif
            }
        }
        bool add = check.nbDims() == selectedViews.size();
        for (unsigned int j = 0; j < check.nbDims(); ++j)
            add &= (check.getAt(j) == 1);
        if (add)
            result.push_back(*it);
    }
    return result;
}

bool isMuch(const QComboBox *cb, const std::vector<std::string> &atts)
{
    if (cb->count() != (int)atts.size())
        return false;
    for (unsigned int i = 0; i < atts.size(); ++i)
    {
        if (cb->itemText(i).toStdString() != atts[i])
            return false;
    }
    return true;
}

void
AlgorithmConfiguratorDialog::updateInputs(std::vector<frontend::Algorithm *> algos)
{
    frontend::ViewManager::ViewsList views = frontend::ViewManager::instance().getViews();
    std::vector<frontend::ViewInfoPtr> selectedViews;

    auto isImage = [](const frontend::ViewInfoPtr & v)
    {
        return (v->getViewType() == frontend::ViewInfo::ImageType);
    };

    auto isMesh = [](const frontend::ViewInfoPtr & v)
    {
        return (v->getViewType() == frontend::ViewInfo::MeshType);
    };

    for (unsigned int i = 0; i < m_inputs.size(); ++i)
    {
        QComboBox *comboBox = m_inputs[i].first;
        assert(comboBox);
        disconnect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setView(QString)));
        QString currentText = comboBox->currentText();
        int index = -1;
        comboBox->clear();
        frontend::ViewManager::ViewsList::const_iterator it = views.begin();
        const frontend::ViewManager::ViewsList::const_iterator end = views.end();
        for (; it != end; ++it)
        {
            for (unsigned int algoIdx = 0; algoIdx < algos.size(); ++algoIdx)
            {
                assert(algos[algoIdx]->input().get().size() > (unsigned int)i);
                bool added = false;
                const frontend::Any &in = algos[algoIdx]->input().get()[i];
                if (in.isA<sd::core::ObjectDescription>())
                {
                    if (((*it).second)->getViewType() == frontend::ViewInfo::ImageType)
                    {
                        sd::core::ImageView *iv = ((frontend::ImageInfo *) ((*it).second))->imagePtr.get();
                        sd::frontend::Any tmp((const sd::core::ImageView *) iv);
                        const sd::core::ObjectDescription *ptr = in.toPtr();
                        if (tmp.isA(*ptr))
                        {
                            comboBox->addItem(QString::fromStdString(it->first));
                            added = true;
                        }
                    }
                    else if (((*it).second)->getViewType() == frontend::ViewInfo::MeshType)
                    {
                        sd::core::MeshView *mv = ((frontend::MeshInfo *) ((*it).second))->meshPtr.get();
                        sd::frontend::Any tmp((const sd::core::MeshView *) mv);
                        const sd::core::ObjectDescription *ptr = in.toPtr();
                        if (tmp.isA(*ptr))
                        {
                            comboBox->addItem(QString::fromStdString(it->first));
                            added = true;
                        }
                    }
                }
                else if ((in.isA<core::ImageView *>() && isImage(it->second))
                         || (in.isA<core::MeshView *>() && isMesh(it->second)))
                {
                    comboBox->addItem(QString::fromStdString(it->first));
                    added = true;
                }
                if (added
                        && selectedViews.size() == (unsigned int) i
                        && currentText == QString::fromStdString(it->first))
                {
                    index = comboBox->count() - 1;
                    *m_views[comboBox] = views[currentText.toStdString()];
                    selectedViews.push_back((views.find(currentText.toStdString()))->second);
                }
                if (added && currentText == QString::fromStdString(it->first))
                {
                    const sd::core::Object *obj = ((frontend::ImageInfo *) ((*it).second))->imagePtr.get();
                    const frontend::Any &in = algos[algoIdx]->input().get()[i];
                    if (in.isA<sd::core::ObjectDescription>())
                    {
                        const sd::core::ObjectDescription *od = in.toPtr();
                        assert(m_inputs[i].second.size() == od->getOptionalKeysNb());
                        std::vector<std::vector<std::string> > atts;
                        od->getOptionalKeys(*obj, atts);
                        for (unsigned int j = 0; j < m_inputs[i].second.size(); ++j)
                        {
                            QComboBox *comboBox = m_inputs[i].second[j];
                            const std::vector<std::string> &attList = atts[j];
                            QString currentAtt = comboBox->currentText();
                            if (!isMuch(comboBox, attList))
                            {
                                disconnect(comboBox);
                                comboBox->clear();
                                for (unsigned int k = 0; k < attList.size(); ++k)
                                    comboBox->addItem(QString::fromStdString(attList[k]));
                                comboBox->setCurrentIndex(std::max(comboBox->findText(currentAtt), 0));
                                connect(comboBox, SIGNAL(currentIndexChanged(int)),
                                        this, SIGNAL(parametersChanged()));

                            }
                        }
                    }
                }
            }
        }

        // The last is invalid
        if (index == -1)
            selectedViews.resize(std::min<unsigned int>(selectedViews.size(), i));
        //else
        //*m_views[comboBox] = views[currentText.toStdString()];

        if (selectedViews.size() <= (unsigned int) i + 1)
        {
            algos = getAlgorithmsAcceptViewsFirst(algos, selectedViews);
            if (selectedViews.size() == 0) algos.clear();
            m_currentAvailableAlgos = algos;
        }
        comboBox->setCurrentIndex(index);

        connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setView(QString)));
    }
    // Init all values...
    if (selectedViews.size() != m_inputs.size()/*(size_t)m_inputLayout->count()*/)
    {
        ((QComboBox *) (m_inputLayout->itemAt(selectedViews.size())->widget()))->setCurrentIndex(0);
    }
    // m_currentAvailableAlgos = algos;
}

bool
AlgorithmConfiguratorDialog::isPreviewActive() const
{
    return m_previewCB->isChecked();
}

bool
AlgorithmConfiguratorDialog::addInputs(const std::vector<frontend::Algorithm *> &algos,
                                       std::vector<frontend::ViewInfoPtr> &inputViews)
{
    const frontend::ViewManager::ViewsList &views = frontend::ViewManager::instance().getViews();
    if (views.empty() || algos.empty())
        return false;

    inputViews.resize(algos.front()->input().get().size());

    for (unsigned int i = 0; i < inputViews.size(); ++i)
    {
        QComboBox *comboBox = new QComboBox;
        m_inputLayout->addWidget(comboBox);

        std::vector<QComboBox *> atts;
        const frontend::Any &in = algos.front()->input().get()[0];
        if (in.isA<sd::core::ObjectDescription>())
        {
            std::vector<std::string> names = ((const sd::core::ObjectDescription *)in.toPtr())->getOptionalKeysNames();
            for (unsigned int j = 0; j < names.size(); ++j)
            {
                QComboBox *attComboBox = new QComboBox;
                atts.push_back(attComboBox);
                QWidget *w = new QWidget();
                QHBoxLayout *l = new QHBoxLayout();
                l->addWidget(new QLabel(QString::fromUtf8(names[j].c_str()) + " : "));
                l->addWidget(attComboBox);
                w->setLayout(l);
                m_inputLayout->addWidget(w);
            }
        }
        m_inputs.push_back(std::pair<QComboBox *, std::vector<QComboBox *> >(comboBox, atts));
        // add callbacks
        m_views[comboBox] = &inputViews[i];
        //comboBox->setCurrentIndex(-1);
        connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setView(QString)));
        //comboBox->setCurrentIndex(0);
    }
    return true;
}

/*void
AlgorithmConfiguratorDialog::addParameters(const frontend::ParameterList &p)
{
    if (!p.empty() && !m_parameterizer)
    {
        std::vector<frontend::ParameterList> params;
        params.push_back(p);
        m_parameterizer = new AlgorithmParameterizer(params, this);
        m_parametersLayout->addWidget(m_parameterizer);
        connect(m_parameterizer, SIGNAL(parametersChanged()),
                this, SIGNAL(parametersChanged()));
    }
}*/

void 
AlgorithmConfiguratorDialog::addParameters(const std::string& xml)
{
    if (!m_parameterizer)
    {
        m_parameterizer = new AlgorithmParameterizer(xml, this);
        m_parametersLayout->addWidget(m_parameterizer);
        connect(m_parameterizer, SIGNAL(parametersChanged()),
                this, SIGNAL(parametersChanged()));
    }
}

/*std::vector<frontend::ParameterList>
AlgorithmConfiguratorDialog::getModifiedParameters()
{
    std::vector<frontend::ParameterList> result(1);
    if (m_parameterizer)
        result = m_parameterizer->getModifiedParameters();
    frontend::ParameterList &pl = result[0];
    for (unsigned int i = 0; i < m_inputs.size(); ++i)
        for (unsigned int j = 0; j < m_inputs[i].second.size(); ++j)
        {
            pl.push_back(frontend::Parameter(std::string("akey:") + i + "-" + j,
                                             m_inputs[i].second[j]->currentText().toStdString()));
        }
    //result.push_back(pl);
    return result;
}*/

std::map<std::string, std::string>
AlgorithmConfiguratorDialog::getModifiedXMLParameters()
{
    /*std::vector<frontend::ParameterList> result(1);
    if (m_parameterizer)
        result = m_parameterizer->getModifiedParameters();
    frontend::ParameterList &pl = result[0];
    for (unsigned int i = 0; i < m_inputs.size(); ++i)
        for (unsigned int j = 0; j < m_inputs[i].second.size(); ++j)
        {
            pl.push_back(frontend::Parameter(std::string("akey:") + i + "-" + j,
                                             m_inputs[i].second[j]->currentText().toStdString()));
        }*/
    //result.push_back(pl);
    std::cout << "in the getModifiedXMLParameters" << std::endl;
    return m_parameterizer->getModifiedXMLParameters();
}

void
AlgorithmConfiguratorDialog::updatePreviews(const std::vector<core::ViewNode *> &nodes)
{
    if (nodes.empty() || !m_previewCB->isChecked())
        return;

    assert(nodes.size() == m_previews.size());

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // :TODO: reduce number of memory allocations for Info objects
        frontend::ViewInfoPtr info = frontend::ViewManager::instance().createViewInfo(nodes[i], "", false);
        m_previews[i]->setView(info.get());
    }
}

void
AlgorithmConfiguratorDialog::setView(QString name)
{
    auto views = frontend::ViewManager::instance().getViews();
    *m_views[sender()] = views[name.toStdString()];
    viewSet(m_views[sender()]);
}

}

}
