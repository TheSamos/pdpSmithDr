# - Try to find SmithDR
# Once done this will define
#
#  SMITHDR_FOUND - system has SmithDR
#  SMITHDR_INCLUDE_DIRS - the SmithDR include directory
#  SMITHDR_LIBRARIES - Link these to use SmithDR
#  SMITHDR_DEFINITIONS - Compiler switches required for using SmithDR

if (SMITHDR_LIBRARIES AND SMITHDR_INCLUDE_DIRS)
  # in cache already
  set(SMITHDR_FOUND TRUE)
else (SMITHDR_LIBRARIES AND SMITHDR_INCLUDE_DIRS)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  if (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    include(UsePkgConfig)
    pkgconfig(SmithDR _SMITHDR_INCLUDEDIR _SMITHDR_LIBDIR _SMITHDR_LDFLAGS _SMITHDR_CFLAGS)
  else (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
      pkg_check_modules(_SMITHDR SmithDR)
    endif (PKG_CONFIG_FOUND)
  endif (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
  find_path(SMITHDR_INCLUDE_DIR
    NAMES
    core/SmithDRDefs.hpp
    PATHS
    ${_SMITHDR_INCLUDEDIR}
    ${TOP_SRC_DIR}/SmithDR
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
    )

  find_library(SMITHDR_LIBRARY
    NAMES
    sdcore sdio
    PATHS
    ${_SMITHDR_LIBDIR}
    ${CMAKE_BINARY_DIR}/lib
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
    )

  if (SMITHDR_LIBRARY)
    set(SMITHDR_FOUND TRUE)
  endif (SMITHDR_LIBRARY)

  set(SMITHDR_INCLUDE_DIRS
    ${SMITHDR_INCLUDE_DIR}
    )

  if (SMITHDR_FOUND)
    set(SMITHDR_LIBRARIES
      ${SMITHDR_LIBRARIES}
      ${SMITHDR_LIBRARY}
      )
  endif (SMITHDR_FOUND)

  if (SMITHDR_INCLUDE_DIRS AND SMITHDR_LIBRARIES)
    set(SMITHDR_FOUND TRUE)
  endif (SMITHDR_INCLUDE_DIRS AND SMITHDR_LIBRARIES)

  if (SMITHDR_FOUND)
    if (NOT SMITHDR_FIND_QUIETLY)
      message(STATUS "Found SmithDR: ${SMITHDR_LIBRARY}")
    endif (NOT SMITHDR_FIND_QUIETLY)
  else (SMITHDR_FOUND)
    if (SMITHDR_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find SMITHDR")
    endif (SMITHDR_FIND_REQUIRED)
  endif (SMITHDR_FOUND)

  # show the SMITHDR_INCLUDE_DIRS and SMITHDR_LIBRARIES variables only in the advanced view
  mark_as_advanced(SMITHDR_INCLUDE_DIRS SMITHDR_LIBRARIES)

endif (SMITHDR_LIBRARIES AND SMITHDR_INCLUDE_DIRS)

