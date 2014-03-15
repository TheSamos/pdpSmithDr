cmake_minimum_required(VERSION 2.8.0)

set(SMITHDR_PROJECT SmithDR)
project(${SMITHDR_PROJECT})

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# Supress regeneration
set(CMAKE_SUPPRESS_REGENERATION FALSE)

# where to look first for cmake modules,
# before ${CMAKE_ROOT}/Modules is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(SmithDRMacros)

# Usage SMITHDR_OPTION
# 1. parameter: option name without prefix 'SMITHDR_'
# 2. parameter: description
# 3. parameter: default value, ON or OFF
# 4. parameter: system on which option is used: ALL, GCC, ...

SMITHDR_OPTION_INIT()

# Options for all compilers/systems
SMITHDR_OPTION(TESTS "Build tests" OFF ALL)
SMITHDR_OPTION(BUILD_32BIT_COMPATIBLE "Set to ON to build in 32 bit compatibility mode" OFF ALL)
SMITHDR_OPTION(VERBOSE "Generate verbose makefiles" OFF ALL)
SMITHDR_OPTION(CPACK "Use the CPack management" OFF ALL)
SMITHDR_OPTION(SHARED_LIBRARIES "Build shared libraries" ON ALL)

# GCC specific
SMITHDR_OPTION(PROFILING "Enable profiling mode" OFF GCC)

## Set build_type if not set
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
ENDIF()


# Check option dependencies
if(SMITHDR_CPACK)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build release version" FORCE)
  set(SMITHDR_PROFILING OFF)
endif()


message(STATUS)

# Note: on DLL platforms aka Windows, DLLs are classified as runtime targets,
#       i.e. the RUNTIME variables/properties apply.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX OR MINGW)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  message(STATUS "Using GCC version ${GCC_VERSION}")
endif()


if(WIN32)
  set(USE_WINDOWS_PACKAGING 1)
elseif(APPLE)
  set(USE_MACOSX_PACKAGING 1)
else()
  set(USE_POSIX_PACKAGING 1)
endif()


set(SMITHDR_MAJOR_VERSION 2)
set(SMITHDR_MINOR_VERSION 0)
set(SMITHDR_PATCH_VERSION 0)
set(PACKAGE_BASE ${SMITHDR_PROJECT})
set(PACKAGE_VERSION "${SMITHDR_MAJOR_VERSION}.${SMITHDR_MINOR_VERSION}.${SMITHDR_PATCH_VERSION}")
set(PACKAGE_BUGREPORT "smithdr@labri.fr")
set(SMITHDR_DIR_VER "SMITHDR_DIR_${SMITHDR_MAJOR_VERSION}${SMITHDR_MINOR_VERSION}x")
set(SMITHDR_USERDIR_VER "SMITHDR_USERDIR_${SMITHDR_MAJOR_VERSION}${SMITHDR_MINOR_VERSION}x")
set(SMITHDR_INSTALL_SUFFIX "${SMITHDR_MAJOR_VERSION}.${SMITHDR_MINOR_VERSION}")
set(SMITHDR_DATE "2011-07-13")


set(PACKAGE ${PACKAGE_BASE})

set(SMITHDR_ABS_INSTALLED_DATADIR "${CMAKE_INSTALL_PREFIX}")
set(SMITHDR_ABS_TOP_SRCDIR "${CMAKE_SOURCE_DIR}")

# smithdr's source files
set(SMITHDR_C_FILES *.c)
set(SMITHDR_H_FILES *.h)
set(SMITHDR_CPP_FILES *.cpp)
set(SMITHDR_HPP_FILES *.hpp)
set(SMITHDR_HXX_FILES *.hxx)
set(SMITHDR_MOC_FILES moc_*.cpp)


# When shared libs are supported enable this option
if(SMITHDR_SHARED_LIBRARIES)
  set(library_type SHARED)
else()
  set(library_type STATIC)
endif()

set(CMAKE_CXX_FLAGS "")

find_package(ZLIB)
if (ZLIB_FOUND)
  add_definitions(-DHAVE_ZLIB)
endif()
find_package(FFTW3 REQUIRED)
find_package(SharedPtr REQUIRED)

