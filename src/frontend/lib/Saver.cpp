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

#include "Saver.hpp"

/*sd::frontend::Saver::Saver(const std::string& saverName,
			   const sd::frontend::AlgorithmInfo& in,
			   const sd::frontend::ParameterList& parameters)
  : BlackBoxAlgorithm(saverName, AlgorithmInfo(), in, parameters) {
  setGhostView();
}*/

sd::frontend::Saver::Saver(const std::string& saverName,
         const sd::frontend::AlgorithmInfo& in,
         const std::string& parameters)
  : BlackBoxAlgorithm(saverName, AlgorithmInfo(), in, parameters) {
  setGhostView();
}

sd::frontend::Saver::~Saver() {}
