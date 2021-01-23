#!/bin/bash
path=$(dirname $0)
pwd
rm -rf ../include/feature.pb.h
rm -rf ../src/feature.pb.cc

#wget "https://raw.githubusercontent.com/tensorflow/tensorflow/master/tensorflow/core/example/feature.proto"
protoc --cpp_out=./ *.proto

mv feature.pb.h ../include/feature.pb.h
mv feature.pb.cc ../src/feature.pb.cc


wget "https://raw.githubusercontent.com/skystrife/cpptoml/master/include/cpptoml.h"
