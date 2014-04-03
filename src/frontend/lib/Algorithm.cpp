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

#include "Algorithm.hpp"

#include <core/ViewNode.hpp>
#include "AlgorithmInfo.hpp"

#include <vector>
#include <cassert>

#include <core/ViewDAG.hpp>

#include <frontend/libqt/QtXMLValueParser.hpp>
#include <frontend/libqt/SimpleParameter.hpp>

sd::core::ViewDAG sd::frontend::Algorithm::m_dag;

namespace sd
{

namespace frontend
{

/*#define VEC_NAME() Vec
#define CONTAINER(N) m_##parameters_##N

#define DEFINE_GET_ALL_RESET_ALL(ListNo)                \
    void                                \
    Algorithm::getAllParams_##ListNo(VEC_NAME()& v) const       \
    {                                   \
      for (auto itr = CONTAINER(ListNo).begin(); itr != CONTAINER(ListNo).end(); ++itr) \
    v.push_back(*itr);                      \
    }                                   \
    void                                \
    Algorithm::resetAllParams_##ListNo(VEC_NAME()& v)           \
    {                                   \
      CONTAINER(ListNo) = v;                        \
    }*/


/*
 * Algorithm
 */

/*Algorithm::Algorithm(const AlgorithmInfo& out,
         const AlgorithmInfo& in,
         const ParameterList& parameters)
  : m_parameters_0(), m_parameters_1(), m_parameters_2(), m_parameters_3(),
m_parameters_4(), m_parameters_5(), m_parameters_6(), m_parameters_7(),
m_input(in), m_output(out), m_expectedParameters(parameters), m_xml_parameters()
{
}*/

Algorithm::Algorithm(const AlgorithmInfo &out,
                     const AlgorithmInfo &in,
                     std::string parameters)
    : m_input(in), m_output(out), m_defaultXmlParams(parameters), m_xml_parameters()
{

}



Algorithm::~Algorithm()
{
#ifdef DEBUG
    std::cout << "~Algorithm" << std::endl;
#endif
}

const AlgorithmInfo &
Algorithm::input() const
{
    return m_input;
}

const AlgorithmInfo &
Algorithm::output() const
{
    return m_output;
}

/*const ParameterList&
Algorithm::expectedParameters() const
{
  return m_expectedParameters;
}*/


const std::string& Algorithm::getXMLString() const
{
    return m_defaultXmlParams;
}





/*void
Algorithm::initialize(const ParameterList& parameters)
{
  m_expectedParameters = parameters;
}*/

void
Algorithm::initialize(std::string defaultXmlParam)
{
    //m_expectedParameters = ParameterList();
    m_defaultXmlParams = defaultXmlParam;
}

/*void
Algorithm::setParams(const std::vector<frontend::ParameterList>& p)
{
  std::cout << "Setting params" << std::endl;

  for (size_t listNo = 0; listNo < p.size(); ++listNo) {
if (listNo == 0) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_0(p[listNo][j]);
}
else if (listNo == 1) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_1(p[listNo][j]);
}
else if (listNo == 2) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_2(p[listNo][j]);
}
else if (listNo == 3) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_3(p[listNo][j]);
}
else if (listNo == 4) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_4(p[listNo][j]);
}
else if (listNo == 5) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_5(p[listNo][j]);
}
else if (listNo == 6) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_6(p[listNo][j]);
}
else if (listNo == 7) {
  for (size_t j = 0; j < p[listNo].size(); ++j)
    setParams_7(p[listNo][j]);
}
  }
}*/

void
Algorithm::setXMLParams(std::map<std::string, std::string> new_params)
{
    m_xml_parameters = new_params;
    std::cout << "Setting params_xml" << std::endl;
}

sd::libqt::SDRParameter *
Algorithm::getXMLParams(std::string name)
{
    std::string parameter = m_xml_parameters[name];
    std::cout << "getXMLParams: parameter: " << parameter << std::endl;
    sd::libqt::QtXMLValueParser valueParser(parameter);

    std::cout << "getXMLParams: parameter: " << parameter << std::endl;
    
    sd::libqt::SDRParameter *p = valueParser.parseSimpleParameter();
    
    if(p == nullptr)
      std::cout << "nullptr in getXMLParams (Algorithm)" << std::endl;

    return p;
}


void
Algorithm::setInputNodes(const std::vector<ViewInfoPtr> &inputs)
{
    core::ViewNode *child = dynamic_cast<core::ViewNode *>(this);

    for (size_t i = 0; i < inputs.size(); ++i)
    {
        switch (inputs[i].get()->getViewType())
        {
        case ViewInfo::MeshType:
        {
            MeshInfo *meshInfo = (MeshInfo *) inputs[i].get();
            child->addParent(meshInfo->meshPtr.get());
        }
        break;
        case ViewInfo::ImageType:
        {
            ImageInfo *imageInfo = (ImageInfo *) inputs[i].get();
            child->addParent(imageInfo->imagePtr.get());
        }
        break;
        case ViewInfo::UndefinedType:
        default:
            assert(!"Should never get here!");
        }
    }
}

const std::vector<core::ViewNode *> &
Algorithm::getOutputs()
{
    core::ViewNode *parent = dynamic_cast<core::ViewNode *>(this);
    std::vector<core::ViewNode *>::const_iterator it = parent->getNextViewNodes().begin();
    const std::vector<core::ViewNode *>::const_iterator end = parent->getNextViewNodes().end();
    for (; it != end; ++it)
    {
        (*it)->setGhostView(false);
        (*it)->setDAG(&m_dag);
    }
    return parent->getNextViewNodes();
}

void
Algorithm::disconnectOutputNodes()
{
    core::ViewNode *parent = dynamic_cast<core::ViewNode *>(this);
    parent->disconnectChildren();
}

/*DEFINE_GET_ALL_RESET_ALL(0)
DEFINE_GET_ALL_RESET_ALL(1)
DEFINE_GET_ALL_RESET_ALL(2)
DEFINE_GET_ALL_RESET_ALL(3)
DEFINE_GET_ALL_RESET_ALL(4)
DEFINE_GET_ALL_RESET_ALL(5)
DEFINE_GET_ALL_RESET_ALL(6)
DEFINE_GET_ALL_RESET_ALL(7)

void
Algorithm::getAllParams(int listNo, VEC_NAME()& v) const
{
  assert(listNo >= 0);
  assert(listNo < Algorithm::MaxNbList);
  if (listNo == 0)
getAllParams_0(v);
  else if (listNo == 1)
getAllParams_1(v);
  else if (listNo == 2)
getAllParams_2(v);
  else if (listNo == 3)
getAllParams_3(v);
  else if (listNo == 4)
getAllParams_4(v);
  else if (listNo == 5)
getAllParams_5(v);
  else if (listNo == 6)
getAllParams_6(v);
  else if (listNo == 7)
getAllParams_7(v);
}

void
Algorithm::resetAllParams(int listNo, VEC_NAME()& v)
{
  assert(listNo >= 0);
  assert(listNo < Algorithm::MaxNbList);
  if (listNo == 0)
resetAllParams_0(v);
  else if (listNo == 1)
resetAllParams_1(v);
  else if (listNo == 2)
resetAllParams_2(v);
  else if (listNo == 3)
resetAllParams_3(v);
  else if (listNo == 4)
resetAllParams_4(v);
  else if (listNo == 5)
resetAllParams_5(v);
  else if (listNo == 6)
resetAllParams_6(v);
  else if (listNo == 7)
resetAllParams_7(v);
}

#undef DEFINE_GET_ALL_RESET_ALL
#undef CONTAINER
#undef VEC_NAME*/

}
}
