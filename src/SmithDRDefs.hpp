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

#ifndef SMITHDRDEFS_HPP
#define SMITHDRDEFS_HPP
// From: http://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define SMITHDR_HELPER_DLL_IMPORT __declspec(dllimport)
#define SMITHDR_HELPER_DLL_EXPORT __declspec(dllexport)
#define SMITHDR_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define SMITHDR_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
#define SMITHDR_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
#define SMITHDR_HELPER_DLL_LOCAL __attribute__ ((visibility("hidden")))
#else
#define SMITHDR_HELPER_DLL_IMPORT
#define SMITHDR_HELPER_DLL_EXPORT
#define SMITHDR_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define SMITHDR_API and SMITHDR_LOCAL.
// SMITHDR_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// SMITHDR_LOCAL is used for non-api symbols.

#ifdef SMITHDR_DLL_EXPORTS // defined if we are building the SMITHDR DLL (instead of using it)
#define SMITHDR_API SMITHDR_HELPER_DLL_EXPORT
#else
#define SMITHDR_API SMITHDR_HELPER_DLL_IMPORT
#endif // SMITHDR_DLL_EXPORTS
#define SMITHDR_LOCAL SMITHDR_HELPER_DLL_LOCAL


/************************************************/
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/************************************************/


/************************************************/
#define DO_PRAGMA(x) _Pragma(#x)

#define NO_QT_WARNINGS()			\
  DO_PRAGMA(GCC diagnostic ignored "-Weffc++")
  DO_PRAGMA(GCC diagnostic ignored "-Wconversion")

#define RESTORE_CONTEXT()
/************************************************/


#endif /*SMITHDRDEFS_HPP*/
