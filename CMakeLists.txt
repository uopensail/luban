cmake_minimum_required(VERSION 3.16)
project(luban)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/include)

link_directories(/usr/local/lib)

SET(LUBAN_INCLUDE include/base64.h include/cpptoml.h include/feature_builtin_operators.hpp
include/feature_helper.hpp  include/feature_hash_toolkit.hpp include/feature_operator_toolkit.hpp
include/feature_operator_handler.hpp include/feature_operator_runtime.hpp 
include/feature_operator_toolkit.hpp include/feature.pb.h 
include/helper.h include/MurmurHash3.h include/toolkit.hpp)

SET(LUBAN_SOURCE  src/MurmurHash3.cc src/base64.cpp  src/feature.pb.cc)

add_library(toolkit SHARED ${LUBAN_INCLUDE} ${LUBAN_SOURCE})
#add_library(toolkit STATIC ${LUBAN_INCLUDE} ${LUBAN_SOURCE})
add_library(luban SHARED ${LUBAN_INCLUDE} include/luban.h ${LUBAN_SOURCE} src/luban.cpp)
#add_library(luban STATIC ${LUBAN_INCLUDE} include/luban.h ${LUBAN_SOURCE} src/luban.cpp)

add_executable(optoolkit_test ${LUBAN_INCLUDE} ${LUBAN_SOURCE} src/test_operator_toolkit.cpp)

target_link_libraries(toolkit protobuf pthread)
target_link_libraries(luban protobuf pthread)
target_link_libraries(optoolkit_test protobuf)