if(HAVE_CPP0X_SHARED_PTR)
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CPP0X_SHARED_PTR" )
elseif(HAVE_TR1_SHARED_PTR)
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_TR1_SHARED_PTR" )
elseif(HAVE_BOOST_TR1_SHARED_PTR)
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_BOOST_TR1_SHARED_PTR" )
elseif(HAVE_BOOST_SHARED_PTR)
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_BOOST_SHARED_PTR" )
endif()

#---------------- 32/64 bit issues ---------------------------------------
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  MESSAGE( STATUS "32 bit architecture detected" )
ENDIF()

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  MESSAGE( STATUS "64 bit architecture detected" )

  IF( BUILD_32BIT_COMPATIBLE )
    IF( COMMAND SET_PROPERTY )
      SET_PROPERTY(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS 0)
      SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
    ELSE()
      MESSAGE( FATAL_ERROR "to build in 32 bit compatibility mode you need cmake >= 2.6" )
    ENDIF()
    MESSAGE( STATUS "Build in 32 bit compatibility mode" )
  ELSE()
    MESSAGE( STATUS "Build in native 64 bit mode" )
  ENDIF()
ENDIF()
#-------------------------------------------------------------------------

if(NOT MSVC)
  if(SMITHDR_VERBOSE)
    set(CMAKE_VERBOSE_MAKEFILE ON)
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused-parameter -pedantic -fno-deduce-init-list")

  if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-stdcall-fixup")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--enable-stdcall-fixup")
  endif()

  if((UNIX OR MINGW) AND NOT GCC_VERSION VERSION_LESS 4.3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" CACHE TYPE STRING FORCE)
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing" CACHE TYPE STRING FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG " CACHE TYPE STRING FORCE) ##-Weffc++ -Wshadow -Wswitch-default -Wswitch-enum -Warray-bounds -Wconversion -Wlogical-op -Winit-self -Wmissing-include-dirs
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG" CACHE TYPE STRING FORCE)

  if(SMITHDR_PROFILING)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg" CACHE TYPE STRING FORCE)
  endif()
endif()
set(CMAKE_CXX_FLAGS "")
#-------------------------------------------------------------------------

IF(UNIX)
    IF (APPLE)
        ADD_DEFINITIONS(-DAPPLE)
    ELSE(APPLE)
        # Add nothing
    ENDIF(APPLE)
ELSE(UNIX)
    IF(WIN32)
        ADD_DEFINITIONS(-DWIN32)
     ELSE(WIN32)
        MESSAGE(STATUS "Unknown platform")
    ENDIF(WIN32)
ENDIF(UNIX)

if(WIN32)
  set(WIN32_CONSOLE WIN32)
  set(SMITHDR_QTMAIN_LIBRARY ${QT_QTMAIN_LIBRARY})
  add_definitions(-DWINVER=0x0500)
endif()

message(STATUS "")
foreach(_v PACKAGE PACKAGE_VERSION
    SMITHDR_DATE SMITHDR_DIR_VER SMITHDR_USERDIR_VER
    SMITHDR_ABS_TOP_SRCDIR
    SMITHDR_ABS_INSTALLED_DATADIR)
  if(NOT DEFINED ${_v})
    message(FATAL_ERROR "${_v} not defined")
  endif()
  string(SUBSTRING "${_v}                            " 0 28 _v1)
  message(STATUS "----- ${_v1}: ${${_v}}")
endforeach(_v)
message(STATUS "")


# config.h generation
configure_file(cmake/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

# create uninstall configuration file 
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)

