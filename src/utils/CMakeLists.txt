set(sdutils "sdutils")
project(${sdutils})

file(GLOB sdutils_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB sdutils_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

add_library(${sdutils} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdutils_sources}
  ${sdutils_headers}
  )

if(MINGW)
  target_link_libraries(${sdutils} ole32)
endif()


INSTALL(TARGETS ${sdutils}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdutils_headers}
  DESTINATION include/utils
  COMPONENT headers
  )
