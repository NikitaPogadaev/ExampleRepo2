#!/bin/bash

rm -rf build
rm -rf lib
rm -rf bin
mkdir -p build
cd build

if [[($@ == *'--test'*)]]
then
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build . --target Tests -j 16
else
    cmake ..
    cmake --build . -j 16
fi

# cmake ..
# make
