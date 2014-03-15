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

#include "AlgorithmManager.hpp"

using namespace sd::frontend;

AlgorithmManager::~AlgorithmManager()
{}

void
sd::frontend::registerAlgorithm(Algorithm* obj) {
  AlgorithmManager::instance().addObject(obj);
}

void
sd::frontend::registerAlgorithm(Algorithm* obj, const ParameterList& parameters) {
  obj->initialize(parameters);
  AlgorithmManager::instance().addObject(obj);
}

sd::frontend::Query<Algorithm>
sd::frontend::queryAlgorithm(const std::string& name) {
  return AlgorithmManager::instance().availablePrototypes(name);
}

const Query<Algorithm>
sd::frontend::getAlgorithms() {
  Query<Algorithm> proto = AlgorithmManager::instance().availablePrototypes();
  if (proto.empty())
    return proto;

  std::map<std::string, bool> names;

  // remove duplicates: same name
  auto predicate = [&names](Algorithm* obj) -> bool {
    const std::string& n = obj->name();
    if (names.find(n) != names.end())
      return true;

    names[n] = true;
    return false;
  };

  proto.withoutPredicate(predicate);

  return proto;
}
