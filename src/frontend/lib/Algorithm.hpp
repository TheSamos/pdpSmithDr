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

#ifndef SD_FRONTEND_ALGORITHM_HPP
#define SD_FRONTEND_ALGORITHM_HPP

#include <SmithDRDefs.hpp>
#include <core/ViewNode.hpp>
#include <core/Clonable.hpp>
#include "AlgorithmInfo.hpp"
//#include "Parameter.hpp"
#include "TypesInfo.hpp"

#include <frontend/libqt/SDRParameter.hpp>



#include <vector>
#include <string>

namespace sd {
  
  namespace frontend {

/*#define DEFINE_TYPED_GET_SET(GetName, SetName, Vec, Itr, Container)	\
    public:								\
									\
    template<typename DataType, typename... Params>			\
    void SetName(std::string pName, const DataType& p, Params ...params); \
									\
    template<typename... Params>					\
    void GetName(std::string pName, Parameter& p, Params&& ...params);	\
									\
    template<typename... Params>					\
    void SetName(const Parameter& p, Params ...params);


#define VEC_NAME() Vec
#define ITR_NAME() VecItr
#define CONTAINER(N) m_##parameters_##N
#define DEFAULT_GET_NAME() getParams
#define DEFAULT_SET_NAME() setParams
#define GET_NAME(N) getParams##_##N
#define SET_NAME(N) setParams##_##N

#define DEFINE_TYPED_GET_SET_AND_CONTAINER(ListNo)	\
    DEFINE_TYPED_GET_SET(GET_NAME(ListNo),		\
			 SET_NAME(ListNo),		\
			 VEC_NAME(),			\
			 ITR_NAME(),			\
			 CONTAINER(ListNo)		\
			 )				\
    void getAllParams_##ListNo(VEC_NAME()& v) const;	\
    void resetAllParams_##ListNo(VEC_NAME()& v);	\
							\
  private:						\
							\
  VEC_NAME() CONTAINER(ListNo);


#define DEFINE_GET_SET_LIST(ListNo)		\
    DEFINE_TYPED_GET_SET_AND_CONTAINER(ListNo)	\
						\
    private:					\
						\
    void getParams##_##ListNo () { }		\
    void setParams##_##ListNo () { }*/


    /* Make getParams_N and setParams_N the default methods */
/*#define SET_DEFAULT_GET_SET_LIST(DefaultListNo)		\
    private:						\
							\
    typedef std::vector<Parameter> VEC_NAME();		\
    typedef VEC_NAME()::iterator ITR_NAME();		\
    DEFINE_TYPED_GET_SET(DEFAULT_GET_NAME(),		\
			 DEFAULT_SET_NAME(),		\
			 VEC_NAME(),			\
			 ITR_NAME(),			\
			 CONTAINER(DefaultListNo)	\
			 )*/

    // NOTE: using vectors
    //    - need an order on elements which may respect order of parameter specification (user may want: first parameter added, first displayed)
    //    - only add at end; no erase
    //    - search for existing entry (each time setParams is called)
    // ==> prefer vectors for random iterators, no overhead, and [] operator

    /*
     * Algorithm
     */

    class Algorithm/* : public sd::core::Clonable*/ {

    public:

      static inline sd::core::ViewDAG* dag() {return &m_dag;}

    private:

      static sd::core::ViewDAG m_dag;

    public:

      virtual ~Algorithm();
      
      virtual const std::string& name() const = 0;
      virtual Algorithm* clone() /*const*/ = 0;
      // virtual Algorithm* clone(const std::vector<core::ViewNode*>& parents) const = 0;
      
      virtual bool prerun() {return true;}
      virtual bool run() = 0;
      virtual bool rerun() {return this->run();}
      virtual bool rerunable() {return false;}
      
      const AlgorithmInfo& input() const;
      const AlgorithmInfo& output() const;
      //const ParameterList& expectedParameters() const;
      
      const std::string getXmlString() const;
      sd::libqt::SDRParameter *getXMLParams(std::string name);
      
      //void initialize(const ParameterList&); // set expectedParameters
      void initialize(std::string defaultXmlParam);
      
      //void setParams(const std::vector<frontend::ParameterList>&);

      void setXMLParams(std::map<std::string, std::string> new_params);
      
    protected:
      
      /*Algorithm(const AlgorithmInfo& output,
		const AlgorithmInfo& input,
		const ParameterList& parameters);*/

      Algorithm(const AlgorithmInfo& out,
       const AlgorithmInfo& in,
       std::string parameters);
      
    private:
      
      void setParents(core::ViewNode*) { }
      void disconnectParents(core::ViewNode*) { }
      void getChild(int, const std::vector<core::ViewNode*>&) { }

      template<typename... Nodes>
      void setParents(core::ViewNode* child, core::ViewNode* node, Nodes ...nodes);
      
      template<typename... Nodes>
      void disconnectParents(core::ViewNode* child, core::ViewNode* node, Nodes ...nodes);
      
      template<typename First, typename ...Rest>
      void getChild(int i, const std::vector<core::ViewNode*>& children, First& first, Rest& ...rest);
      
      
    public:
      
      template<typename... Nodes>
      void setInputNodes(Nodes ...nodes);
      
      void setInputNodes(const std::vector<ViewInfoPtr>&);
      
      template<typename... Nodes>
      void disconnectInputNodes(Nodes ...nodes);
      
      template<typename... Nodes>
      void getOutputs(Nodes& ...nodes);
      
      const std::vector<core::ViewNode*>& getOutputs();
      
      void disconnectOutputNodes();
      
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
      
    public:
      
      //void getAllParams(int listNo, VEC_NAME()& v) const;
      //void resetAllParams(int listNo, VEC_NAME()& v);
      
      // Manage char* as std::string
      //template<typename... Params>
      //void setParams(std::string pName, const char* p, Params ...params);
      
    private:
      
      void getParams() { }
      void setParams() { }
      
    private:
      
      AlgorithmInfo m_input;
      AlgorithmInfo m_output;
      //ParameterList m_expectedParameters;

      std::string m_defaultXmlParam;
      std::map<std::string, std::string> m_xml_parameters;

    };

/*#undef SET_DEFAULT_GET_SET_LIST
#undef DEFINE_GET_SET_LIST
#undef DEFINE_TYPED_GET_SET_AND_CONTAINER
#undef SET_NAME
#undef GET_NAME
#undef DEFAULT_SET_NAME
#undef DEFAULT_GET_NAME
#undef CONTAINER
#undef ITR_NAME
#undef VEC_NAME
#undef DEFINE_TYPED_GET_SET*/

  }
}

#include "Algorithm.hxx"

#endif /*! SD_FRONTEND_ALGORITHM_HPP */
