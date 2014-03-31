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

#ifndef SD_FRONTEND_ALGORITHM_INFO_HXX
#define SD_FRONTEND_ALGORITHM_INFO_HXX

namespace sd {
  namespace frontend {

    template<typename ...Values>
    bool
    AlgorithmInfo::validate(Values ...values) {
      if (sizeof...(values) != m_info.size())
	return false;
      return checkValue(0, values...);
    }
    /*
    template<typename T>
    bool
    AlgorithmInfo::checkValue(int i, T) {
      return m_info[i].isA<T>();
    }
    */
    bool
    AlgorithmInfo::checkValue(int i, sd::core::ImageView* iv) {
      return iv
	?  (m_info[i].containsA(sd::core::ObjectDescription::ImageView(iv->channelType(),
								      (int) iv->nbDimensions(),
								      (int) iv->nbChannels()))
	    || m_info[i].isA<sd::core::ImageView*>())
	: m_info[i].isA<sd::core::ImageView*>();
    }
    
    bool
    AlgorithmInfo::checkValue(int i, sd::core::MeshView* mv) {
      return mv
	? (m_info[i].containsA(sd::core::ObjectDescription::MeshView(mv->getVertexDataType(),
								     (int) mv->getFaceDegree()))
	   || m_info[i].isA<sd::core::MeshView*>())
	: m_info[i].isA<sd::core::MeshView*>();
    }
    /*
    bool
    AlgorithmInfo::checkValue(int i, sd::core::ObjectDescription value) {
      return m_info[i].isA(value);
    }
    */
    template<typename First, typename ...Rest>
    bool
    AlgorithmInfo::checkValue(int i, First first, Rest ...rest) {
      bool valid = checkValue(i, first);
      if (! valid)
	return false;
      return checkValue(i+1, std::forward<Rest>(rest)...);
    }

    template<typename T>
    void
    AlgorithmInfo::populate(T value) {
      m_info.push_back(Any(value));
    }

    template<typename First, typename ...Rest>
    void
    AlgorithmInfo::populate(First first, Rest ...rest) {
      populate(first);
      populate(std::forward<Rest>(rest)...);
    }

    template<typename ...Info>
    AlgorithmInfo
    make_info(Info ...info) {
      AlgorithmInfo ai;
      if (sizeof...(info) > 0)
	ai.populate(info...);
      return ai;
    }

  }
}

#endif /*! SD_FRONTEND_ALGORITHM_INFO_HXX */
