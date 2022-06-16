cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

SET(LUBAN_INCLUDE include/MurmurHash3.h include/feature.pb.h 
include/common.h include/cpptoml.h include/factory.h 
include/helper.h include/toolkit.h include/utils.h 
include/basic_fps.h include/luban_c.h )

SET(LUBAN_SOURCE  src/MurmurHash3.cc src/feature.pb.cc 
src/basic_fps.cpp src/luban_c.cpp src/toolkit.cpp 
src/utils.cpp)

SET(TOOLKIT_INCLUDE include/MurmurHash3.h include/feature.pb.h 
include/common.h include/cpptoml.h include/factory.h include/helper.h 
include/toolkit.h include/utils.h include/basic_fps.h include/luban.h)

SET(TOOLKIT_SOURCE src/MurmurHash3.cc src/feature.pb.cc src/basic_fps.cpp 
src/luban.cpp src/toolkit.cpp src/utils.cpp)

SET(STATA_INCLUDE include/json.hpp include/stata.h)
SET(STATA_SOURCE src/stata.cpp)

add_library(luban SHARED ${LUBAN_INCLUDE} ${LUBAN_SOURCE})
add_library(toolkit SHARED ${TOOLKIT_INCLUDE} ${TOOLKIT_SOURCE})
add_library(stata SHARED ${STATA_INCLUDE} ${STATA_SOURCE})

add_executable(luban_tool ${LUBAN_INCLUDE} ${LUBAN_SOURCE} src/luban_tool.cpp)
add_executable(test_stata ${STATA_INCLUDE} ${STATA_SOURCE} src/test_stata.cpp)

target_link_libraries(luban protobuf)
target_link_libraries(luban_tool protobuf)
target_link_libraries(toolkit protobuf)
target_link_libraries(stata glog)
target_link_libraries(test_stata glog)