cmake_minimum_required(VERSION 3.16)
project(luban)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/include)

link_directories(/usr/local/lib)

SET(LUBAN_INCLUDE include/MurmurHash3.h include/feature.pb.h 
include/cpptoml.h include/helper.h  include/feature_builtin_operators.hpp
include/feature_helper.hpp include/feature_operator_handler.hpp
include/feature_operator_runtime.hpp include/feature_operator_toolkit.hpp
include/utils.h)

SET(LUBAN_SOURCE  src/MurmurHash3.cc src/feature.pb.cc)

#SET(TOOLKIT_INCLUDE include/MurmurHash3.h include/feature.pb.h 
#include/common.h include/cpptoml.h include/helper.h 
#include/toolkit.h include/utils.h include/basic_funcs.h include/luban.h)

#SET(TOOLKIT_SOURCE src/MurmurHash3.cc src/feature.pb.cc src/basic_trans.cpp 
#src/basic_funcs.cpp src/luban.cpp src/toolkit.cpp)

#SET(STATA_INCLUDE include/json.hpp include/stata.h)
#SET(STATA_SOURCE src/stata.cpp)

#SET(METRIC_INCLUDE include/metric.h)
#SET(METRIC_SOURCE src/metric.cpp)

#add_library(luban SHARED ${LUBAN_INCLUDE} ${LUBAN_SOURCE})
#add_library(toolkit SHARED ${TOOLKIT_INCLUDE} ${TOOLKIT_SOURCE})

add_executable(optoolkit_test ${LUBAN_INCLUDE} ${LUBAN_SOURCE} src/test_operator_toolkit.cpp)
#add_library(stata SHARED ${STATA_INCLUDE} ${STATA_SOURCE})

#add_executable(luban_tool ${LUBAN_INCLUDE} ${LUBAN_SOURCE} src/luban_tool.cpp)
#add_executable(test_stata ${STATA_INCLUDE} ${STATA_SOURCE} src/test_stata.cpp)

#add_executable(test_metric ${METRIC_INCLUDE} ${METRIC_SOURCE} src/test_metric.cpp)

target_link_libraries(optoolkit_test protobuf)
#target_link_libraries(luban_tool protobuf)
#target_link_libraries(toolkit protobuf)
#target_link_libraries(toolkit_test protobuf)

#target_link_libraries(stata glog)
#target_link_libraries(test_stata glog)
#target_link_libraries(test_metric glog)