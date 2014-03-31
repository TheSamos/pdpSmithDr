
function(plugin_compile plugin_sources plugin_link_libraries)

  foreach(plugin IN ITEMS ${plugin_sources})
    
    get_filename_component(dirname ${plugin} PATH)
    
    if(EXISTS "${dirname}/CMakeLists.txt")
      add_subdirectory(${dirname})
    else()
      get_filename_component(plugin_name ${plugin} NAME_WE)
      
      if(EXISTS "${dirname}/${plugin_name}")
	file(GLOB_RECURSE current_plugin_sources ${dirname}/${plugin_name}/${SMITHDR_CPP_FILES})
      endif()
      
      add_library(${plugin_name} MODULE ${plugin} ${current_plugin_sources})
      set_target_properties(${plugin_name} PROPERTIES DEFINE_SYMBOL "SMITHDR_PLUGIN_DLL_EXPORTS")
      target_link_libraries(${plugin_name} ${plugin_link_libraries})
      set(current_plugin_sources "")
      
      if(MINGW)
	target_link_libraries(${plugin_name} ole32)
      endif()
      
      INSTALL(TARGETS ${plugin_name}
	ARCHIVE DESTINATION plugins
	LIBRARY DESTINATION plugins
	RUNTIME DESTINATION bin
	COMPONENT plugins
	)
      
    endif()
    
  endforeach(plugin)

endfunction()
