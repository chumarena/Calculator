cmake_minimum_required(VERSION 3.10)

project(ModularCalculator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/plugins)

include_directories(${PROJECT_SOURCE_DIR}/src)


add_executable(Calculator
    BaseOperations.cpp
    BaseOperations.h
    Calculator.cpp
    ExpressionTree.cpp
    ExpressionTree.h
    Parser.cpp
    Parser.h
    PluginLoader.cpp
    PluginLoader.h
)

if (WIN32)
    target_link_libraries(Calculator PUBLIC ws2_32) 
endif()

function(add_calculator_plugin plugin_name)
        set(PLUGIN_SOURCES 
        plugins/${plugin_name}/${plugin_name}.cpp
        plugins/${plugin_name}/framework.h
        plugins/${plugin_name}/pch.cpp
        plugins/${plugin_name}/pch.h
    )

    
    
    
    add_library(${plugin_name} SHARED ${PLUGIN_SOURCES})
    
    
    set_target_properties(${plugin_name} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} 
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins
    )

    
    target_include_directories(${plugin_name} PUBLIC ${PROJECT_SOURCE_DIR}/src)

    
    set_property(TARGET ${plugin_name} APPEND PROPERTY COMPILE_DEFINITIONS EXPORTING_DLL)

endfunction()


add_calculator_plugin(cos)
add_calculator_plugin(ln)
add_calculator_plugin(pow)
add_calculator_plugin(sin)



file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)
