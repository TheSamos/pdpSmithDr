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

#include "LoaderManager.hpp"
#include <utils/sdFileUtils.hpp>

#include <map>

using namespace sd::frontend;

LoaderManager::~LoaderManager()
{
}

void
sd::frontend::registerLoader(Loader* obj)
{
  LoaderManager::instance().addObject(obj);
}

/*void
sd::frontend::registerLoader(Loader* obj, const ParameterList& parameters)
{
  obj->initialize(parameters);
  LoaderManager::instance().addObject(obj);
}*/

void
sd::frontend::registerLoader(Loader* obj, const std::string parameters)
{
  obj->initialize(parameters);
  std::cout << "param: " << parameters << std::endl;
  LoaderManager::instance().addObject(obj);
}

sd::frontend::Query<Loader>
sd::frontend::queryLoader(const std::string& name)
{
  return LoaderManager::instance().availablePrototypes(name);
}

const Query<Loader>
sd::frontend::getLoaders()
{
  Query<Loader> proto = LoaderManager::instance().availablePrototypes();
  if (proto.empty())
    return proto;

  struct Element {
    Element(std::string n, std::string f, std::string e)
      : name(n), fmt(f), ext(e)
    {
    }

    bool
    operator<(const Element& elt) const
    {
      if (name == elt.name) {
	if (fmt == elt.fmt) {
	  return (ext < elt.ext);
	}
	else
	  return (fmt < elt.fmt);
      }
      else
	return (name < elt.name);
    }

    std::string name;
    std::string fmt;
    std::string ext;
  };

  std::map<Element, bool> elements;

  // remove duplicates: same name, same formatName, same fileExtensionFilters
  auto predicate = [&elements](Loader* obj) -> bool {
    const std::string& name = obj->name();
    const std::string& fmtName = obj->formatName();
    const std::string& fileExt = obj->fileExtensionFilters();
    Element elt(name, fmtName, fileExt);
    if (elements.find(elt) != elements.end())
      return true;

    elements[elt] = true;
    return false;
  };

  proto.withoutPredicate(predicate);

  return proto;
}

const Query<Loader>
sd::frontend::detectLoaders(const std::string& filename)
{
  Query<Loader> proto = LoaderManager::instance().availablePrototypes();
  if (proto.empty())
    return proto;

  // get prototypes according to file extension filters
  auto predicate = [&filename](Loader* obj) -> bool {
    const std::string& fileExt = obj->fileExtensionFilters();
    return !sd::utils::fileMatchesFilters(filename, fileExt);
  };

  proto.withoutPredicate(predicate);

  return proto;
}
