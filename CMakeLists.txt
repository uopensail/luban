cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

SET(SOURCE include/MurmurHash3.h src/MurmurHash3.cc include/feature.pb.h src/feature.pb.cc include/common.h include/cpptoml.h include/factory.h include/helper.h include/toolkit.h include/utils.h include/basic_fps.h src/basic_fps.cpp include/luban_c.h src/luban_c.cpp src/toolkit.cpp src/utils.cpp)
SET(CPP_SOURCE include/MurmurHash3.h src/MurmurHash3.cc include/feature.pb.h src/feature.pb.cc include/common.h include/cpptoml.h include/factory.h include/helper.h include/toolkit.h include/utils.h include/basic_fps.h src/basic_fps.cpp include/luban.h src/luban.cpp src/toolkit.cpp src/utils.cpp)

add_library(luban SHARED ${SOURCE})
add_library(toolkit STATIC ${CPP_SOURCE})

add_executable(luban_tool ${SOURCE} src/luban_tool.cpp src/basic_fps.cpp src/toolkit.cpp src/utils.cpp)

target_link_libraries(luban protobuf)
target_link_libraries(luban_tool protobuf)
target_link_libraries(toolkit protobuf)