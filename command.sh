#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"
if [ -d "$DIR/Build" ] 
then
    echo "No need to create a directory." 
else
    mkdir Build
fi

g++ -o src ./source/MacOS/heap.cpp ./source/MacOS/tool.cpp ./source/MacOS/unittest.cpp ./source/MacOS/src.cpp -std=c++20
mv src Build
echo "build completed!"