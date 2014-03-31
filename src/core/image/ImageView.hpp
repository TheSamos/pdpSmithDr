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

#ifndef SD_CORE_IMAGE_VIEW_HPP
#define SD_CORE_IMAGE_VIEW_HPP

#include <map>
#include <string>
#include <typeinfo>

#include "ImageDataType.hpp"
#include "ImageViewInfo.hpp"

#include <Types.hpp>
#include <core/DataType.hpp>
#include <core/ViewNode.hpp>

namespace sd {
  namespace core {

    class ImageView : public ViewNode {

    public:

      virtual inline ~ImageView();

      virtual const ImageViewInfo& information() const = 0;

      inline Size size() const;
      inline uint nbDimensions() const {return size().nbDims();}
      inline UINT8 nbChannels() const;
      inline ColorSpace colorSpace() const;
      inline Orientation orientation() const;
      inline Resolution resolution() const;

      inline const std::string& spaceUnit() const;
      inline const std::string& timeUnit() const;

      virtual DataType channelType() const = 0;

      inline bool hasAdditionalParameter(const std::string& name) const;
      inline std::string getAdditionalParameter( const std::string& name) const;
      inline void setAdditionalParameter(const std::string& name, const std::string& value);

    protected:
      
      inline ImageView(ViewNode* previousViewNode, const std::string& name);
      inline ImageView(std::vector<ViewNode*>& previousViewNodes, const std::string& name);
      
    private:

      ImageView(const ImageView&) = delete;
      ImageView& operator=(const ImageView&) = delete;

    protected:

      std::map<std::string, std::string> m_additionalParameters;

    };

  }
}

#include "ImageView.hxx"

#endif /*! SD_CORE_IMAGE_VIEW_HPP */
