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

#ifndef SD_QTGUI_NATIVE_WIDGETS_MULTIPLE_VALUES_WIDGET_HPP
#define SD_QTGUI_NATIVE_WIDGETS_MULTIPLE_VALUES_WIDGET_HPP

#include <frontend/libqt/QtParametrizationWidget.hpp>
//#include <frontend/lib/Parameter.hpp>
#include <frontend/libqt/SDRParameter.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
#include <QListWidget>
RESTORE_CONTEXT()

#include <functional>

namespace sd {
  
  namespace qtgui {
    
    namespace nativewidgets {

      class MultipleValuesWidget : public libqt::QtParametrizationWidget {

      public:

	MultipleValuesWidget(sd::libqt::SDRParameter* p);

	virtual ~MultipleValuesWidget();

	virtual void build(QWidget* parameterizer);

	virtual void updateParameter();
  virtual sd::libqt::SDRParameter* updateXMLParameter();

      };

      /*
       * VectorEditor
       */
      class VectorEditor : public QListWidget {
	
	Q_OBJECT

	public:

	typedef std::function<QWidget* (void)> ItemCreator;

      public:

	VectorEditor(ItemCreator defaultItemCreator);

	void addNewItem(ItemCreator itemCreator);

      signals:

	void updated();

      private slots:

	void addElement();
	void removeElement();

      private:

	ItemCreator m_itemCreator;

      };

    }
    
  }
  
}

#endif /*! SD_QTGUI_NATIVE_WIDGETS_MULTIPLE_VALUES_WIDGET_HPP */
