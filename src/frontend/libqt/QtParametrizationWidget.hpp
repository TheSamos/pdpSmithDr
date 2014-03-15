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

#ifndef SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HPP
#define SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HPP

#include <frontend/lib/ParametrizationWidget.hpp>
#include <frontend/lib/Parameter.hpp>

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
#include <QLayout>
RESTORE_CONTEXT()

namespace sd {
  
  namespace libqt {
    
    class QtParametrizationWidget : public frontend::ParametrizationWidget {

    public:

      inline virtual ~QtParametrizationWidget();

      inline QWidget* widget() const;
      inline QLayout* layout() const;

      virtual void build(QWidget* parameterizer) = 0;

    protected:

      inline QtParametrizationWidget(frontend::Parameter& p);

    protected:

      QWidget* m_widget;
      QLayout* m_layout;

    };

  }
  
}

#include "QtParametrizationWidget.hxx"

#endif /*! SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HPP */
