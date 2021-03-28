cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)


include_directories(/usr/local/include)

link_directories(/usr/local/lib)

add_library(luban MurmurHash3.h MurmurHash3.cc feature.pb.h feature.pb.cc common.h cpptoml.h factory.h helper.h toolkit.h utils.h basic_fps.h config.h luban_c.h pyluban.cpp pyluban.h)

target_link_libraries(luban --leveldb)