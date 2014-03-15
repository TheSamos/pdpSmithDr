
file(GLOB sdfilters_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${FFTW3_INCLUDE_DIR})

INSTALL(FILES ${sdfilters_headers}
  DESTINATION include/imageprocessing/filters
  COMPONENT headers
  )
