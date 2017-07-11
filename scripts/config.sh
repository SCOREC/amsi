#!/bin/bash
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
  BUILD_DIR=$ROOT/build_release
  BUILD_TESTS=OFF
  if [ "$TEST_OVERRIDE" == "ON" ] ; then
    BUILD_TESTS=ON
  fi
fi
verify_directory_recreate ${BUILD_DIR}
cd $BUILD_DIR
HOSTNAME=`hostname`
if [ "$HOSTNAME" == "q.ccni.rpi.edu" ]; then
  module use /gpfs/u/home/PASC/PASCtbnw/barn-shared/install/petsc-3.6.3/bgq/xl/lib/petsc/conf/modules/
  module use /gpfs/u/barn/PASC/shared/module
  module load xl
  module load petsc/3.6.3
  module load proprietary/simmetrix/simModSuite/11.0-160922pre-xl
  module load boost
  cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_C_COMPILER="mpicc" \
    -DCMAKE_CXX_COMPILER="mpicxx" \
    -DBUILD_TESTS=$BUILD_TESTS \
    -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/ \
    -DSCOREC_DIR=$DEVROOT/install/core/lib/cmake/SCOREC \
    -DHWLOC_ROOT=$DEVROOT/install/hwloc/xl/ \
    -DBOOST_INCLUDE_DIR=$BOOST_INCLUDE_DIR \
    ..
else
    CC=`which mpicc`
    CXX=`which mpicxx`
    cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DBUILD_TESTS=$BUILD_TESTS \
        -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/ \
        -DCMAKE_C_COMPILER=$CC \
        -DCMAKE_CXX_COMPILER=$CXX \
        -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
        -DSCOREC_DIR=$DEVROOT/install/core/lib/cmake/SCOREC \
        ..
fi
