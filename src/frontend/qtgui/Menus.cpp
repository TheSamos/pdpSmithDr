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

#include "Menus.hpp"

#include <functional>
#include <sstream>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidgetAction>
#include <QToolButton>
RESTORE_CONTEXT()

#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>
#include <frontend/lib/ViewManager.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/Algorithm.hpp>

#include "GraphicView/Widget.hpp"
#include "GraphicView/BlockWidget.hpp"
#include "GraphicView/ImageScene.hpp"
#include "GraphicView/ViewingWindowManager.hpp"
#include "GraphicView/SliceWindowManager.hpp"

#include "AlgorithmAction.hpp"
#include "LoaderAction.hpp"
#include "MainWindow.hpp"
#include "ViewConfigDialog.hpp"
#include "GraphicViewsManager.hpp"

namespace sd
{
namespace qtgui
{

Menus::Menus()
    : m_fileMenu(0),
      m_algorithmSelectionMenu(0),
      m_blocksMenu(0),
      m_graphicViewsMenu(0),
      m_viewsMenu(0)
{
    populate();

    viewCreationCallbackHandle = frontend::ViewManager::instance().registerViewCreationCallback(std::bind(&Menus::viewCreationCallback, this, std::placeholders::_1));
    viewDeletionCallbackHandle = frontend::ViewManager::instance().registerViewDeletionCallback(std::bind(&Menus::viewDeletionCallback, this, std::placeholders::_1));
    viewModificationCallbackHandle = frontend::ViewManager::instance().registerViewModificationCallback(std::bind(&Menus::viewModificationCallback, this, std::placeholders::_1));

    connect(this, SIGNAL(enqueueViewCreationSlot(frontend::ViewInfo *)),
            this, SLOT(viewCreationSlot(frontend::ViewInfo *)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(enqueueViewDeletionSlot(frontend::ViewInfo *)),
            this, SLOT(viewDeletionSlot(frontend::ViewInfo *)),
            Qt::QueuedConnection);
}

Menus::~Menus()
{
    // Avoid unwanted behavior dependant on static object initialization order
    frontend::ViewManager::instance().unregisterViewCreationCallback(viewCreationCallbackHandle);
    frontend::ViewManager::instance().unregisterViewDeletionCallback(viewDeletionCallbackHandle);
    frontend::ViewManager::instance().unregisterViewModificationCallback(viewModificationCallbackHandle);
}

void
Menus::quit()
{
    MainWindow::instance().close();
}

QMenu *
Menus::getFileMenu()
{
    if (!m_fileMenu)
    {
        m_fileMenu = new QMenu(QObject::tr("&File"));
        m_fileMenu->addAction(QObject::tr("Open file"), this, SLOT(openFile()), QKeySequence::Open);

        m_fileMenu->addAction(QObject::tr("Open raw image"), this, SLOT(openRawImage()));

        //m_fileMenu->addAction(QObject::tr("Save as"), this, SLOT(saveView()), QKeySequence::Save); // which view?

        m_fileMenu->addSeparator();
        m_fileMenu->addAction(QObject::tr("Quit"), this, SLOT(quit()), QKeySequence::Quit);

    }
    return m_fileMenu;
}

QMenu *
Menus::getAlgorithmSelectionMenu()
{
    if (!m_algorithmSelectionMenu)
        m_algorithmSelectionMenu = new QMenu(QObject::tr("&Algorithm"));
    return m_algorithmSelectionMenu;
}

QMenu *
Menus::getAlgorithmSelectionSubMenu(std::string path)
{
    QMenu *currentMenu = getAlgorithmSelectionMenu();

    std::string subMenuPath = "";
    size_t pos = path.find_first_of(':');
    while (pos != std::string::npos)
    {
        std::string subMenuName = path.substr(0, pos);
        if (!subMenuPath.empty())
            subMenuPath += ":";
        subMenuPath += subMenuName;
        path = path.substr(pos + 1);
        pos = path.find_first_of(':');

        SubMenus::iterator it = m_algorithmSubMenus.find(subMenuPath);
        if (it == m_algorithmSubMenus.end())
        {
            QMenu *subMenu = new QMenu(QObject::tr(subMenuName.c_str()), currentMenu);
            currentMenu->addMenu(subMenu);
            m_algorithmSubMenus[subMenuPath] = subMenu;
        }
        currentMenu = m_algorithmSubMenus[subMenuPath];
    }

    return currentMenu;
}

QMenu *
Menus::getViewsMenu()
{
    if (!m_viewsMenu)
        m_viewsMenu = new QMenu(QObject::tr("&Views"));
    return m_viewsMenu;
}

QMenu *
Menus::getViewMenu(const frontend::ViewInfo *view)
{
    auto it = m_viewToMenu.find(const_cast<frontend::ViewInfo *>(view));
    assert(it != m_viewToMenu.end() && "unable to find view");
    return it->second;
}

void
Menus::buildLayoutButton(const QString &name,
                         graphicView::BlockWidget::LayoutType type,
                         QGridLayout *layout, int row, int col,
                         bool pressed)
{
    QToolButton *button = new QToolButton;
    m_layoutButtonGroup->addButton(button);
    button->setCheckable(true);
    button->setStyleSheet("border: 0; background: none;");
    button->setIcon(QIcon(QString(SD_ICON_PATH) + "/layouts/" + name + ".svg"));
    layout->addWidget(button, row, col);
    button->setProperty("SdLayoutType", type);
    if (pressed)
    {
        button->setStyleSheet("border: 0; background: lightgray;");
        button->setChecked(pressed);
    }
    connect(button, SIGNAL(released()), this, SLOT(layoutActivated()));
    connect(button, SIGNAL(toggled(bool)), this, SLOT(layoutChanged(bool)));
}

void
Menus::changeLayout(graphicView::BlockWidget::LayoutType type)
{
    QList<QAbstractButton *> buttons = m_layoutButtonGroup->buttons();
    for (auto it = buttons.begin(); it != buttons.end(); ++it)
    {
        graphicView::BlockWidget::LayoutType buttonType = (graphicView::BlockWidget::LayoutType) (*it)->property("SdLayoutType").toInt();
        if (buttonType == type)
        {
            (*it)->toggle();
        }
    }
}

void
Menus::layoutChanged(bool)
{
    QToolButton *button = ((QToolButton *) sender());
    if (button->isChecked())
        button->setStyleSheet("border: 0; background: lightgray;");
    else
        button->setStyleSheet("border: 0; background: none;");
}

void
Menus::layoutActivated()
{
    QToolButton *button = ((QToolButton *) sender());
    graphicView::BlockWidget::LayoutType type = (graphicView::BlockWidget::LayoutType) button->property("SdLayoutType").toInt();
    auto block = MainWindow::instance().currentBlock();
    if (block)
        block->changeLayout(type);
}

QMenu *
Menus::getBlocksMenu()
{
    if (!m_blocksMenu)
    {
        m_blocksMenu = new QMenu(QObject::tr("&Block"));
        QMenu *configMenu = new QMenu("Select layout");
        m_blocksMenu->addMenu(configMenu);

        m_layoutButtonGroup = new QButtonGroup;

        QGridLayout *layout = new QGridLayout;
        layout->setSpacing(2);
        buildLayoutButton("layout1", graphicView::BlockWidget::BlockLayout1, layout, 0, 0);
        buildLayoutButton("layout4", graphicView::BlockWidget::BlockLayout4, layout, 0, 1, true);
        buildLayoutButton("layout2h", graphicView::BlockWidget::BlockLayout2h, layout, 0, 2);
        buildLayoutButton("layout2v", graphicView::BlockWidget::BlockLayout2v, layout, 0, 3);
        buildLayoutButton("layout1-2h", graphicView::BlockWidget::BlockLayout1_2h, layout, 1, 0);
        buildLayoutButton("layout2-1h", graphicView::BlockWidget::BlockLayout2_1h, layout, 1, 1);
        buildLayoutButton("layout1-2v", graphicView::BlockWidget::BlockLayout1_2v, layout, 1, 2);
        buildLayoutButton("layout2-1v", graphicView::BlockWidget::BlockLayout2_1v, layout, 1, 3);

        QWidget *w = new QWidget;
        w->setLayout(layout);
        QWidgetAction *qwa = new QWidgetAction(configMenu);
        qwa->setDefaultWidget(w);
        configMenu->addAction(qwa);

        m_blocksMenu->addSeparator();

        QAction *sshotAction = m_blocksMenu->addAction("Take &screenshot");
        connect(sshotAction, SIGNAL(triggered()), this, SLOT(takeBlockScreenshot()));
    }
    return m_blocksMenu;
}

void
Menus::initGraphicViewMenu(graphicView::BlockWidget *block)
{
    for (auto it = m_graphicViewToMenu.begin(); it != m_graphicViewToMenu.end(); ++it)
    {
        it->second->menuAction()->setVisible(false);
    }

    auto graphicViews = block->getGraphicViews();
    for (auto it = graphicViews.begin(); it != graphicViews.end(); ++it)
    {
        auto menu = getGraphicViewMenu(*it);
        menu->menuAction()->setVisible(true);
    }
}

QMenu *
Menus::getGraphicViewsMenu()
{
    if (!m_graphicViewsMenu)
    {
        m_graphicViewsMenu = new QMenu(QObject::tr("&GraphicViews"));
    }
    return m_graphicViewsMenu;
}

QMenu *
Menus::getGraphicViewMenu(const graphicView::Widget *graphicView)
{
    auto it = m_graphicViewToMenu.find(const_cast<graphicView::Widget *>(graphicView));
    assert(it != m_graphicViewToMenu.end() && "unable to find graphicView");
    return it->second;
}

void
Menus::graphicViewCreated(graphicView::Widget *graphicView)
{
    //graphicView::Scene* scene = graphicView->getScene();
    QMenu *graphicViewMenu = new QMenu(graphicView->getName(), getGraphicViewsMenu());
    getGraphicViewsMenu()->addMenu(graphicViewMenu);
    m_graphicViewToMenu[graphicView] = graphicViewMenu;

    //= View Window stuff =//
    /*
    m_graphicViewToViewWindowMenu[scene] = graphicViewMenu->addMenu("View Window");
    m_graphicViewToViewWindowGroup[scene] = new QActionGroup(m_graphicViewToViewWindowMenu[scene]);

    for (auto it = m_viewWindows.begin(), end = m_viewWindows.end(); it != end; ++it) {
    QAction* action = m_graphicViewToViewWindowMenu[scene]->addAction((*it)->getName());
    action->setCheckable(true);
    if (*it == graphicView->getScene()->getViewWindow())
      action->setChecked(true);
    m_actionToGraphicView[action] = graphicView;
    m_actionToViewWindow[action] = *it;
    m_graphicViewToViewWindowGroup[scene]->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(viewWindowSelected()));
         }

         m_graphicViewToViewSepAction[scene] = m_graphicViewToViewWindowMenu[scene]->addSeparator();
         m_graphicViewToViewWindowGroup[scene]->addAction(m_graphicViewToViewSepAction[scene]);

         QAction* action = m_graphicViewToViewWindowMenu[scene]->addAction("New View Window");
         m_graphicViewToViewWindowGroup[scene]->addAction(action);
         m_actionToGraphicView[action] = graphicView;
         action->setCheckable(false);
         connect(action, SIGNAL(triggered()), this, SLOT(createAndAssignViewWindow()));

         connect(scene, SIGNAL(viewWindowSet(graphicView::ViewingWindow*)),
          this, SLOT(viewWindowAssigned(graphicView::ViewingWindow*)));
         */

    //= Slice Window stuff =//
    /*
    if (graphicView->getScene()->getType() == graphicView::Scene::ImageSceneType) {
    m_graphicViewToSliceWindowMenu[scene] = graphicViewMenu->addMenu("Slice Window");
    m_graphicViewToSliceWindowGroup[scene] = new QActionGroup(m_graphicViewToSliceWindowMenu[scene]);

    for (auto it = m_sliceWindows.begin(), end = m_sliceWindows.end(); it != end; ++it) {
      action = m_graphicViewToSliceWindowMenu[scene]->addAction((*it)->getName());
      action->setCheckable(true);
      if (*it == ((graphicView::ImageScene*) graphicView->getScene())->getSliceWindow())
        action->setChecked(true);
      m_actionToGraphicView[action] = graphicView;
      m_actionToSliceWindow[action] = *it;
      m_graphicViewToSliceWindowGroup[scene]->addAction(action);
      connect(action, SIGNAL(triggered()), this, SLOT(sliceWindowSelected()));
    }

    m_graphicViewToSliceSepAction[scene] = m_graphicViewToSliceWindowMenu[scene]->addSeparator();
    m_graphicViewToSliceWindowGroup[scene]->addAction(m_graphicViewToSliceSepAction[scene]);

    action = m_graphicViewToSliceWindowMenu[scene]->addAction("New Slice Window");
    m_graphicViewToSliceWindowGroup[scene]->addAction(action);
    m_actionToGraphicView[action] = graphicView;
    action->setCheckable(false);
    connect(action, SIGNAL(triggered()), this, SLOT(createAndAssignSliceWindow()));
    connect(scene, SIGNAL(sliceWindowSet(graphicView::SliceWindow*)),
       this, SLOT(sliceWindowAssigned(graphicView::SliceWindow*)));
         }
         */

    //= Other stuff =//
    graphicViewMenu->addSeparator();
    QAction *sshotAction = graphicViewMenu->addAction("Take &screenshot");
    m_actionToGraphicView[sshotAction] = graphicView;
    connect(sshotAction, SIGNAL(triggered()), this, SLOT(takeScreenshot()));
}

void
Menus::graphicViewDeleted(graphicView::Widget *graphicView)
{
    // Require a 2 pass deletion of iterator because of iterator invalidation of std::map::erase
    std::vector<decltype(m_actionToGraphicView.begin())> toDelete;
    for (auto it = m_actionToGraphicView.begin(), end = m_actionToGraphicView.end(); it != end; ++it)
    {
        if (it->second == graphicView)
        {
            delete it->first;
            toDelete.push_back(it);
        }
    }
    for (auto it = toDelete.begin(), end = toDelete.end(); it != end; ++it)
    {
        m_actionToGraphicView.erase(*it);
    }

    delete m_graphicViewToMenu[graphicView];
    m_graphicViewToMenu.erase(graphicView);

    graphicView::Scene *scene = graphicView->getScene();
    m_graphicViewToViewSepAction.erase(scene);
    m_graphicViewToViewWindowMenu.erase(scene);
    m_graphicViewToViewWindowGroup.erase(scene);
    m_graphicViewToSliceSepAction.erase(scene);
    m_graphicViewToSliceWindowMenu.erase(scene);
    m_graphicViewToSliceWindowGroup.erase(scene);
}

void
Menus::graphicViewChanged(graphicView::Widget *graphicView)
{
    auto it = m_graphicViewToMenu.find(graphicView);
    assert(it != m_graphicViewToMenu.end() && "unable to find graphicView");
    it->second->setTitle(graphicView->getName());
}

void
Menus::viewWindowCreated(graphicView::ViewingWindow * /*vw*/)
{
    /*
    for (auto it = m_graphicViewToViewWindowGroup.begin(), end = m_graphicViewToViewWindowGroup.end();
     it != end; ++it) {
    QAction* action = new QAction(vw->getName(), m_graphicViewToViewWindowMenu[it->first]);
    action->setCheckable(true);

    m_actionToGraphicView[action] = &it->first->getWidget();

    m_actionToViewWindow[action] = vw;
    m_graphicViewToViewWindowMenu[it->first]->insertAction(m_graphicViewToViewSepAction[it->first], action);
    it->second->addAction(action);

    connect(action, SIGNAL(triggered()), this, SLOT(viewWindowSelected()));
         }
         m_viewWindows.insert(vw);
         */
}

void
Menus::viewWindowDeleted(graphicView::ViewingWindow * /*vw*/)
{
    /*
    for (auto it1 = m_graphicViewToViewWindowMenu.begin(), end1 = m_graphicViewToViewWindowMenu.end();
     it1 != end1; ++it1) {
    QList<QAction*> actions = m_graphicViewToViewWindowMenu[it1->first]->actions();
    QAction* action = 0;
    for (auto it = actions.begin(), end = actions.end(); it != end; ++it) {
      if ((*it)->text() == vw->getName()) {
        action = *it;
        action->setEnabled(false);
        action->setVisible(false);
        action->deleteLater();
        m_actionToGraphicView.erase(action);
        m_actionToViewWindow.erase(action);
        break;
      }
    }
    m_graphicViewToViewWindowGroup[it1->first]->removeAction(action);
         }
         m_viewWindows.erase(vw);
         */
}

void
Menus::sliceWindowCreated(graphicView::SliceWindow * /*sw*/)
{
    /*
    for (auto it = m_graphicViewToSliceWindowGroup.begin(), end = m_graphicViewToSliceWindowGroup.end(); it != end; ++it) {
    QAction* action = new QAction(sw->getName(), m_graphicViewToSliceWindowMenu[it->first]);
    action->setCheckable(true);

    m_actionToGraphicView[action] = &it->first->getWidget();

    m_actionToSliceWindow[action] = sw;
    m_graphicViewToSliceWindowMenu[it->first]->insertAction(m_graphicViewToSliceSepAction[it->first], action);
    it->second->addAction(action);

    connect(action, SIGNAL(triggered()), this, SLOT(sliceWindowSelected()));
         }
         m_sliceWindows.insert(sw);
         */
}

void
Menus::sliceWindowDeleted(graphicView::SliceWindow * /*sw*/)
{
    /*
    for (auto it1 = m_graphicViewToSliceWindowMenu.begin(), end1 = m_graphicViewToSliceWindowMenu.end();
     it1 != end1; ++it1) {
    QList<QAction*> actions = m_graphicViewToSliceWindowMenu[it1->first]->actions();
    QAction* action = 0;
    for (auto it = actions.begin(), end = actions.end(); it != end; ++it) {
      if ((*it)->text() == sw->getName()) {
        action = *it;
        action->setEnabled(false);
        action->setVisible(false);
        action->deleteLater();
        m_actionToGraphicView.erase(action);
        m_actionToSliceWindow.erase(action);
        break;
      }
    }
    m_graphicViewToSliceWindowGroup[it1->first]->removeAction(action);
         }
         m_sliceWindows.erase(sw);
         */
}

void
Menus::populate()
{
    auto algorithms = frontend::getAlgorithms();
    for (auto it = algorithms.begin(), itEnd = algorithms.end(); it != itEnd; ++it)
    {
        std::string path = (*it)->name();
        size_t pos = path.find_last_of(':');
        std::string name = (pos == std::string::npos ? path : path.substr(pos + 1));

        QMenu *subMenu = getAlgorithmSelectionSubMenu(path);
        std::cout << "Algo: " << name << std::endl;
        AlgorithmAction *action = new AlgorithmAction(name, path, subMenu);

        if (name.size() > 6)
            if (name.substr(name.size() - 6) == std::string("Plugin"))
                action->setXMLParameterized(true);


        subMenu->addAction(action);
        connect(action, SIGNAL(triggered()), action, SLOT(runAlgorithm()));
    }
}

void
Menus::viewCreationCallback(frontend::ViewInfo *view)
{
    enqueueViewCreationSlot(view);
}

void
Menus::viewDeletionCallback(frontend::ViewInfo *view)
{
    enqueueViewDeletionSlot(view);
}

void
Menus::viewModificationCallback(frontend::ViewInfo *view)
{
    m_viewToMenu[view]->setTitle(QString::fromStdString(view->getName()));
}

void
Menus::viewCreationSlot(frontend::ViewInfo *view)
{
    QMenu *viewMenu = getViewsMenu()->addMenu(QString::fromStdString(view->getName()));
    //viewMenu->setParent(getViewsMenu());
    m_viewToMenu[view] = viewMenu;

    //viewMenu->addMenu(getAlgorithmSelectionMenu());
    //viewMenu->addSeparator();

    /*
    QMenu* sendToMenu = viewMenu->addMenu("Send to");
    m_sendToMenuToView[sendToMenu] = view;
    m_viewToSendToMenu[view] = sendToMenu;
    connect(sendToMenu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSendTo()));
    */

    viewMenu->addSeparator();
    QAction *saveAction = viewMenu->addAction("Save as...");
    m_saveActionToView[saveAction] = view;
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveView()));

