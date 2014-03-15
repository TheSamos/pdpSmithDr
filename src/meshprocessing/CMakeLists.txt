set(sdmeshprocessing "sdmeshprocessing")
project(${sdmeshprocessing})

file(GLOB_RECURSE _sdmeshprocessing_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE _sdmeshprocessing_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${SmithDR_SRC_DIR}/meshprocessing/${SMITHDR_H_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdmeshprocessing_sources "${_sdmeshprocessing_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdmeshprocessing_headers "${_sdmeshprocessing_headers}")

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

if ("${sdmeshprocessing_sources}" STREQUAL "")
  set(dummyFile "sdDummy.cpp")
  FILE(WRITE ${dummyFile} "")
  set(sdmeshprocessing_sources ${dummyFile})
endif()

add_library(${sdmeshprocessing} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdmeshprocessing_sources}
  ${sdmeshprocessing_headers}
  )
set_target_properties(${sdmeshprocessing} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${sdmeshprocessing} sdcore)

if(APPLE)
  target_link_libraries(${sdmeshprocessing} "-bind_at_load")
  target_link_libraries(${sdmeshprocessing} "-framework Carbon")
endif()

if(MINGW)
  target_link_libraries(${sdmeshprocessing} ole32)
endif()


INSTALL(TARGETS ${sdmeshprocessing}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdmeshprocessing_headers}
  DESTINATION include/meshprocessing
  COMPONENT headers
  )
