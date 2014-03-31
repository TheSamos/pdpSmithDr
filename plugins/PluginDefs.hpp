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

#ifndef PLUGINDEFS_HPP
#define PLUGINDEFS_HPP
// From: http://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define SMITHDR_PLUGIN_HELPER_DLL_IMPORT __declspec(dllimport)
  #define SMITHDR_PLUGIN_HELPER_DLL_EXPORT __declspec(dllexport)
  #define SMITHDR_PLUGIN_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define SMITHDR_PLUGIN_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
    #define SMITHDR_PLUGIN_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
    #define SMITHDR_PLUGIN_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define SMITHDR_PLUGIN_HELPER_DLL_IMPORT
    #define SMITHDR_PLUGIN_HELPER_DLL_EXPORT
    #define SMITHDR_PLUGIN_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define SMITHDR_PLUGIN_API and SMITHDR_PLUGIN_LOCAL.
// SMITHDR_PLUGIN_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// SMITHDR_PLUGIN_LOCAL is used for non-api symbols.

#ifdef SMITHDR_PLUGIN_DLL_EXPORTS // defined if we are building the SMITHDR_PLUGIN DLL (instead of using it)
#define SMITHDR_PLUGIN_API SMITHDR_PLUGIN_HELPER_DLL_EXPORT
#else
#define SMITHDR_PLUGIN_API SMITHDR_PLUGIN_HELPER_DLL_IMPORT
#endif // SMITHDR_PLUGIN_DLL_EXPORTS
#define SMITHDR_PLUGIN_LOCAL SMITHDR_PLUGIN_HELPER_DLL_LOCAL


#endif /*PLUGINDEFS_HPP*/
