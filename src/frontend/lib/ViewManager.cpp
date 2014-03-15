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

#include "ViewManager.hpp"

#include <cassert>
#include <sstream>

#include "LoaderManager.hpp"

namespace sd {
  namespace frontend {

    ViewManager::ViewManager()
      : m_views(),
	m_viewCreationCallbacks(),
	m_viewDeletionCallbacks(),
	m_viewModificationCallbacks()
    {
    }

    ViewManager::~ViewManager()
    {
    }

    bool
    ViewManager::createView(const std::string& fpath, std::string& errMsg)
    {
      auto it = m_views.find(fpath);
      if (it != m_views.end()) {
	errMsg = "File already opened";
	return false;
      }
      
      auto loader = frontend::detectLoaders(fpath).first();
      if (!loader)
	return false;
      
      Loader::Type type = loader->loaderType();
      ViewInfo* info = 0;
      switch (type) {
      case Loader::ImageLoader: {
	core::ImageView* iv = NULL;
	bool loaded = frontend::loadFile(fpath, iv);
	if (!loaded || !iv)
	  return false;
	ImagePtr imagePtr(iv);
	info = new FileImageInfo(fpath, imagePtr);
      }
	break;
      case Loader::MeshLoader: {
	core::MeshView* mv = NULL;
	bool loaded = frontend::loadFile(fpath, mv);
	if (!loaded || !mv)
	  return false;
	MeshPtr meshPtr(mv);
	info = new FileMeshInfo(fpath, meshPtr);
      }
	break;
      case Loader::Unknown:
      default:
	assert(!"Should never get here!");
      }
      m_views[fpath] = info;
      notifyCreationCallbacks(info);
      
      return true;
    }
    
    template<typename T>
    std::string
    stringOf(const T& val)
    {
      std::ostringstream os;
      os << val;
      return os.str();
    }

    std::string
    uniqueName(const ViewManager::ViewsList& views, const std::string& name)
    {
      std::string uniqueName = name + " output 1";

      size_t i = 0;
      auto it = views.begin();
      while (it != views.end()) {
	++i;
	uniqueName = name + " output " + stringOf(i);
	it = views.find(uniqueName);
      }

      return uniqueName;
    }

    ViewInfoPtr
    ViewManager::createViewInfo(core::ViewNode* node, const std::string& name,
				bool persistent)
    {
      ViewInfoPtr info = 0;
      if (dynamic_cast<core::ImageView*>(node)) {
	ImagePtr imagePtr(dynamic_cast<core::ImageView*>(node));
	info = new AlgorithmOutputImageInfo(name, imagePtr, persistent);
      }
      else if (dynamic_cast<core::MeshView*>(node)) {
	MeshPtr meshPtr(dynamic_cast<core::MeshView*>(node));
	info = new AlgorithmOutputMeshInfo(name, meshPtr, persistent);
      }
      return info;
    }

    bool
    ViewManager::createView(core::ViewNode* node, std::string& errMsg)
    {
      std::string name = uniqueName(m_views, node->name());
      ViewInfoPtr info = createViewInfo(node, name);

      if (!info) {
	errMsg = "Unable to create view";
	return false;
      }

      m_views[info->getName()] = info.get();
      notifyCreationCallbacks(info.get());
      return true;
    }

    const ViewManager::ViewsList&
    ViewManager::getViews() const
    {
      return m_views;
    }

    bool
    ViewManager::changeViewName(frontend::ViewInfo* view, const std::string& newName)
    {
      auto it = m_views.find(newName);
      if (it != m_views.end())
	return false;

      for (it = m_views.begin(); it != m_views.end(); ++it) {
	if (it->second == view) {
	  m_views[newName] = view;
	  view->setName(newName);
	  viewModified(view);
	}
      }

      return true;
    }

    ViewManager::CallbackHandle
    ViewManager::registerViewCreationCallback(const ViewMonitoringCallback& callback)
    {
      m_viewCreationCallbacks.push_front(callback);
      return m_viewCreationCallbacks.begin();
    }

    ViewManager::CallbackHandle
    ViewManager::registerViewDeletionCallback(const ViewMonitoringCallback& callback)
    {
      m_viewDeletionCallbacks.push_front(callback);
      return m_viewDeletionCallbacks.begin();
    }

    ViewManager::CallbackHandle
    ViewManager::registerViewModificationCallback(const ViewMonitoringCallback& callback)
    {
      m_viewModificationCallbacks.push_front(callback);
      return m_viewModificationCallbacks.begin();
    }

    void
    ViewManager::unregisterViewCreationCallback(const ViewManager::CallbackHandle& callbackHandle)
    {
      m_viewCreationCallbacks.erase(callbackHandle);
    }

    void
    ViewManager::unregisterViewDeletionCallback(const ViewManager::CallbackHandle& callbackHandle)
    {
      m_viewDeletionCallbacks.erase(callbackHandle);
    }

    void
    ViewManager::unregisterViewModificationCallback(const ViewManager::CallbackHandle& callbackHandle)
    {
      m_viewModificationCallbacks.erase(callbackHandle);
    }

    void
    ViewManager::notifyCreationCallbacks(ViewInfo* viewInfo) const
    {
      for (auto it = m_viewCreationCallbacks.begin(), end = m_viewCreationCallbacks.end(); it != end; ++it) {
	(*it)(viewInfo);
      }
    }

    void
    ViewManager::notifyDeletionCallbacks(ViewInfo* viewInfo) const
    {
      for (auto it = m_viewDeletionCallbacks.begin(), end = m_viewDeletionCallbacks.end(); it != end; ++it) {
	(*it)(viewInfo);
      }
    }

    void
    ViewManager::notifyModificationCallbacks(ViewInfo* viewInfo) const
    {
      for (auto it = m_viewModificationCallbacks.begin(), end = m_viewModificationCallbacks.end(); it != end; ++it) {
	(*it)(viewInfo);
      }
    }

    void
    ViewManager::viewDeleted(ViewInfo* self)
    {
      for (auto it = m_views.begin(), end = m_views.end(); it != end; ++it) {
	if (it->second == self) {
	  m_views.erase(it);
	  notifyDeletionCallbacks(self);
	  return;
	}
      }
      assert(!"Should never get here!");
    }

    void
    ViewManager::viewModified(ViewInfo* self)
    {
      notifyModificationCallbacks(self);
    }

  }
}
