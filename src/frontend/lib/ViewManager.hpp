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

#ifndef SD_FRONTEND_VIEWMANAGER_HPP
#define SD_FRONTEND_VIEWMANAGER_HPP

#include <functional>
#include <list>
#include <string>

#include <core/ViewNode.hpp>
#include "TypesInfo.hpp"

namespace sd {
  namespace frontend {

    class ViewManager
    {

    public:

      typedef std::map<std::string, ViewInfo*> ViewsList;

      typedef std::function<void (ViewInfo*)> ViewMonitoringCallback;
      typedef std::list<ViewMonitoringCallback> ViewMonitoringCallbackSet;
      typedef ViewMonitoringCallbackSet::iterator CallbackHandle;

    public:

      static inline ViewManager&
      instance()
      {
	static ViewManager inst;
	return inst;
      }

      const ViewsList& getViews() const;

      bool changeViewName(ViewInfo* view, const std::string& newName);

      // On success: returns true
      // On warning: returns true and set errMsg to the warning message
      // On error:   returns false and set errMsg to the error message
      bool createView(const std::string& fpath, std::string& errMsg);
      bool createView(core::ViewNode* node, std::string& errMsg);

      ViewInfoPtr createViewInfo(core::ViewNode* node, const std::string& name = "",
				 bool persistent = true);

      CallbackHandle registerViewCreationCallback(const ViewMonitoringCallback& callback);
      CallbackHandle registerViewDeletionCallback(const ViewMonitoringCallback& callback);
      CallbackHandle registerViewModificationCallback(const ViewMonitoringCallback& callback);
      void unregisterViewCreationCallback(const CallbackHandle& callbackHandle);
      void unregisterViewDeletionCallback(const CallbackHandle& callbackHandle);
      void unregisterViewModificationCallback(const CallbackHandle& callbackHandle);

    private:

      ViewManager();
      ~ViewManager();
      ViewManager(const ViewManager&) = delete;
      ViewManager& operator=(const ViewManager&) = delete;

      void notifyCreationCallbacks(ViewInfo* viewInfo) const;
      void notifyDeletionCallbacks(ViewInfo* viewInfo) const;
      void notifyModificationCallbacks(ViewInfo* viewInfo) const;

    private:

      friend class ViewInfo;
      void viewDeleted(ViewInfo* self);
      void viewModified(ViewInfo* self);

    private:

      ViewsList m_views;

      ViewMonitoringCallbackSet m_viewCreationCallbacks;
      ViewMonitoringCallbackSet m_viewDeletionCallbacks;
      ViewMonitoringCallbackSet m_viewModificationCallbacks;

    };

  }
}

#endif /*! SD_FRONTEND_VIEWMANAGER_HPP */
