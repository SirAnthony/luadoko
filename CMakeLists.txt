
cmake_minimum_required (VERSION 2.8)

project (luadoko)

find_program(PYTHON_EXECUTABLE python)
find_package(PythonLibs 2.6 REQUIRED)
include(FindPythonModule.cmake)
find_python_module(mako REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(src include)

add_custom_target(generated COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate.py"
    COMMENT "Scanning include files ..."
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SRC src/*.cpp)
add_library(luadoko STATIC ${SRC})
add_dependencies(luadoko generated)