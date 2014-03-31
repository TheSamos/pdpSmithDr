
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${FFTW3_INCLUDE_DIR})

file(GLOB_RECURSE plugin_sources ${CMAKE_CURRENT_SOURCE_DIR}/*Plugin.cpp)

set(link_libs smithdr ${FFTW3_LIBRARIES})

plugin_compile("${plugin_sources}" "${link_libs}")
