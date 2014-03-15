
file(GLOB sdcoremesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(iterators)

INSTALL(FILES ${sdcoremesh_headers}
  DESTINATION include/core/mesh
  COMPONENT headers
  )