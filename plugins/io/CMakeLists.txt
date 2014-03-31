
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE plugin_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})

set(link_libs smithdr)

find_package(ZLIB)
if(ZLIB_FOUND)
 set(link_libs ${link_libs} ${ZLIB_LIBRARIES})
endif()

plugin_compile("${plugin_sources}" "${link_libs}")
