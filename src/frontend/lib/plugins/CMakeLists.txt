
file(GLOB sdfrontendplugins_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

INSTALL(FILES ${sdfrontendplugins_headers}
  DESTINATION include/frontend/plugins
  COMPONENT headers
  )