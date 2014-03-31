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

#ifndef SD_FRONTEND_TYPESINFO_HPP
#define SD_FRONTEND_TYPESINFO_HPP

#include <string>

#include <SmartPointers.hpp>

#include <core/image/ImageView.hpp>
#include <core/mesh/MeshView.hpp>

namespace sd {
  
  namespace frontend {

    class ViewInfo {

    public:

      enum ViewType { MeshType = MeshObjectType,
		      ImageType = ImageObjectType,
		      UndefinedType };

    public:
      
      virtual ~ViewInfo();

      std::string& getName();
      const std::string& getName() const;
      ViewType getViewType() const;
      
    protected:

      ViewInfo(ViewType type, const std::string& name, bool persistent = true);
      
    private:

      friend class ViewManager;
      void setName(const std::string& name);
      
      friend inline void
      intrusive_ptr_add_ref(ViewInfo* self) {
	++self->m_refCounter;
      }

      friend inline void
      intrusive_ptr_release(ViewInfo* self) {
	if (!--self->m_refCounter)
	  delete self;
      }

    private:

      std::string m_name;
      size_t m_refCounter;
      bool m_persistent;
      const ViewType m_viewType;

    };

    typedef sd::intrusive_ptr<ViewInfo> ViewInfoPtr;

#define DECLARE_TYPE2(TypeView, TypeName, TypePtr,			\
		      TypeInfo, TypeInfoPtr,				\
		      NamePtr,						\
		      FileTypeInfo, FileTypeInfoPtr,			\
		      AlgorithmTypeInfo, AlgorithmTypeInfoPtr		\
		      )							\
    typedef sd::shared_ptr<TypeView> TypePtr;				\
									\
    class TypeInfo : public ViewInfo {					\
									\
    public:								\
									\
      enum OriginType { File, AlgorithmOutput };			\
									\
    public:								\
    									\
      const TypePtr NamePtr;						\
      									\
    public:								\
    									\
      virtual ~TypeInfo();						\
      OriginType getOriginType() const;					\
      									\
    protected:								\
    									\
    TypeInfo(OriginType type, const std::string& name,			\
	     TypePtr data, bool persistent = true);			\
    									\
    private:								\
    									\
      const OriginType m_originType;					\
      									\
    };									\
    									\
    typedef sd::intrusive_ptr<TypeInfo> TypeInfoPtr;			\
    									\
    class FileTypeInfo : public TypeInfo {				\
      									\
    public:								\
    									\
      FileTypeInfo(const std::string& fpath, TypePtr data);		\
      									\
    };									\
									\
    typedef sd::intrusive_ptr<FileTypeInfo> FileTypeInfoPtr;		\
    									\
									\
    class AlgorithmTypeInfo : public TypeInfo {				\
      									\
    public:								\
    									\
    AlgorithmTypeInfo(const std::string& name,				\
		      TypePtr data, bool persistent = true);		\
									\
    };									\
    									\
    typedef sd::intrusive_ptr<AlgorithmTypeInfo> AlgorithmTypeInfoPtr;
    
#define SD_STRINGIFY(x, y) x##y
#define SD_CONCAT(x, y) SD_STRINGIFY(x, y)

#define DECLARE_TYPE1(TypeView, TypeName, TypePtr, TypeInfo, NamePtr,	\
		      FileTypeInfo, AlgorithmTypeInfo)			\
    DECLARE_TYPE2(TypeView,						\
		  TypeName,						\
		  TypePtr,						\
		  TypeInfo, SD_CONCAT(TypeInfo, Ptr),			\
		  NamePtr,						\
		  FileTypeInfo, SD_CONCAT(FileTypeInfo, Ptr),		\
		  AlgorithmTypeInfo, SD_CONCAT(AlgorithmTypeInfo, Ptr))
    
#define DECLARE_TYPE(BaseType, TypeName, DataName)			\
    DECLARE_TYPE1(BaseType,						\
		  SD_CONCAT(TypeName, Type),				\
		  SD_CONCAT(TypeName, Ptr),				\
		  SD_CONCAT(TypeName, Info),				\
		  SD_CONCAT(DataName, Ptr),				\
		  SD_CONCAT(File, SD_CONCAT(TypeName, Info)),		\
		  SD_CONCAT(AlgorithmOutput, SD_CONCAT(TypeName, Info)))
    
    DECLARE_TYPE(core::MeshView,  Mesh,  mesh)
    DECLARE_TYPE(core::ImageView, Image, image)
    
#undef DECLARE_TYPE
#undef DECLARE_TYPE1
#undef DECLARE_TYPE2
    
  }
  
}

#endif /*! SD_FRONTEND_TYPESINFO_HPP */
