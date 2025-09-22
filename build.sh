#!/bin/bash

if [ "$1" = "Debug" ]; then
    BUILD_DIR=build
    BUILD_TYPE=Debug
elif [ "$1" = "Release" ]; then
     BUILD_DIR=build
     BUILD_TYPE=Release
else
    echo "Build type expected as \"release\" or \"debug\" (i.g. \"build.sh release\")"
    exit 1
fi

rm -rf ${BUILD_DIR}

cmake -H. -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DOCI_INC_DIR=$2 -DOCI_LIB_DIR=$3 -DPOCO_INC_DIR=$4 -DPOCO_LIB_DIR=$5
cmake --build ${BUILD_DIR} --target all -- -j4
