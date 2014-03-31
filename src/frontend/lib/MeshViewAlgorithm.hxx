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

#ifndef SD_FRONTEND_MESH_VIEW_ALGORITHM_HXX
#define SD_FRONTEND_MESH_VIEW_ALGORITHM_HXX

/*template<typename T, unsigned int N>
sd::frontend::MeshViewAlgorithm<T, N>::MeshViewAlgorithm(const std::string& name,
							 const AlgorithmInfo& input,
							 const ParameterList& parameters)
  : sd::core::MeshView_<T, N>(name),
    sd::frontend::Algorithm(make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N)),
			    input, parameters)
{}*/

template<typename T, unsigned int N>
sd::frontend::MeshViewAlgorithm<T, N>::MeshViewAlgorithm(const std::string& name,
               const AlgorithmInfo& input,
               const std::string& parameters)
  : sd::core::MeshView_<T, N>(name),
    sd::frontend::Algorithm(make_info(sd::core::ObjectDescription::MeshView(sd::core::DataType_<T>(), N)),
          input, parameters)
{}

template<typename T, unsigned int N>
void
sd::frontend::MeshViewAlgorithm<T, N>::init() {
  this->build();
}

#endif /*! SD_FRONTEND_MESH_VIEW_ALGORITHM_HXX */
