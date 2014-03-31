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

#ifndef SD_FRONTEND_ALGORITHM_HXX
#define SD_FRONTEND_ALGORITHM_HXX

#include <core/ViewNode.hpp>
#include "AlgorithmInfo.hpp"
//#include "Parameter.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace sd {
  namespace frontend {
/*
#ifdef DEBUG
#define VERBOSE(op, param)					\
    std::cout << "\t" << #op << " " << param << std::endl;
#else
#define VERBOSE(op, param)
#endif

    template<typename Vec, typename Itr>
    Itr
    findElement(const std::string& eltName, Vec& vec)
    {
#ifdef DEBUG
      std::cout << "search: " << eltName << std::endl;
#endif
      return std::find_if(vec.begin(), vec.end(), [&eltName](const Parameter& p) -> bool {
#ifdef DEBUG
	  std::cout << "compare: " << p.name() << " to " << eltName << std::endl;
#endif
	  return p.name() == eltName;
	});
    }

    template<typename Vec, typename Itr>
    Parameter&
    getElement(const std::string& eltName, Vec& vec)
    {
      Itr it = findElement<Vec,Itr>(eltName, vec);
      if (it != vec.end()) {
#ifdef DEBUG
	std::cout << "found\n";
#endif
	return *it;
      }
#ifdef DEBUG
      std::cout << "not found\n";
#endif
      vec.push_back(Parameter(eltName));
      return vec.back();
    }

#define DEFINE_TYPED_GET_SET(GetName, SetName, Vec, Itr, Container)	\
    template<typename DataType, typename... Params>			\
    void								\
    Algorithm::SetName(std::string pName, const DataType& p, Params ...params) \
    {									\
      Parameter& param = getElement<Vec,Itr>(pName, Container);		\
      param = std::move(p);						\
      VERBOSE(set, param);						\
      SetName(std::forward<Params>(params)...);				\
    }									\
    template<typename... Params>					\
    void								\
    Algorithm::GetName(std::string pName, Parameter& p, Params&& ...params) \
    {									\
      p = getElement<Vec,Itr>(pName,Container);				\
      VERBOSE(get, p);							\
      GetName(std::forward<Params>(params)...);				\
    }									\
    template<typename... Params>					\
    void								\
    Algorithm::SetName(const Parameter& p, Params ...params)		\
    {									\
      getElement<Vec,Itr>(p.name(),Container) = std::move(p);	\
      VERBOSE(set, p);							\
      SetName(std::forward<Params>(params)...);				\
    }

#define VEC_NAME() Vec
#define ITR_NAME() VecItr
#define CONTAINER(N) m_##parameters_##N
#define DEFAULT_GET_NAME() getParams
#define DEFAULT_SET_NAME() setParams
#define GET_NAME(N) getParams##_##N
#define SET_NAME(N) setParams##_##N

#define DEFINE_GET_SET_LIST(ListNo)					\
    DEFINE_TYPED_GET_SET(GET_NAME(ListNo),				\
			 SET_NAME(ListNo),				\
			 VEC_NAME(),					\
			 ITR_NAME(),					\
			 CONTAINER(ListNo)				\
			 )

*/
    /* Make getParams_N and setParams_N the default methods */
/*#define SET_DEFAULT_GET_SET_LIST(DefaultListNo)		\
    DEFINE_TYPED_GET_SET(DEFAULT_GET_NAME(),		\
			 DEFAULT_SET_NAME(),		\
			 VEC_NAME(),			\
			 ITR_NAME(),			\
			 CONTAINER(DefaultListNo)	\
			 )*/


    /*
     * Algorithm
     */

    template<typename... Nodes>
    void
    Algorithm::setParents(core::ViewNode* child, core::ViewNode* node, Nodes ...nodes)
    {
      child->addParent(node);
      setParents(child, std::forward<Nodes>(nodes)...);
    }

    template<typename... Nodes>
    void
    Algorithm::disconnectParents(core::ViewNode* child, core::ViewNode* node, Nodes ...nodes)
    {
      node->disconnectChild(child);
      disconnectParents(child, std::forward<Nodes>(nodes)...);
    }

    template<typename First, typename ...Rest>
    void
    Algorithm::getChild(int i, const std::vector<core::ViewNode*>& children, First& first, Rest& ...rest)
    {
      first = static_cast<First>(children[i]);
      getChild(i+1, children, std::forward<Rest>(rest)...);
    }

    template<typename... Nodes>
    void
    Algorithm::setInputNodes(Nodes ...nodes)
    {
      if (!m_input.validate(nodes...)) {
	std::cout << "Incorrect algorithm input!\n";
	return;
      }

      core::ViewNode* child = dynamic_cast<core::ViewNode*>(this);
      setParents(child, nodes...);
    }

    template<typename... Nodes>
    void
    Algorithm::disconnectInputNodes(Nodes ...nodes)
    {
      core::ViewNode* child = dynamic_cast<core::ViewNode*>(this);
      disconnectParents(child, nodes...);
    }

    template<typename... Nodes>
    void
    Algorithm::getOutputs(Nodes& ...nodes)
    {
      if (!m_output.validate(nodes...)) {
	std::cout << "Incorrect algorithm output!\n";
	return;
      }

      core::ViewNode* parent = dynamic_cast<core::ViewNode*>(this);
      const std::vector<core::ViewNode*>& children = parent->getNextViewNodes();

      if (sizeof...(nodes) != children.size()) {
	std::cout << "\tExpecting " << sizeof...(nodes) << " child(ren) exactly, found " << children.size() << ". Abort!\n";
	return;
      }

      getChild(0, children, nodes...);
    }
        
    /*SET_DEFAULT_GET_SET_LIST(0)

    static const int MaxNbList = 8;
    DEFINE_GET_SET_LIST(0)
    DEFINE_GET_SET_LIST(1)
    DEFINE_GET_SET_LIST(2)
    DEFINE_GET_SET_LIST(3)
    DEFINE_GET_SET_LIST(4)
    DEFINE_GET_SET_LIST(5)
    DEFINE_GET_SET_LIST(6)
    DEFINE_GET_SET_LIST(7)*/

    // Manage char* as std::string
    /*template<typename... Params>
    void
    Algorithm::setParams(std::string pName, const char* p, Params ...params)
    {
      setParams(pName, std::string(p), params...);
    }

#undef SET_DEFAULT_GET_SET_LIST
#undef DEFINE_GET_SET_LIST
#undef SET_NAME
#undef GET_NAME
#undef DEFAULT_SET_NAME
#undef DEFAULT_GET_NAME
#undef CONTAINER
#undef ITR_NAME
#undef VEC_NAME
#undef DEFINE_TYPED_GET_SET
#undef VERBOSE*/

  }
}

#endif /*! SD_FRONTEND_ALGORITHM_HXX */
