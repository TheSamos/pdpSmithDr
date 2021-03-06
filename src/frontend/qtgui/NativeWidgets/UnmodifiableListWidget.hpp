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

#ifndef SD_QTGUI_NATIVE_WIDGETS_UNMODIFIABLE_LIST_WIDGET_HPP
#define SD_QTGUI_NATIVE_WIDGETS_UNMODIFIABLE_LIST_WIDGET_HPP

#include <frontend/libqt/QtParametrizationWidget.hpp>
//#include <frontend/lib/Parameter.hpp>
#include <frontend/libqt/SDRParameter.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
RESTORE_CONTEXT()

namespace sd {
  namespace qtgui {
    namespace nativewidgets {

      class UnmodifiableListWidget : public libqt::QtParametrizationWidget {

      public:

	UnmodifiableListWidget(sd::libqt::SDRParameter* p);

	virtual ~UnmodifiableListWidget();

	virtual void build(QWidget* parameterizer);

	virtual void updateParameter();
  virtual sd::libqt::SDRParameter* updateXMLParameter();

      };

    }
  }
}

#endif /*! SD_QTGUI_NATIVE_WIDGETS_UNMODIFIABLE_LIST_WIDGET_HPP */
