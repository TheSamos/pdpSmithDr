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

#include "SaverManager.hpp"

using namespace sd::frontend;

SaverManager::~SaverManager()
{
}

void
sd::frontend::registerSaver(Saver* obj)
{
  SaverManager::instance().addObject(obj);
}

const Query<Saver>
sd::frontend::getSavers()
{
  Query<Saver> proto = SaverManager::instance().availablePrototypes();
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
  auto predicate = [&elements](Saver* obj) -> bool {
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
