set(sdimageprocessing "sdimageprocessing")
project(${sdimageprocessing})

file(GLOB_RECURSE _sdimageprocessing_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE _sdimageprocessing_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdimageprocessing_sources "${_sdimageprocessing_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" sdimageprocessing_headers "${_sdimageprocessing_headers}")

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${FFTW3_INCLUDE_DIR})

add_subdirectory(filters)

add_library(${sdimageprocessing} ${library_type}
#  ${WIN32_CONSOLE}
  ${sdimageprocessing_sources}
  ${sdimageprocessing_headers}
  )

target_link_libraries(${sdimageprocessing} sdcore ${FFTW3_LIBRARIES})

if(MINGW)
  target_link_libraries(${sdimageprocessing} ole32)
endif()

INSTALL(TARGETS ${sdimageprocessing}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sdimageprocessing_headers}
  DESTINATION include/imageprocessing
  COMPONENT headers
  )
