cmake_minimum_required(VERSION 3.16)
project(pyluban)

message(STATUS CMAKE_SYSTEM_VERSION)
message(STATUS CMAKE_HOST_SYSTEM_VERSION)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
add_compile_definitions(__GCC__)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(APPLE)
add_compile_definitions(_LIBCPP_DISABLE_AVAILABILITY)
endif(APPLE)

add_compile_options(-fPIC)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/include)


link_directories(/usr/local/lib)

SET(LUBAN_INCLUDE include/builtin.h include/common.h include/feature.h 
include/helper.h include/MurmurHash3.h include/placement.h
include/operator.h include/toolkit.h)

SET(LUBAN_SOURCE  src/builtin.cpp src/common.cpp src/feature.cpp 
src/MurmurHash3.cc src/operator.cpp src/placement.cpp src/toolkit.cpp)

add_library(luban_static STATIC ${LUBAN_SOURCE})

option(PYBIND "Description of PYBIND" ON)
if (PYBIND)
    set (Python3_FIND_VIRTUALENV FIRST)
    set(PYTHON3_EXECUTABLE ${PYTHON_EXECUTABLE})

    find_package(Python3 COMPONENTS Interpreter Development)
    include_directories(${Python3_INCLUDE_DIRS})

    message(STATUS "PYTHON3_LIBRARIES = ${Python3_LIBRARIES}")
    message(STATUS "PYTHON3_EXECUTABLE = ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON3_INCLUDE_DIRS = ${Python3_INCLUDE_DIRS}")

    # Include pybind11
    find_package(pybind11 REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})

    # Define the Python module
    pybind11_add_module(pyluban  src/pyluban.cpp)
    target_link_libraries(pyluban PRIVATE luban_static pthread)
endif()