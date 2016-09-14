#!/bin/bash
# CMake config for AMSI
# usage: ./config.sh [build_type] [build_tests_flag]
#
ROOT=$DEVROOT/amsi
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
if [ ! -d $BUILD_DIR ]; then
  mkdir $BUILD_DIR
fi
read -p "About to delete (${BUILD_DIR}). Are you sure (y/n)? " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]] ; then
  rm -rf $BUILD_DIR
  mkdir $BUILD_DIR
else
  echo "Exiting..."
  exit
fi
cd $BUILD_DIR
module load cmake
HOSTNAME=`hostname`
if [ "$HOSTNAME" == "q.ccni.rpi.edu" ]; then
  module load xl
  module load /gpfs/u/home/PASC/PASCtbnw/barn-shared/install/petsc-3.6.3/bgq/xl/lib/petsc/conf/modules/petsc/3.6.3
  module load proprietary/simmetrix/simModSuite/10.0-160902-xl
  module use /gpfs/u/barn/PASC/shared/module
  module load boost
  # export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/gpfs/u/home/PASC/PASCtbnw/barn-shared/install
  cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_C_COMPILER="mpicc" \
    -DCMAKE_CXX_COMPILER="mpicxx" \
    -DBUILD_TESTS=$BUILD_TESTS \
    -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/sim/bgq/xl/ \
    -DCORE_DIR=$DEVROOT/install/core/sim/xl/ \
    -DHWLOC_ROOT=$DEVROOT/install/hwloc/xl/ \
    -DSIM_MPI=bgmpi \
    -DBOOST_INCLUDE_DIR=$BOOST_INCLUDE_DIR \
    ..
  chmod g+rw $BUILD_DIR
else
    module load $DEVROOT/module/openmpi/1.10.0
    module load $DEVROOT/module/simmetrix/simModSuite/10.0-151221beta
    module load $DEVROOT/petsc/petsc-3.6.3/ompi110-debug/lib/petsc/conf/modules/petsc/3.6.3

    CC=`which mpicc`
    CXX=`which mpicxx`
    cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DBUILD_TESTS=ON \
        -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/sim/openmpi-1.10.0/ \
        -DCMAKE_C_COMPILER=$CC \
        -DCMAKE_CXX_COMPILER=$CXX \
        -DHWLOC_ROOT=$DEVROOT/install/hwloc/openmpi-1.10.0 \
        -DCORE_DIR=$DEVROOT/install/core/sim/openmpi-1.10.0 \
        -DSIM_MPI=openmpi110 \
        ..
#    -DSIMMETRIX_LIB_DIR=$DEVROOT/simPartitionWrapper/PartitionWrapper/lib \
#    --debug-output \
fi
# Special parameters for running the test on the Q
# are also passed to CMake in the prior if statement
# Locations of some mpi libraries
# (those that are found automatically by find_package(MPI) on monopoly)
# on the q if they are needed in the future. Set by using:
# SET(MPI_LIBRARIES ${MPI_LIBRARIES} example.a)
# /bgsys/drivers/ppcfloor/comm/gcc/lib/
# /bgsys/drivers/ppcfloor/gnu-linux/powerpc64-bgq-linux/lib/
