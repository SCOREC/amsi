#!/bin/bash -x
# CMake config for AMSI
# usage: ./config.sh [build_type] [build_tests_flag]
#
source util
ROOT=$(dirname $PWD)
# Determine if we should build test based on build_type and override
TEST_OVERRIDE=$2
if [ -z $1 ]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=$1
fi
if [ "$BUILD_TYPE" == "Debug" ]; then
  BUILD_DIR=$ROOT/build_debug
  BUILD_TESTS=ON
  if [ "$TEST_OVERRIDE" == "OFF" ] ; then
    BUILD_TESTS=OFF
  fi
elif [ "$BUILD_TYPE" == "Release" ] ; then
  BUILD_TYPE="RelWithDebugInfo"
  BUILD_DIR=$ROOT/build_release
  BUILD_TESTS=OFF
  if [ "$TEST_OVERRIDE" == "ON" ] ; then
    BUILD_TESTS=ON
  fi
fi
verify_directory_recreate ${BUILD_DIR}
mkdir $BUILD_DIR
rm -rf $BUILD_DIR/CMakeCache.txt
cd $BUILD_DIR
HOSTNAME=`hostname`
if [ "$HOSTNAME" == "q.ccni.rpi.edu" ]; then
  cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_C_COMPILER="mpicc" \
    -DCMAKE_CXX_COMPILER="mpicxx" \
    -DBUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX=/gpfs/u/scratch/PASC/shared/install/amsi/ \
    -DSCOREC_DIR=/gpfs/u/scratch/PASC/shared/install/core/lib/cmake/SCOREC\ \
    -DBOOST_INCLUDE_DIR=/gpfs/u/barn/PASC/shared/boost_1_56_0/ \
    -DVERBOSITY=0 \
    ..
else
    CC=`which mpicc`
    CXX=`which mpicxx`
    cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DBUILD_TESTS=$BUILD_TESTS \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
        -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/$BUILD_TYPE \
        -DCMAKE_C_COMPILER=$CC \
        -DCMAKE_CXX_COMPILER=$CXX \
        -DSCOREC_DIR=$DEVROOT/install/core/$BUILD_TYPE/lib/cmake/SCOREC \
        -DPETSC_DIR="$PETSC_DIR" \
        -DPETSC_ARCH="$PETSC_ARCH" \
        -DENABLE_VERBOSITY:STRING=HIGH \
        ..
        #-DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
fi
