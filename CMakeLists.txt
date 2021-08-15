cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

SET(SOURCE include/MurmurHash3.h src/MurmurHash3.cc include/feature.pb.h src/feature.pb.cc include/common.h include/cpptoml.h include/factory.h include/helper.h include/toolkit.h include/utils.h include/basic_fps.h src/basic_fps.cpp include/luban_c.h src/luban_c.cpp src/toolkit.cpp src/utils.cpp)
SET(CPP_SOURCE include/MurmurHash3.h src/MurmurHash3.cc include/feature.pb.h src/feature.pb.cc include/common.h include/cpptoml.h include/factory.h include/helper.h include/toolkit.h include/utils.h include/basic_fps.h src/basic_fps.cpp include/luban.h src/luban.cpp src/toolkit.cpp src/utils.cpp)
SET(STATA_SOURCE include/json.hpp include/stata.h src/stata.cpp)
add_library(luban SHARED ${SOURCE})
add_library(toolkit SHARED ${CPP_SOURCE})
add_library(stata SHARED ${STATA_SOURCE})

add_executable(luban_tool ${SOURCE} src/luban_tool.cpp src/basic_fps.cpp src/toolkit.cpp src/utils.cpp)

add_executable(test_stata ${STATA_SOURCE} src/test_stata.cpp)

target_link_libraries(luban protobuf)
target_link_libraries(luban_tool protobuf)
target_link_libraries(toolkit protobuf)
target_link_libraries(stata glog)
target_link_libraries(test_stata glog)