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

#ifndef SD_FRONTEND_PARAMETRIZATION_WIDGET_FACTORY_HPP
#define SD_FRONTEND_PARAMETRIZATION_WIDGET_FACTORY_HPP

#include <SmithDRDefs.hpp>
#include <string>

#include <frontend/libqt/SDRParameter.hpp>

namespace sd {
  namespace frontend {

    class ParametrizationWidget;
    class Parameter;

    class ParametrizationWidgetFactory
    {

    public:

      virtual ~ParametrizationWidgetFactory() { }

      virtual const std::string& name() const = 0;

      virtual ParametrizationWidget* createWidget(sd::libqt::SDRParameter* p) const = 0;

    protected:

      ParametrizationWidgetFactory() { }

    };

  }
}

#endif /*! SD_FRONTEND_PARAMETRIZATION_WIDGET_FACTORY_HPP */
