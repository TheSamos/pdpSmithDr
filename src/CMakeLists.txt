set(sd "smithdr")
project(${sd})

file(GLOB sd_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB sd_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HXX_FILES})

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/external)

add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(imageprocessing)
add_subdirectory(meshprocessing)
add_subdirectory(frontend)

if ("${sd_sources}" STREQUAL "")
  set(dummyFile "sdDummy.cpp")
  FILE(WRITE ${dummyFile} "int main(){return 0;}")
  set(sd_sources ${dummyFile})
endif()

add_library(${sd} ${library_type}
#  ${WIN32_CONSOLE}
  ${sd_sources}
  ${sd_headers}
  )
set_target_properties(${sd} PROPERTIES LINKER_LANGUAGE CXX)


set(smithdr_link_libs
  sdutils
  sdcore
#  sdGSO
  sdimageprocessing
  sdmeshprocessing
  sdfrontendlib
  sdfrontendlibqt
)

#link_directories(${CMAKE_BINARY_DIR}/lib)

target_link_libraries(${sd} ${smithdr_link_libs})

if(MINGW)
  target_link_libraries(${sd} ole32)
endif()

INSTALL(TARGETS ${sd}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT libraries
  )

INSTALL(FILES ${sd_headers}
  DESTINATION include
  COMPONENT headers
  )
