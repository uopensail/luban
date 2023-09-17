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


include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/include)

# Find and link to Python
find_package (Python COMPONENTS Interpreter Development)
include_directories(${PYTHON_INCLUDE_DIRS})

message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")

# Include pybind11
find_package(pybind11 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


link_directories(/usr/local/lib)

SET(LUBAN_INCLUDE include/builtin.h include/common.h include/feature.h 
include/helper.h include/MurmurHash3.h include/placement.h
include/operator.h include/toolkit.h)

SET(LUBAN_SOURCE  src/builtin.cpp src/common.cpp src/feature.cpp 
src/MurmurHash3.cc src/operator.cpp src/placement.cpp src/toolkit.cpp)

# Define the Python module
pybind11_add_module(pyluban ${LUBAN_INCLUDE} ${LUBAN_SOURCE} src/pyluban.cpp)