cmake_minimum_required(VERSION 3.17)
project(luban)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
#add_library(damo damo.cpp damo.h common.h decay_learning_rate.h storage.h vector.h initializer.h optimizer.h config.h pserver.h)



add_library(luban MurmurHash3.h MurmurHash3.cc feature.pb.h feature.pb.cc common.h cpptoml.h factory.h helper.h toolkit.h utils.h basic_fps.h)

target_link_libraries(luban --leveldb)