# create uninstall target
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#set(SMITHDR_PLUGIN_DIR ${SmithDR_BINARY_DIR}/plugins)
set(SMITHDR_PLUGIN_DIR ${SmithDR_BINARY_DIR}/lib)
add_definitions(-DPLUGIN_DIR=\"${SMITHDR_PLUGIN_DIR}\")

set(SMITHDR_ICON_PATH ${SmithDR_SOURCE_DIR}/resources/icons)
add_definitions(-DSD_ICON_PATH=\"${SMITHDR_ICON_PATH}\")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/eigen)


#add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(plugins)

if (SMITHDR_TESTS)
  add_subdirectory(tests)
endif()


message(STATUS)
message(STATUS "Build options, switch SMITHDR_* variables by -DSMITHDR_*=1 or 0:")
message(STATUS)
SMITHDR_OPTION_LIST_ALL()


#---------------- CPack --------------------------------------------------
set(CPACK_BUNDLE_NAME "SmithDR")
set(CPACK_PACKAGE_NAME "smithdr")
set(CPACK_PACKAGE_VENDOR "LaBRI")
set(CPACK_PACKAGE_DESCRIPTION_SUMARY "SmithDR - A tool for data processing")
set(CPACK_PACKAGE_VERSION_MAJOR "${SMITHDR_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${SMITHDR_MINOR_VERSION}")

# Find the revision number and use it as the release in rpm-package-build.
# This way we may omit the otherwise needed "--force" parameter when
# installing from that rpm package.
FIND_PROGRAM(SMITHDR_SVNVERSION svnversion)
if(${SMITHDR_SVNVERSION} MATCHES "-NOTFOUND")
  set(CPACK_PACKAGE_PATCH_MINOR "${SMITHDR_PATCH_VERSION}")
else()
  EXECUTE_PROCESS(COMMAND ${SMITHDR_SVNVERSION} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE CPACK_RPM_PACKAGE_RELEASE OUTPUT_STRIP_TRAILING_WHITESPACE)
  # We use this value also to set the package-patch-value
  if(CPACK_RPM_PACKAGE_RELEASE MATCHES "^\([0-9]+\)")
    set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_1})
  endif()
endif()

set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

# Tell CPack all of the components to install. The "ALL"
# refers to the fact that this is the set of components that
# will be included when CPack is instructed to put everything
# into the binary installer (the default behavior)
set(CPACK_COMPONENTS_ALL applications libraries headers tests plugins)

# Set the displayed names for each of the components to install.
# These will be displayed in the list of components inside the installer.
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "SmithDR application")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "SmithDR libraries")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "SmithDR headers")
set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "SmithDR tests")
set(CPACK_COMPONENT_PLUGINS_DISPLAY_NAME "SmithDR plugins")

# Provide descriptions for each of the components to install.
# When the user hovers the mouse over the name of a component,
# the description will be shown in the "Description" box in the
# installer. If no descriptions are provided, the "Description"
# box will be removed.
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Qt-based GUI for SmithDR application")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Libraries used to build programs with SmithDR")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "C++ header files for use with SmithDR")
set(CPACK_COMPONENT_TESTS_DESCRIPTION "C++ test files of SmithDR")
set(CPACK_COMPONENT_PLUGINS_DESCRIPTION "Plugins for use with SmithDR")

# Put the components into two diferent groups: "Runtime" and "Development"
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_COMPONENT_TESTS_GROUP "Development")
set(CPACK_COMPONENT_PLUGINS_GROUP "Development")

# Expand the "Development" group by default, since we have so few components.
# Also, provide this group with a description.
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_EXPANDED ON)
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "Tools to develop with SmithDR")

# It doesn't make sense to install the headers without the libraries
# (because you could nevers use the headers!), so make the headers component
# depend on the libraries component.
set(CPACK_COMPONENT_HEADERS_DEPEND libraries)
set(CPACK_COMPONENT_TESTS_DEPEND libraries)
set(CPACK_COMPONENT_PLUGINS_DEPEND libraries)

# Create two installation types with pre-selected components.
# The "Developer" installation has just the library and headers,
# while the "Full" installation has everything.
set(CPACK_ALL_INSTALL_TYPES Full Developer)
set(CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Complete")
set(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_TESTS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_PLUGINS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)

if(APPLE)
  set(CPACK_BINARY_DRAGNDROP ON)
  set(CPACK_GENERATOR DragNDrop)
endif()

if(SMITHDR_CPACK)
  include(CPack)
endif()
#-------------------------------------------------------------------------

mark_as_advanced(QT_QMAKE_EXECUTABLE)
mark_as_advanced(SMITHDR_SVNVERSION)

message(STATUS)
