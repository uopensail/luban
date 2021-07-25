cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)


include_directories(/usr/local/include)

link_directories(/usr/local/lib)

SET(SOURCE src/MurmurHash3.h src/MurmurHash3.cc src/feature.pb.h src/feature.pb.cc src/common.h src/cpptoml.h src/factory.h src/helper.h src/toolkit.h src/utils.h src/basic_fps.h src/luban_c.h src/luban_c.cpp)

add_library(luban SHARED ${SOURCE})

add_executable(luban_tool ${SOURCE} src/luban_tool.cpp)

target_link_libraries(luban protobuf)
target_link_libraries(luban_tool protobuf)