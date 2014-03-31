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

#ifndef SD_FRONTEND_BLACK_BOX_ALGORITHM_HPP
#define SD_FRONTEND_BLACK_BOX_ALGORITHM_HPP

#include <SmithDRDefs.hpp>
#include <core/ViewNode.hpp>
#include "Algorithm.hpp"
#include "AlgorithmInfo.hpp"
//#include "Parameter.hpp"

#include <vector>
#include <string>

namespace sd {
  
  namespace frontend {

    class BlackBoxAlgorithm : public core::ViewNode, public Algorithm {

    public:
      
      virtual const std::string& name() const {return core::ViewNode::name();}

    protected:

      /*BlackBoxAlgorithm(const std::string& algoName,
			const AlgorithmInfo& output = AlgorithmInfo(),
			const AlgorithmInfo& input = AlgorithmInfo(),
			const ParameterList& parameters = ParameterList());*/

      BlackBoxAlgorithm(const std::string& algoName,
      const AlgorithmInfo& output = AlgorithmInfo(),
      const AlgorithmInfo& input = AlgorithmInfo(),
      const std::string& parameters = "");

      virtual ~BlackBoxAlgorithm();

    private:

      BlackBoxAlgorithm(const BlackBoxAlgorithm&) = delete;
      BlackBoxAlgorithm& operator=(const BlackBoxAlgorithm&) = delete;

    };

  }
  
}

#endif /*! SD_FRONTEND_BLACK_BOX_ALGORITHM_HPP */
