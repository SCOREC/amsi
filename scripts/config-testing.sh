#!/bin/bash
# CMake config for AMSI testing with ctest
ROOT=$(dirname "$PWD")
BUILD_DIR=$ROOT/build_debug
BUILD_TESTS=ON
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
module load cmake
module load $DEVROOT/module/openmpi/1.10.0
module load simmetrix/simModSuite
module load $DEVROOT/petsc/petsc-3.6.3/ompi110-debug/lib/petsc/conf/modules/petsc/3.6.3
CC=`which mpicc`
CXX=`which mpicxx`
cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTS=${BUILD_TESTS} \
  -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/sim/openmpi-1.10.0/ \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX \
  -DHWLOC_ROOT=$DEVROOT/install/hwloc/ \
  -DSCOREC_DIR=$DEVROOT/install/core/openmpi-1.10.0/lib/cmake/SCOREC \
  ..
