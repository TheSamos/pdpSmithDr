include(SmithDRPluginCompilation.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${smithdr_SOURCE_DIR} ${sdfrontendlib_SOURCE_DIR}/plugins)

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

find_package(Qt4)
if(QT_FOUND)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${QT_INCLUDES})
  include(${QT_USE_FILE})
  set(link_libs ${link_libs}  ${QT_QTCORE_LIBRARIES})
endif()

add_subdirectory(algo)
add_subdirectory(io)
add_subdirectory(widgets)