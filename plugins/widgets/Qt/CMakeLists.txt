
find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${QT_INCLUDES})

include(${QT_USE_FILE})

file(GLOB_RECURSE _plugin_sources ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_CPP_FILES})
file(GLOB_RECURSE moc_files ${CMAKE_CURRENT_SOURCE_DIR}/${SMITHDR_MOC_FILES})
list(REMOVE_ITEM _plugin_sources ${moc_files} .)
SMITHDR_REMOVE_DIR_CONTENT(".svn/" plugin_sources "${_plugin_sources}")

set(link_libs smithdr ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES})

foreach(plugin IN ITEMS ${plugin_sources})
  get_filename_component(plugin_name ${plugin} NAME_WE)
  string(REGEX REPLACE ".cpp" ".hpp" plugin_header ${plugin})

  if (EXISTS ${plugin_header})
    unset(plugin_moc_header)
    SMITHDR_GET_MOC_HEADERS(plugin_moc_header "${plugin_header}")

    if (NOT ${plugin_moc_header} STREQUAL "")
      QT4_WRAP_CPP(plugin_moc_source ${plugin_moc_header})

      set(src ${plugin_moc_header})
      get_filename_component(src_path ${src} PATH)
      get_filename_component(src_name_we ${src} NAME_WE)

      string(REGEX REPLACE ${CMAKE_SOURCE_DIR} "" subdir ${src_path})
      set(include_dir ${CMAKE_BINARY_DIR}${subdir})
      include_directories(${include_dir})

      execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${include_dir})
      qt4_generate_moc(${src} ${include_dir}/${src_name_we}.moc)
      set_source_files_properties(${src} PROPERTIES OBJECT_DEPENDS ${include_dir}/${src_name_we}.moc)

      add_library(${plugin_name} MODULE ${plugin} ${plugin_moc_source})
    else()
      add_library(${plugin_name} MODULE ${plugin})
    endif()
  else()
    add_library(${plugin_name} MODULE ${plugin})
  endif()

  set_target_properties(${plugin_name} PROPERTIES DEFINE_SYMBOL "SMITHDR_PLUGIN_DLL_EXPORTS")

  target_link_libraries(${plugin_name} ${link_libs})

  if(MINGW)
    target_link_libraries(${plugin_name} ole32)
  endif()

  INSTALL(TARGETS ${plugin_name}
    ARCHIVE DESTINATION plugins
    LIBRARY DESTINATION plugins
    RUNTIME DESTINATION bin
    COMPONENT plugins
    )
endforeach(plugin)
