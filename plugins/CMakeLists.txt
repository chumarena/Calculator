
macro(add_calculator_plugin plugin_name)
    
    
    file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/${plugin_name}.cpp")

    
    if(SOURCES)
        message(STATUS "Source for plugin ${plugin_name} found. Building shared library.")
        
       
        add_library(${plugin_name}_lib SHARED ${SOURCES})

        set_target_properties(${plugin_name}_lib PROPERTIES 
            OUTPUT_NAME ${plugin_name}
            PREFIX ""
            
            RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:calc>/plugins"
        )
    else()
        
        message(STATUS "Source file ${plugin_name}.cpp not found. Skipping plugin build.")
    endif()
endmacro()



add_calculator_plugin(sin)
add_calculator_plugin(cos)
add_calculator_plugin(ln)
add_calculator_plugin(pow)