    viewMenu->addSeparator();
    QAction *configAction = viewMenu->addAction("Configure");
    m_configDeleteActionToView[configAction] = view;
    connect(configAction, SIGNAL(triggered()), this, SLOT(configureView()));
    QAction *deleteAction = viewMenu->addAction("Deletable");
    deleteAction->setCheckable(true);
    deleteAction->setChecked(false);
    m_configDeleteActionToView[deleteAction] = view;
    connect(deleteAction, SIGNAL(toggled(bool)), this, SLOT(setDeletableView(bool)));

    auto block = GraphicViewsManager::instance().createBlockWidget(view);
    MainWindow::instance().addBlock(block);

    initGraphicViewMenu(MainWindow::instance().currentBlock());
}

void
Menus::viewDeletionSlot(frontend::ViewInfo *view)
{
    delete m_viewToMenu[view];
    m_viewToMenu.erase(view);
    /*
    QMenu* sendToMenu = m_viewToSendToMenu[view];
    m_viewToSendToMenu.erase(view);
    m_sendToMenuToView.erase(sendToMenu);
    */

    // Require a 2 pass deletion of iterator because of iterator invalidation of std::map::erase
    std::vector<decltype(m_configDeleteActionToView.begin())> toDelete;
    for (auto it = m_configDeleteActionToView.begin(), end = m_configDeleteActionToView.end(); it != end; ++it)
    {
        if (it->second == view)
            toDelete.push_back(it);
    }
    for (auto it = toDelete.begin(), end = toDelete.end(); it != end; ++it)
    {
        m_configDeleteActionToView.erase(*it);
    }

    for (auto it = m_saveActionToView.begin(), end = m_saveActionToView.end(); it != end; ++it)
    {
        if (it->second == view)
        {
            m_saveActionToView.erase(it);
            break;
        }
    }
}

void
Menus::aboutToShowSendTo()
{
    QMenu *sendToMenu = (QMenu *)sender();
    //frontend::ViewInfo* view = m_sendToMenuToView[sendToMenu];

    // If menu was dismissed without any action selected, we need to
    //  empty it before populating it anew
    emptySendToMenu(sendToMenu);

    /*
    // Populate menu (entries are deleted in sendViewTo signal handler)
    auto& graphicViewsBlocks = GraphicViewsManager::instance().getGraphicViewsBlocks();
    for (auto it = graphicViewsBlocks.cbegin(), end = graphicViewsBlocks.cend(); it != end; ++it) {
    QAction* action = sendToMenu->addAction((*it)->getName());
    connect(action, SIGNAL(triggered()), this, SLOT(sendViewTo()));
    m_sendToActionToView[action] = view;
    m_sendToActionToIsBlock[action] = true;
    m_sendToActionToGraphicViewBlock[action] = *it;

    for (size_t i = 0; i < 4; ++i) {
      graphicView::Widget* widget = (*it)->getWidget(i);
      QAction* subaction = sendToMenu->addAction(widget->getName());
      connect(subaction, SIGNAL(triggered()), this, SLOT(sendViewTo()));
      m_sendToActionToView[subaction] = view;
      m_sendToActionToIsBlock[subaction] = false;
      m_sendToActionToGraphicView[subaction] = widget;
    }
         }
         */
}

void
Menus::emptySendToMenu(QMenu *menu)
{
    menu->clear();
    m_sendToActionToView.clear();
    m_sendToActionToIsBlock.clear();
    m_sendToActionToGraphicView.clear();
    m_sendToActionToGraphicViewBlock.clear();
}

namespace
{

void
getExtensions(const std::string &extensions, const std::string &format,
              QString &allSupported, QString &descrip)
{
    std::stringstream st(extensions);
    std::string ext;
    QString allExt;
    while (st)
    {
        ext = "";
        std::getline(st, ext, ';');
        if (!ext.empty())
            allExt += (QString(ext.c_str()) + " ");
    }

    descrip += (format.c_str() + QString(" (") + allExt + ");;");
    allSupported += allExt;
}

static QString currentPath;

QString
selectOpenFileDialog()
{
    std::cout << "In selectOpenFileDialog ==========" << std::endl;
    auto loaders = frontend::getLoaders();

    QString allSupportedExts;
    QString dialogDescStr;
    for (auto it = loaders.begin(), itEnd = loaders.end(); it != itEnd; ++it)
    {
        std::string extensions = (*it)->fileExtensionFilters();
        getExtensions(extensions, (*it)->formatName(), allSupportedExts, dialogDescStr);
    }
    dialogDescStr = "All Supported (" + allSupportedExts + ");;" + dialogDescStr +
                    "All Files (*.*);;";

    currentPath = QFileDialog::getOpenFileName(&MainWindow::instance(),
                  QObject::tr("Open file"),
                  currentPath, dialogDescStr);
    return currentPath;
}

QString
selectSaveFileDialog(frontend::Saver::Type expectedType)
{
    QString dialogDescStr;
    if (expectedType == frontend::Saver::Unknown)
    {
        dialogDescStr = "All Files (*.*);;";
    }
    else
    {
        auto savers = frontend::getSavers();

        QString allSupportedExts;
        for (auto it = savers.begin(), itEnd = savers.end(); it != itEnd; ++it)
        {
            if ((*it)->saverType() != expectedType)
            {
                continue;
            }
            std::string extensions = (*it)->fileExtensionFilters();
            getExtensions(extensions, (*it)->formatName(), allSupportedExts, dialogDescStr);
        }
        dialogDescStr = "All Supported (" + allSupportedExts + ");;" + dialogDescStr +
                        "All Files (*.*);;";
    }

    currentPath = QFileDialog::getSaveFileName(&MainWindow::instance(),
                  QObject::tr("Save file"),
                  currentPath, dialogDescStr);
    return currentPath;
}

}

void
Menus::saveView(const frontend::ViewInfo *view)
{
    switch (view->getViewType())
    {
    case frontend::ViewInfo::MeshType:
        saveMesh(((frontend::MeshInfo *)view)->meshPtr.get());
        break;
    case frontend::ViewInfo::ImageType:
        saveImage(((frontend::ImageInfo *)view)->imagePtr.get());
        break;
    case frontend::ViewInfo::UndefinedType:
    default:
        assert(!"Should never get here!");
    }
}

namespace
{

void
checkErrorSave(bool ok, const std::string &err)
{
    if (ok)
    {
        if (!err.empty())
        {
            QMessageBox::warning(&MainWindow::instance(),
                                 QObject::tr("Warning saving the file"),
                                 QString::fromStdString(err));
        }
    }
    else
    {
        if (!err.empty())
        {
            QMessageBox::critical(&MainWindow::instance(),
                                  QObject::tr("Error saving the file"),
                                  QString::fromStdString(err));
        }
        else
        {
            QMessageBox::critical(&MainWindow::instance(),
                                  QObject::tr("Error saving the file"),
                                  "<Unknown error>");
        }
    }
}

void
checkErrorOpen(bool ok, const std::string &err)
{
    if (ok)
    {
        if (!err.empty())
        {
            QMessageBox::warning(&MainWindow::instance(),
                                 QObject::tr("Warning opening the file"),
                                 QString::fromStdString(err));
        }
    }
    else
    {
        if (!err.empty())
        {
            QMessageBox::critical(&MainWindow::instance(),
                                  QObject::tr("Error opening the file"),
                                  QString::fromStdString(err));
        }
        else
        {
            QMessageBox::critical(&MainWindow::instance(),
                                  QObject::tr("Error opening the file"),
                                  "<Unknown error>");
        }
    }
}

}

void
Menus::saveMesh(const core::MeshView *mesh)
{
    QString filepath = selectSaveFileDialog(frontend::Saver::MeshSaver);
    if (filepath.size() == 0)
        return;

    std::string err;
    bool ok = frontend::saveFile((core::MeshView *) mesh, filepath.toStdString());

    checkErrorSave(ok, err);
}

void
Menus::saveImage(const core::ImageView *image)
{
    QString filepath = selectSaveFileDialog(frontend::Saver::ImageSaver);
    if (filepath.size() == 0)
        return;

    std::string err;
    bool ok = frontend::saveFile((core::ImageView *) image, filepath.toStdString());

    checkErrorSave(ok, err);
}

void
Menus::saveImage(const QImage &image)
{
    QString filepath = selectSaveFileDialog(frontend::Saver::Unknown);
    if (filepath.size() == 0)
        return;
    bool ok = image.save(filepath);
    std::string err;
    if (!ok)
        err = "Qt error: unable to save file";

    checkErrorSave(ok, err);
}

void
Menus::openFile()
{
    QString filepath = selectOpenFileDialog();
    if (filepath.size() == 0)
        return;

    std::string err;
    bool ok = frontend::ViewManager::instance().createView(filepath.toStdString(), err);

    checkErrorOpen(ok, err);
}

void
Menus::openRawImage()
{
    LoaderAction *action = new LoaderAction("LoaderRawImagePlugin", m_fileMenu);
    action->runLoader();
}

void
Menus::createAndAssignViewWindow()
{
    /*
    QAction* s = (QAction*)sender();
    graphicView::ViewingWindowPtr vw = graphicView::ViewingWindowManager::instance().getNewWindow();
    m_actionToGraphicView[s]->getScene()->setViewWindow(vw);
    */
}

void
Menus::viewWindowSelected()
{
    /*
    QAction* s = (QAction*)sender();
    m_actionToGraphicView[s]->getScene()->setViewWindow(m_actionToViewWindow[s]);
    */
}

void
Menus::viewWindowAssigned(graphicView::ViewingWindow *window)
{
    graphicView::Scene *scene = (graphicView::Scene *)sender();
    const auto &actions = m_graphicViewToViewWindowMenu[scene]->actions();
    for (auto it = actions.begin(), end = actions.end(); it != end; ++it)
    {
        if ((*it)->text() == window->getName())
        {
            (*it)->setChecked(true);
        }
    }
}

void
Menus::createAndAssignSliceWindow()
{
    QAction *s = (QAction *)sender();
    graphicView::SliceWindowPtr sw = graphicView::SliceWindowManager::instance().getNewWindow();
    ((graphicView::ImageScene *) m_actionToGraphicView[s]->getScene())->setSliceWindow(sw);
}

void
Menus::sliceWindowSelected()
{
    QAction *s = (QAction *)sender();
    ((graphicView::ImageScene *) m_actionToGraphicView[s]->getScene())->setSliceWindow(m_actionToSliceWindow[s]);
}

void
Menus::sliceWindowAssigned(graphicView::SliceWindow *window)
{
    graphicView::Scene *scene = (graphicView::Scene *)sender();
    const auto &actions = m_graphicViewToSliceWindowMenu[scene]->actions();
    for (auto it = actions.begin(), end = actions.end(); it != end; ++it)
    {
        if ((*it)->text() == window->getName())
        {
            (*it)->setChecked(true);
        }
    }
}

void
Menus::saveView()
{
    QAction *action = (QAction *)sender();
    frontend::ViewInfo *view = m_saveActionToView[action];
    saveView(view);
}

void
Menus::sendViewTo()
{
    QAction *action = (QAction *) sender();
    frontend::ViewInfo *view = m_sendToActionToView[action];

    // Send to...
    if (m_sendToActionToIsBlock[action])
    {
        m_sendToActionToGraphicViewBlock[action]->setView(view);
    }
    else
    {
        m_sendToActionToGraphicView[action]->setView(view);
    }

    // ... and delete menu entries
    emptySendToMenu(m_viewToSendToMenu[view]);
}

void
Menus::configureView()
{
    QAction *action = (QAction *) sender();
    frontend::ViewInfo *view = m_configDeleteActionToView[action];

    std::unique_ptr<ViewConfigDialog> dialog(createViewConfigDialog(view, &MainWindow::instance()));
    dialog->exec();
}

void
Menus::setDeletableView(bool mark)
{
    QAction *action = (QAction *) sender();
    frontend::ViewInfo *view = m_configDeleteActionToView[action];
    MainWindow::instance().getGraph()->setDeletable(view, mark);
}

void
Menus::takeScreenshot()
{
    QAction *action = (QAction *) sender();
    graphicView::Widget *graphicView = m_actionToGraphicView[action];
    graphicView->takeScreenshot();
}

void
Menus::takeBlockScreenshot()
{
    auto block = MainWindow::instance().currentBlock();
    if (block)
        block->takeScreenshot();
}

}
}
