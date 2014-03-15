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

#ifndef SD_FRONTEND_PARAMETER_HXX
#define SD_FRONTEND_PARAMETER_HXX

namespace sd {
  namespace frontend {

    inline std::string
    Parameter::name() const
    {
      return m_name;
    }

    inline void
    Parameter::setName(std::string name)
    {
      m_name = name;
    }

    inline std::string
    Parameter::getDescription() const
    {
      return m_description;
    }

    inline void
    Parameter::setDescription(std::string description)
    {
      m_description = description;
    }

    inline bool
    Parameter::hasPredefinedValues() const { return m_hasPredefinedValues; }

    inline bool
    Parameter::isMinDefined() const
    {
      return !(m_min.isA<double>() && anyCast<double>(m_min) == anyCast<double>(undefined));
    }

    inline bool
    Parameter::isMaxDefined() const
    {
      return !(m_max.isA<double>() && anyCast<double>(m_max) == anyCast<double>(undefined));
    }

    inline std::string
    Parameter::getWidgetFactoryName() const
    {
      return m_widgetFactoryName;
    }

    inline void
    Parameter::setWidgetFactoryName(std::string name)
    {
      m_widgetFactoryName = name;
    }

    namespace {

      void
      populate(ParameterList&)
      {
      }

      template<typename DataType, typename ...Params>
      void
      populate(ParameterList& pl, std::string name, const DataType& value, Params ...params)
      {
	pl.push_back(Parameter(name, value));
	populate(pl, std::forward<Params>(params)...);
      }

    }

    template<typename ...Params>
    ParameterList
    make_parameter_list(Params ...params)
    {
      ParameterList pl;
      populate(pl, params...);
      return pl;
    }

  }
}

#endif /*! SD_FRONTEND_PARAMETER_HXX */
