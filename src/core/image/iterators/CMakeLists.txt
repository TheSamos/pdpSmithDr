
file(GLOB sdcoreimageiterators_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

INSTALL(FILES ${sdcoreimageiterators_headers}
  DESTINATION include/core/image/iterators
  COMPONENT headers
  )