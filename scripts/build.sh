#!/bin/bash

mkdir -p build
mkdir -p bin

cd build
cmake ..
make

cd ..
cp build/libflow.so bin
