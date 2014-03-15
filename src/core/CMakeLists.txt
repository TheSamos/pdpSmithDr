set(sdcore "sdcore")
project(${sdcore})

file(GLOB_RECURSE _sdcore_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE _sdcore_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdcore_sources "${_sdcore_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdcore_headers "${_sdcore_headers}")

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(image)
add_subdirectory(mesh)

add_library(${sdcore} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdcore_sources}
  ${sdcore_headers}
  )

if(MINGW)
  target_link_libraries(${sdcore} ole32)
endif()

if(UNIX)
  target_link_libraries(${sdcore} dl)
endif()

INSTALL(TARGETS ${sdcore}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdcore_headers}
  DESTINATION include/core
  COMPONENT headers
  )
