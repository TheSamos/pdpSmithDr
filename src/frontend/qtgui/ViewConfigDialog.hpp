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

#ifndef SD_QTGUI_VIEWCONFIGDIALOG_HPP
#define SD_QTGUI_VIEWCONFIGDIALOG_HPP

#include <vector>
#include <map>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
RESTORE_CONTEXT()

#include <frontend/lib/TypesInfo.hpp>

namespace sd {
  namespace qtgui {

    class ViewConfigDialog;

    ViewConfigDialog* createViewConfigDialog(frontend::ViewInfo* viewInfo,
					     QWidget* parent = 0);

    /*
     * ViewConfigDialog
     */
    class ViewConfigDialog
      : public QDialog
    {
      Q_OBJECT

    public:

      ViewConfigDialog(frontend::ViewInfo* viewInfo, QWidget* p);
      virtual ~ViewConfigDialog() = 0;

    protected:

      virtual bool valid() const;

    protected:

      QVBoxLayout* m_mainLayout;
      QHBoxLayout* m_nameLayout;

      QLabel* m_nameLabel;
      QLineEdit* m_nameEdit;

    private:

      frontend::ViewInfoPtr m_view;

      QVBoxLayout* m_masterLayout;   // mainLayout + ok/cancel

      QHBoxLayout* m_okCancelLayout;
      QPushButton* m_okButton;
      QPushButton* m_cancelButton;

    private slots:

      void okPushed();

    };

    /*
     * Mesh dialogs
     */
    class MeshFileViewConfigDialog
      : public ViewConfigDialog
    {

    public:

      MeshFileViewConfigDialog(frontend::ViewInfo* v, QWidget* p);

    };

    class MeshAlgorithmViewConfigDialog
      : public ViewConfigDialog
    {

    public:

      MeshAlgorithmViewConfigDialog(frontend::ViewInfo* v, QWidget* p);

    };

    /*
     * Image dialogs
     */
    class ImageFileViewConfigDialog
      : public ViewConfigDialog
    {

    public:

      ImageFileViewConfigDialog(frontend::ViewInfo* v, QWidget* p);

    };

    class ImageAlgorithmViewConfigDialog
      : public ViewConfigDialog
    {

    public:

      ImageAlgorithmViewConfigDialog(frontend::ViewInfo* v, QWidget* p);

    };

  }
}

#endif /*! SD_QTGUI_VIEWCONFIGDIALOG_HPP */
