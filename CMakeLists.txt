cmake_minimum_required(VERSION 3.16)
project(pyluban)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/include)

# Find and link to Python
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

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