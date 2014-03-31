set(sdfrontendlib "sdfrontendlib")
project(${sdfrontendlib})

find_package(Qt4 REQUIRED)

set(QT_USE_QTOPENGL TRUE) 
include(${QT_USE_FILE})
set(CMAKE_PACKAGE_QTGUI TRUE)
set(CMAKE_PACKAGE_QTXML TRUE)
#############
set(QT_USE_QTXML TRUE)
set(QT_USE_QTXMLPATTERNS TRUE)
set(QT_USE_QTWEBKIT TRUE)
################

add_definitions(-DUNICODE)

# Les Q_ASSERT, Q_ASSERT_X, ... sont désactivés en mode Release

if(CMAKE_BUILD_TYPE STREQUAL "Release") 
    add_definitions(-DQT_NO_DEBUG)
endif()
# Include utilisé par Qt (du style include (UseQt4))

include(${QT_USE_FILE})
# Ajoute les définitions propres à Qt (suivant les modules ajoutés)

add_definitions(${QT_DEFINITIONS})





file(GLOB_RECURSE _sdfrontendlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE _sdfrontendlib_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdfrontendlib_sources "${_sdfrontendlib_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdfrontendlib_headers "${_sdfrontendlib_headers}")

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

if ("${sdfrontendlib_sources}" STREQUAL "")
  set(dummyFile "sdDummy.cpp")
  FILE(WRITE ${dummyFile} "int main(){return 0;}")
  set(sdfrontendlib_sources ${dummyFile})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/plugins)

add_subdirectory(plugins)

add_library(${sdfrontendlib} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdfrontendlib_sources}
  ${sdfrontendlib_headers}
  )

if(MINGW)
  target_link_libraries(${sdfrontendlib} ole32)
endif()

if(UNIX)
  target_link_libraries(${sdfrontendlib} dl)
endif()

target_link_libraries(${sdfrontendlib} sdutils sdcore)

INSTALL(TARGETS ${sdfrontendlib}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdfrontendlib_headers}
  DESTINATION include/frontend/lib
  COMPONENT headers
  )
