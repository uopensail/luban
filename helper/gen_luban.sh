#!/bin/bash

cd ../include
swig -c++ -Wall -py3 -python luban.i
mv luban.py ../
mv luban_wrap.cxx ../src/
