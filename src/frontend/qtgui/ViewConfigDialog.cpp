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

#include "ViewConfigDialog.hpp"
#include <frontend/lib/ViewManager.hpp>

#include "MainWindow.hpp"

namespace sd {
  namespace qtgui {

    ViewConfigDialog*
    createViewConfigDialog(frontend::ViewInfo* viewInfo, QWidget* parent)
    {
      assert(viewInfo);

      switch (viewInfo->getViewType()) {
      case frontend::ViewInfo::MeshType: {
	frontend::MeshInfo* meshInfo = (frontend::MeshInfo*)viewInfo;
	switch (meshInfo->getOriginType()) {
	case frontend::MeshInfo::File: {
	  frontend::FileMeshInfo* fileMeshInfo = (frontend::FileMeshInfo*)meshInfo;
	  return new MeshFileViewConfigDialog(fileMeshInfo, parent);
	}
	  break;
	case frontend::MeshInfo::AlgorithmOutput: {
	  frontend::AlgorithmOutputMeshInfo* algorithmMeshInfo = (frontend::AlgorithmOutputMeshInfo*)meshInfo;
	  return new MeshAlgorithmViewConfigDialog(algorithmMeshInfo, parent);
	}
	  break;
	default:
	  assert(!"Should never get here!");
	}
      }
	break;
      case frontend::ViewInfo::ImageType: {
	frontend::ImageInfo* imageInfo = (frontend::ImageInfo*)viewInfo;
	switch (imageInfo->getOriginType()) {
	case frontend::ImageInfo::File: {
	  frontend::FileImageInfo* fileImageInfo = (frontend::FileImageInfo*)imageInfo;
	  return new ImageFileViewConfigDialog(fileImageInfo, parent);
	}
	  break;
	case frontend::ImageInfo::AlgorithmOutput: {
	  frontend::AlgorithmOutputImageInfo* algorithmImageInfo = (frontend::AlgorithmOutputImageInfo*)imageInfo;
	  return new ImageAlgorithmViewConfigDialog(algorithmImageInfo, parent);
	}
	  break;
	default:
	  assert(!"Should never get here!");
	}
      }
	break;
      case frontend::ViewInfo::UndefinedType:
      default:
	assert(!"Should never get here!");
      }
      return 0;   // Should never get here!
    }

    /*
     * ViewConfigDialog
     */
    ViewConfigDialog::ViewConfigDialog(frontend::ViewInfo* v, QWidget* p)
      : QDialog(p), m_view(v)
    {
      m_masterLayout = new QVBoxLayout;
      setLayout(m_masterLayout);

      m_masterLayout->addLayout((m_mainLayout = new QVBoxLayout));

      m_nameLayout = new QHBoxLayout;
      m_nameLayout->addWidget((m_nameLabel = new QLabel(tr("View name:"), p)));
      m_nameLayout->addWidget((m_nameEdit = new QLineEdit(QString::fromStdString(v->getName()), p)));

      m_mainLayout->addLayout(m_nameLayout);

      m_masterLayout->addLayout((m_okCancelLayout = new QHBoxLayout));
      m_okCancelLayout->addWidget((m_okButton = new QPushButton(tr("Ok"))));
      connect(m_okButton, SIGNAL(clicked()), this, SLOT(okPushed()));
      m_okCancelLayout->addWidget((m_cancelButton = new QPushButton(tr("Cancel"))));
      connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    }

    ViewConfigDialog::~ViewConfigDialog()
    {
    }

    void
    ViewConfigDialog::okPushed()
    {
      if (valid()) {
	//m_view->setName(m_nameEdit->text().toStdString());
	bool ok = frontend::ViewManager::instance().changeViewName(m_view.get(), m_nameEdit->text().toStdString());
	if (!ok) {
	  QMessageBox::warning(&MainWindow::instance(),
			       QObject::tr("View configuration problem"),
			       QObject::tr("This name already exists!"));
	  reject();
	}
	accept();
      }
    }

    bool
    ViewConfigDialog::valid() const
    {
      return !m_nameEdit->text().isEmpty();
    }

    /*
     * Mesh dialogs
     */
    MeshFileViewConfigDialog::MeshFileViewConfigDialog(frontend::ViewInfo* v, QWidget* p)
      : ViewConfigDialog(v, p)
    {
    }

    MeshAlgorithmViewConfigDialog::MeshAlgorithmViewConfigDialog(frontend::ViewInfo* v, QWidget* p)
      : ViewConfigDialog(v, p)
    {
    }

    /*
     * Image dialogs
     */
    ImageFileViewConfigDialog::ImageFileViewConfigDialog(frontend::ViewInfo* v, QWidget* p)
      : ViewConfigDialog(v, p)
    {
    }

    ImageAlgorithmViewConfigDialog::ImageAlgorithmViewConfigDialog(frontend::ViewInfo* v, QWidget* p)
      : ViewConfigDialog(v, p)
    {
    }

  }
}
