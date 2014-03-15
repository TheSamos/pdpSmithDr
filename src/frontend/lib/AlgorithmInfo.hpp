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

#ifndef SD_FRONTEND_ALGORITHM_INFO_HPP
#define SD_FRONTEND_ALGORITHM_INFO_HPP

#include <SmithDRDefs.hpp>
#include "Any.hpp"
#include <vector>

namespace sd {
  
  namespace frontend {

    class AlgorithmInfo {
      
    public:
      
      AlgorithmInfo() : m_info() {}
      
      ~AlgorithmInfo()
      {}
      
      template<typename ...Values> bool validate(Values ...values);
      
      const std::vector<Any>& get() const {return m_info;}
      
    private:
      
      void populate() {}
      template<typename T> void populate(T value);
      template<typename First, typename ...Rest> void populate(First first, Rest ...rest);

      bool checkValue(int) { return true; }
      //template<typename T> bool checkValue(int i, T value);
      inline bool checkValue(int i, sd::core::ImageView* iv);
      inline bool checkValue(int i, sd::core::MeshView* mv);
      // inline bool checkValue(int i, sd::core::ObjectDescription value);
      template<typename First, typename ...Rest> bool checkValue(int i, First first, Rest ...rest);

      template<typename ...Info> friend AlgorithmInfo make_info(Info ...info);

    private:

      std::vector<Any> m_info;

    };

  }
  
}

#include "AlgorithmInfo.hxx"

#endif /* ! SD_FRONTEND_ALGORITHM_INFO_HPP */
