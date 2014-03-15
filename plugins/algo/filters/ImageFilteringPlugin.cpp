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

#include <frontend/lib/AlgorithmManager.hpp>

#include <vector>
#include <iostream>

#include "FilteringCommon.hxx"

using namespace sd::imageprocessing;

DECLARE_FILTERING_PLUGIN_NO_ARG(Kirsch, Filters:Edge Detection:)
DECLARE_FILTERING_PLUGIN_NO_ARG(Prewitt, Filters:Edge Detection:)
DECLARE_FILTERING_PLUGIN_NO_ARG(Roberts, Filters:Edge Detection:)

DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V1(Uniform, Filters:Noise Removal:)
DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V1(Median, Filters:Noise Removal:)
DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V2(Sobel, Filters:Edge Detection:)

DECLARE_FILTERING_PLUGIN_TWO_ARGS_KERNEL_SIGMA(Log, Filters:Edge Detection:)
DECLARE_FILTERING_PLUGIN_TWO_ARGS_KERNEL_SIGMA(GaussianLinear, Filters:Noise Removal:)

DECLARE_FILTERING_PLUGIN_ONE_ARG_SIGMA(GaussianFrequency, Filters:Noise Removal:)

DECLARE_FILTERING_PLUGIN_THREE_ARGS_KERNEL_SIGMA1_SIGMA2(Dog, Filters:Generic:)

DECLARE_FILTERING_PLUGIN_MORPHO(Filters:Generic:)


#define REGISTER_PLUGIN_NO_PARAMS(FilterName)				\
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::UINT8>); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::INT8>); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::UINT16>); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::INT16>); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<float>);	\
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<double>);

#define REGISTER_PLUGIN_WITH_PARAMS(FilterName, Parameters)			\
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::UINT8>, Parameters); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::INT8>, Parameters); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::UINT16>, Parameters); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<sd::INT16>, Parameters); \
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<float>, Parameters);	\
  sd::frontend::registerAlgorithm(new FilterName##ImageFilteringPlugin<double>, Parameters);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin() {
  REGISTER_PLUGIN_NO_PARAMS(Kirsch);
  REGISTER_PLUGIN_NO_PARAMS(Prewitt);
  REGISTER_PLUGIN_NO_PARAMS(Roberts);

  sd::frontend::ParameterList p1k = sd::frontend::make_parameter_list("kernel_size", 3);
  REGISTER_PLUGIN_WITH_PARAMS(Uniform, p1k);
  REGISTER_PLUGIN_WITH_PARAMS(Median, p1k);
  REGISTER_PLUGIN_WITH_PARAMS(Sobel, p1k);

  sd::frontend::ParameterList p2ks = sd::frontend::make_parameter_list("kernel_size", 3, "sigma", 0.5);
  REGISTER_PLUGIN_WITH_PARAMS(Log, p2ks);
  REGISTER_PLUGIN_WITH_PARAMS(GaussianLinear, p2ks);

  sd::frontend::ParameterList p1s = sd::frontend::make_parameter_list("sigma", 50.);
  REGISTER_PLUGIN_WITH_PARAMS(GaussianFrequency, p1s);

  sd::frontend::ParameterList p3kss = sd::frontend::make_parameter_list("kernel_size", 3, "sigma1", 0.2, "sigma2", 0.5);
  REGISTER_PLUGIN_WITH_PARAMS(Dog, p3kss);

  const std::vector<std::string> availableMorphoFilters = { "Dilation", "Erosion", "Opening", "Closing" };
  const std::vector<std::string> availableStructElt = { "square", "circle", "cross" };

  sd::frontend::Parameter mf("method", availableMorphoFilters[0], availableMorphoFilters);
  sd::frontend::Parameter sel("structural element", availableStructElt[0], availableStructElt);
  sd::frontend::ParameterList paramMorpho;
  paramMorpho.push_back(mf);
  paramMorpho.push_back(sel);
  paramMorpho.push_back(sd::frontend::Parameter("side", 3));
  paramMorpho.push_back(sd::frontend::Parameter("radius", 1));
  paramMorpho.push_back(sd::frontend::Parameter("is diagonal", true));
  REGISTER_PLUGIN_WITH_PARAMS(Morphological, paramMorpho);
}

#undef DECLARE_FILTERING_PLUGIN_MORPHO
#undef DECLARE_FILTERING_PLUGIN_THREE_ARGS_KERNEL_SIGMA1_SIGMA2
#undef DECLARE_FILTERING_PLUGIN_TWO_ARGS_KERNEL_SIGMA
#undef DECLARE_FILTERING_PLUGIN_ONE_ARG_SIGMA
#undef DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V2
#undef DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V1
#undef DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_HEAD
#undef DECLARE_FILTERING_PLUGIN_NO_ARG
#undef DECLARE_RUN_FOOT
#undef DECLARE_RUN_HEAD
#undef DECLARE_FILTERING_PLUGIN
#undef DECLARE_FILTERING_PLUGIN_CLASS

#undef REGISTER_PLUGIN_WITH_PARAMS
#undef REGISTER_PLUGIN_NO_PARAMS
