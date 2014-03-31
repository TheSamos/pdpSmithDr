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

#ifndef SD_CORE_IMAGE_VIEW_HXX
#define SD_CORE_IMAGE_VIEW_HXX

#include <cassert>
#include <utils/sdTypeUtils.hpp>

namespace sd {
  
  namespace core {
    
    inline sd::core::ImageView::ImageView(sd::core::ViewNode* previousViewNode, const std::string& name)
      : ViewNode(ImageObjectType, previousViewNode, name) {}
    
    inline sd::core::ImageView::ImageView(std::vector<sd::core::ViewNode*>& previousViewNodes,
					  const std::string& name)
      : ViewNode(ImageObjectType, previousViewNodes, name) {}
    
    inline sd::core::ImageView::~ImageView() {}
    
    inline Size
    ImageView::size() const {
      return information().size();
    }

    inline UINT8
    ImageView::nbChannels() const {
      return information().nbChannels();
    }

    inline ColorSpace
    ImageView::colorSpace() const {
      return information().colorSpace();
    }

    inline Orientation
    ImageView::orientation() const {
      return information().orientation();
    }

    inline Resolution
    ImageView::resolution() const {
      return information().resolution();
    }

    const std::string&
    ImageView::spaceUnit() const {
      return information().spaceUnit();
    }

    const std::string&
    ImageView::timeUnit() const {
      return information().timeUnit();
    }

    inline bool
    ImageView::hasAdditionalParameter(const std::string& name) const {
      return m_additionalParameters.find(name) != m_additionalParameters.end();
    }

    inline std::string
    ImageView::getAdditionalParameter(const std::string& name) const {
      auto it = m_additionalParameters.find(name);
      return (it != m_additionalParameters.end()) ? it->second : "";
    }

    inline void
    ImageView::setAdditionalParameter(const std::string& name, const std::string& value) {
      m_additionalParameters[name] = value;
    }

  }
  
}

#endif /*! SD_CORE_IMAGE_VIEW_HXX */
