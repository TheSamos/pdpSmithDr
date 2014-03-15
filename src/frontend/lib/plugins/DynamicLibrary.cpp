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

#ifdef WIN32
  #include <Windows.h>
#else
  #include <dlfcn.h>
#endif

#include "DynamicLibrary.hpp"

#include <sstream>
#include <iostream>

namespace sd {
  namespace frontend {
    namespace plugins {

      DynamicLibrary::DynamicLibrary(void* handle)
	: m_handle(handle)
      {
      }

      DynamicLibrary::~DynamicLibrary()
      {
#ifdef DEBUG
	std::cout << "Closing dll\n";
#endif
	if (m_handle) {
#ifndef WIN32
	  ::dlclose(m_handle);
#else
	  ::FreeLibrary((HMODULE) m_handle);
#endif
	}
      }

      DynamicLibrary*
      DynamicLibrary::load(const std::string& name, std::string& errorString)
      {
	if (name.empty()) {
	  errorString = "Empty path.";
	  return NULL;
	}

	void* handle = NULL;

#ifdef WIN32
	handle = ::LoadLibraryA(name.c_str());
	if (handle == NULL) {
	  DWORD errorCode = ::GetLastError();
	  std::stringstream ss;
	  ss << std::string("LoadLibrary(") << name
	     << std::string(") Failed. errorCode: ")
	     << errorCode;
	  errorString = ss.str();
	}
#else
	handle = ::dlopen(name.c_str(), RTLD_NOW);
	if (!handle) {
	  std::string dlErrorString;
	  const char *zErrorString = ::dlerror();
	  if (zErrorString)
	    dlErrorString = zErrorString;
	  errorString += "Failed to load \"" + name + '"';
	  if (dlErrorString.size())
	    errorString += ": " + dlErrorString;
	  return NULL;
	}
#endif

#ifdef DEBUG
	std::cout << "Dll opened\n";
#endif
	return new DynamicLibrary(handle);
      }

      void*
      DynamicLibrary::getSymbol(const std::string& symbol)
      {
	if (!m_handle)
	  return NULL;
#ifdef WIN32
        return reinterpret_cast<void*>(::GetProcAddress((HMODULE) m_handle, symbol.c_str()));
#else
	return ::dlsym(m_handle, symbol.c_str());
#endif
      }

    }
  }
}
