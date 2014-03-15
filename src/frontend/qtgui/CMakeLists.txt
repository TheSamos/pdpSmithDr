set(_smithdr "SmithDR")
project(${_smithdr})

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${QT_INCLUDES} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})
#set(CMAKE_PACKAGE_QTGUI TRUE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE _smithdr_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE moc_files ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_MOC_FILES})
list(REMOVE_ITEM _smithdr_sources ${moc_files} .)
file(GLOB_RECURSE _smithdr_headers ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_HPP_FILES})
SMITHDR_REMOVE_DIR_CONTENT(".svn/" smithdr_sources "${_smithdr_sources}")
SMITHDR_REMOVE_DIR_CONTENT(".svn/" smithdr_headers "${_smithdr_headers}")

# determine moc headers automatically
SMITHDR_GET_MOC_HEADERS(smithdr_moc_headers "${smithdr_headers}")
QT4_WRAP_CPP(smithdr_moc_sources ${smithdr_moc_headers})

SMITHDR_GET_MOC_HEADERS(smithdr_moc_files "${smithdr_sources}")
foreach(src IN ITEMS ${smithdr_moc_files})
  get_filename_component(src_path ${src} PATH)
  get_filename_component(src_name_we ${src} NAME_WE)

  string(REGEX REPLACE ${CMAKE_SOURCE_DIR} "" subdir ${src_path})
  set(include_dir ${CMAKE_BINARY_DIR}${subdir})
  include_directories(${include_dir})

  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${include_dir})
  qt4_generate_moc(${src} ${include_dir}/${src_name_we}.moc)
  set_source_files_properties(${src} PROPERTIES OBJECT_DEPENDS ${include_dir}/${src_name_we}.moc)
endforeach()

#QT4_ADD_RESOURCES(smithdr_rc_sources ${CMAKE_CURRENT_SOURCE_DIR}/SmithDR.qrc)

#include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} ${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (SMITHDR_DEVEL_VERSION)
  add_definitions(-DDEVEL_VERSION)
endif()

add_definitions(-DPLUGIN_DIR=\"${SMITHDR_PLUGIN_DIR}\")

if(WIN32)
  # may cause an error in running windres.h due to whitespaces in directory name
  set(FILE_RC ${CMAKE_SOURCE_DIR}/resources/smithdr.rc)
endif()

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
if(APPLE)
  set(sd_icon ${CMAKE_SOURCE_DIR}/resources/icons/SmithDR/SD_icon.icns)
  # set how it shows up in the Info.plist file
  set(MACOSX_BUNDLE_ICON_FILE ${sd_icon}) 
  # set where in the bundle to put the icns file
  set_source_files_properties(${sd_icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  set(smithdr_sources ${smithdr_sources} ${sd_icon})
ENDIF(APPLE)

add_executable(${_smithdr} MACOSX_BUNDLE #WIN32
#  ${WIN32_CONSOLE}
  ${smithdr_sources}
  ${smithdr_headers}
  ${smithdr_moc_sources}
  ${FILE_RC}
  ${smithdr_rc_sources}
  )

#link_directories(${CMAKE_BINARY_DIR}/lib)

target_link_libraries(${_smithdr}
  smithdr
  ${QT_QTMAIN_LIBRARY}
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  )

if(MINGW)
  target_link_libraries(${_smithdr} ole32)
endif()


INSTALL(TARGETS ${_smithdr}
  COMPONENT applications
  BUNDLE DESTINATION .
  RUNTIME DESTINATION bin
  )

# external dependencies
if(WIN32)
  add_custom_command(TARGET ${_smithdr} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${FFTW3_LIBRARY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
  install(FILES ${FFTW3_LIBRARY} DESTINATION bin COMPONENT applications)
endif(WIN32)

SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${_smithdr}")
IF(APPLE)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${_smithdr}.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${_smithdr}.exe")
ENDIF(WIN32)

set(DIRS "\${CMAKE_BINARY_DIR}/lib;\${CMAKE_INSTALL_PREFIX}/lib")

INSTALL(CODE "
  file(GLOB_RECURSE SDPLUGINS \"\${CMAKE_BINARY_DIR}/lib/*.so\")
  include(BundleUtilities)
  fixup_bundle(\"${APPS}\" \"\${SDPLUGINS}\" \"${DIRS}\")
  " COMPONENT Runtime)
