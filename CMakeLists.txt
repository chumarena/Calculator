# Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.10)

# Название основного проекта
project(ModularCalculator CXX)

# Включение C++17 для std::filesystem, используемого в PluginLoader
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Установка корневой папки, где будет исполняемый файл и DLL
# (Для Visual Studio: Debug/Release внутри Build, для других: Build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/plugins)

# =================================================================
# 1. КОНФИГУРАЦИЯ ЗАГОЛОВОЧНЫХ ФАЙЛОВ ИНТЕРФЕЙСА (IPlugin.h)
# =================================================================

# Заголовочные файлы интерфейса доступны для всех подпроектов
include_directories(${PROJECT_SOURCE_DIR}/src)

# =================================================================
# 2. КОМПИЛЯЦИЯ ОСНОВНОГО ПРИЛОЖЕНИЯ (Calculator.exe)
# =================================================================

add_executable(Calculator
    BaseOperation.cpp
    BaseOperation.h
    Calculator.cpp
    ExpressionTree.cpp
    ExpressionTree.h
    Parser.cpp
    Parser.h
    PluginLoader.cpp
    PluginLoader.h
)

# Добавляем необходимые библиотеки для PluginLoader (загрузка DLL)
if (WIN32)
    target_link_libraries(Calculator PUBLIC ws2_32) # Обычно требуется для LoadLibrary, хотя не всегда явно
endif()

# =================================================================
# 3. КОМПИЛЯЦИЯ ПЛАГИНОВ (.dll)
# =================================================================

# Функция для упрощения создания каждого плагина
function(add_calculator_plugin plugin_name)
    # Определяем исходные файлы плагина, которые находятся в папке 'plugins/plugin_name'
    set(PLUGIN_SOURCES 
        plugins/${plugin_name}/${plugin_name}.cpp
        plugins/${plugin_name}/framework.h
        plugins/${plugin_name}/pch.cpp
        plugins/${plugin_name}/pch.h
    )

    # Специальная обработка для pow.cpp (файл называется -pow.cpp, а папка pow)
    if (${plugin_name} STREQUAL "pow")
        set(PLUGIN_SOURCES 
            plugins/pow/pow.cpp
            plugins/pow/framework.h
            plugins/pow/pch.cpp
            plugins/pow/pch.h
        )
    endif()

    # Устанавливаем тип сборки как динамическую библиотеку
    add_library(${plugin_name} SHARED ${PLUGIN_SOURCES})
    
    # Устанавливаем директорию вывода для DLL в 'plugins'
    set_target_properties(${plugin_name} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} 
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins
    )

    # Убеждаемся, что плагины могут найти IPlugin.h (который находится в src)
    target_include_directories(${plugin_name} PUBLIC ${PROJECT_SOURCE_DIR}/src)

    # Устанавливаем флаги для экспорта (важно для __declspec(dllexport))
    set_property(TARGET ${plugin_name} APPEND PROPERTY COMPILE_DEFINITIONS EXPORTING_DLL)

endfunction()

# Вызываем функцию для каждого плагина
add_calculator_plugin(cos)
add_calculator_plugin(ln)
add_calculator_plugin(pow)
add_calculator_plugin(sin)


# Создание папки 'plugins' при сборке
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)