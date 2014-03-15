set(sdtests "sdtests")
project(${sdtests})

include_directories(${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/src/frontend/lib/plugins)

file(GLOB sdtests_sources ${CMAKE_SOURCE_DIR}/tests/test${SMITHDR_CPP_FILES})

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

add_definitions(-DPLUGIN_DIR=\"${SMITHDR_PLUGIN_DIR}\")

foreach(test_file ${sdtests_sources})

  get_filename_component(test_name ${test_file} NAME_WE)

  include_directories(${FFTW3_INCLUDE_DIR})

  add_executable(${test_name} WIN32
    ${WIN32_CONSOLE}
    ${test_file}
    )

  link_directories(${CMAKE_BINARY_DIR}/lib)

  target_link_libraries(${test_name} smithdr ${FFTW3_LIBRARIES})

  if(MINGW)
    target_link_libraries(${test_name} ole32)
  endif()

  INSTALL(TARGETS ${test_name}
    RUNTIME DESTINATION bin COMPONENT tests
    )

endforeach(test_file)
