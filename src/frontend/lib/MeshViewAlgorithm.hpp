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

#ifndef SD_FRONTEND_MESH_VIEW_ALGORITHM_HPP
#define SD_FRONTEND_MESH_VIEW_ALGORITHM_HPP

#include <SmithDRDefs.hpp>
#include <core/mesh/MeshView_.hpp>
#include "Algorithm.hpp"
#include "AlgorithmInfo.hpp"
#include "Parameter.hpp"

namespace sd {
  
  namespace frontend {

    template<typename T, unsigned int N>
    class MeshViewAlgorithm : public core::MeshView_<T, N>, public Algorithm {

    public:
      
    protected:
      
      MeshViewAlgorithm(const std::string& name,
			       const AlgorithmInfo& input = AlgorithmInfo(),
			       const ParameterList& parameters = ParameterList());
      
      void init();
      
      virtual ~MeshViewAlgorithm() {}
      
    private:
      
      MeshViewAlgorithm(const MeshViewAlgorithm<T, N>&) = delete;
      MeshViewAlgorithm& operator=(const MeshViewAlgorithm<T, N>&) = delete;
      
    };
    
  }
  
}

#include "MeshViewAlgorithm.hxx"

#endif /*! SD_FRONTEND_MESH_VIEW_ALGORITHM_HPP */
