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

#include "ParametrizationWidgetFactoryManager.hpp"

using namespace sd::frontend;

ParametrizationWidgetFactoryManager::~ParametrizationWidgetFactoryManager()
{
}

void
sd::frontend::registerWidgetFactory(ParametrizationWidgetFactory* obj)
{
  ParametrizationWidgetFactoryManager::instance().addObject(obj);
}

const sd::frontend::ParametrizationWidgetFactory*
sd::frontend::queryWidgetFactory(const std::string& name)
{
  return ParametrizationWidgetFactoryManager::instance().availablePrototypes(name).first();
}
