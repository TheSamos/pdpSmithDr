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

#include "TypesInfo.hpp"

#include <cassert>

#include "ViewManager.hpp"

namespace sd {
  
  namespace frontend {

    ViewInfo::ViewInfo(ViewType type,
		       const std::string& name,
		       bool persistent)
      : m_name(name),
	m_refCounter(0),
	m_persistent(persistent),
	m_viewType(type)
    {}

    ViewInfo::~ViewInfo() {
      if (m_persistent)
	ViewManager::instance().viewDeleted(this);
    }

    std::string&
    ViewInfo::getName() {
      return m_name;
    }

    const std::string&
    ViewInfo::getName() const {
      return m_name;
    }

    ViewInfo::ViewType
    ViewInfo::getViewType() const {
      return m_viewType;
    }

    void
    ViewInfo::setName(const std::string& name) {
      m_name = name;
    }

#define DEFINE_TYPE2(TypeName, TypePtr,					\
		     TypeInfo, TypeInfoPtr,				\
		     NamePtr, FileInfo, FileInfoPtr,			\
		     AlgorithmInfo, AlgorithmInfoPtr)			\
    TypeInfo::TypeInfo(OriginType type, const std::string& name,	\
		       TypePtr data, bool persistent)			\
      : ViewInfo(ViewInfo::TypeName, name, persistent),			\
	NamePtr(data),							\
	m_originType(type) {						\
      assert(NamePtr != 0);						\
    }									\
									\
    TypeInfo::~TypeInfo() {						\
    }									\
									\
    TypeInfo::OriginType						\
    TypeInfo::getOriginType() const {					\
      return m_originType;						\
    }									\
    									\
    FileInfo::FileInfo(const std::string& fpath, TypePtr data)		\
      : TypeInfo(TypeInfo::File,					\
		 fpath.substr(fpath.find_last_of('/')+1), data) {}	\
									\
    AlgorithmInfo::AlgorithmInfo(const std::string& name,		\
				 TypePtr data,				\
				 bool persistent)			\
      : TypeInfo(TypeInfo::AlgorithmOutput, name, data, persistent) {}
    
#define SD_STRINGIFY(x, y) x##y
#define SD_CONCAT(x, y) SD_STRINGIFY(x, y)
    
#define DEFINE_TYPE1(TypeName, TypePtr,					\
		     TypeInfo, NamePtr,					\
		     FileInfo, AlgorithmInfo)				\
    DEFINE_TYPE2(TypeName,						\
		 TypePtr,						\
		 TypeInfo, SD_CONCAT(TypeInfo, Ptr),			\
		 NamePtr,						\
		 FileInfo, SD_CONCAT(FileInfo, Ptr),			\
		 AlgorithmInfo, SD_CONCAT(AlgorithmInfo, Ptr))
    
    
#define DEFINE_TYPE(TypeName, DataName)					\
    DEFINE_TYPE1(SD_CONCAT(TypeName, Type),				\
		 SD_CONCAT(TypeName, Ptr),				\
		 SD_CONCAT(TypeName, Info),				\
		 SD_CONCAT(DataName, Ptr),				\
		 SD_CONCAT(File, SD_CONCAT(TypeName, Info)),		\
		 SD_CONCAT(AlgorithmOutput, SD_CONCAT(TypeName, Info)))
    
    DEFINE_TYPE(Mesh,  mesh)
    DEFINE_TYPE(Image, image)

  }
  
}
