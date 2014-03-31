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

#ifndef SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HXX
#define SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HXX

namespace sd {
  namespace libqt {

    QtParametrizationWidget::QtParametrizationWidget(sd::libqt::SDRParameter* p)
      : frontend::ParametrizationWidget(p),
	m_widget(0), m_layout(0)
    {
    }

    QtParametrizationWidget::~QtParametrizationWidget()
    {
    }

    QWidget*
    QtParametrizationWidget::widget() const
    {
      return m_widget;
    }

    QLayout*
    QtParametrizationWidget::layout() const
    {
      return m_layout;
    }

  }
}

#endif /*! SD_LIBQT_QT_PARAMETRIZATION_WIDGET_HXX */
