set(sdfrontendlibqt "sdfrontendlibqt")
project(${sdfrontendlibqt})

file(GLOB_RECURSE _sdfrontendlibqt_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE _sdfrontendlibqt_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdfrontendlibqt_sources "${_sdfrontendlibqt_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdfrontendlibqt_headers "${_sdfrontendlibqt_headers}")

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

if ("${sdfrontendlibqt_sources}" STREQUAL "")
  set(dummyFile "sdDummy.cpp")
  FILE(WRITE ${dummyFile} "int main(){return 0;}")
  set(sdfrontendlibqt_sources ${dummyFile})
endif()

add_library(${sdfrontendlibqt} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdfrontendlibqt_sources}
  ${sdfrontendlibqt_headers}
  )

if(MINGW)
  target_link_libraries(${sdfrontendlibqt} ole32)
endif()

if(UNIX)
  target_link_libraries(${sdfrontendlibqt} dl)
endif()

target_link_libraries(${sdfrontendlibqt} sdutils sdcore)

INSTALL(TARGETS ${sdfrontendlibqt}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdfrontendlibqt_headers}
  DESTINATION include/frontend/lib
  COMPONENT headers
  )
