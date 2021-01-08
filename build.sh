#!/bin/sh
echo "Building gitty..."
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug && make
cd ..
echo "Added gitty to PATH."
export PATH=$PATH:$PWD/build/src/gitty