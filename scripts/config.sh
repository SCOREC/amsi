#!/bin/bash
# CMake config for AMSI
#
# usage: ./config.sh [build_type]
#

if [ -z $1 ]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=$1
fi

if [ "$BUILD_TYPE" == "Debug" ]; then
  BUILD_DIR=../build_debug
elif [ "$BUILD_TYPE" == "Release" ] ; then
  BUILD_DIR=../build_release
fi

if [ ! -d $BUILD_DIR ]; then
  mkdir $BUILD_DIR
fi
cd $BUILD_DIR
rm -rf ./* #stupid and dangerous

module load cmake

HOSTNAME=`hostname`
if [ "$HOSTNAME" == "q.ccni.rpi.edu" ]; then
  module load xl
  module load parmetis/xl/4.0.3
  module load zoltan/xl/3.8
  module unload parmetis/xl/4.0.3
  module load /gpfs/u/home/PASC/PASCtbnw/barn-shared/petsc-3.5.2/arch-linux2-c-opt/lib/modules/3.5.2-arch-linux2-c-opt
  module load proprietary/simmetrix/simModSuite/10.0-150716
  module load boost

  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/gpfs/u/home/PASC/PASCtbnw/barn-shared/install

   cmake \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_XL_TOOLCHAIN \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_C_COMPILER="mpicc" \
    -DCMAKE_CXX_COMPILER="mpicxx" \
    -DBUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/sim/xl/ \
    -DCORE_DIR=$DEVROOT/install \
    -DSIM_MPI=bgmpi \
    -DBOOST_INCLUDE_DIR=$BOOST_INCLUDE_DIR \
    ..

else
    module load openmpi/1.3.3
    module load simmetrix/simModSuite
    module load $DEVROOT/petsc-3.6.3/ompi133-debug/lib/petsc/conf/modules/petsc/3.6.3

    cmake \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_TESTS=ON \
	-DCMAKE_INSTALL_PREFIX=$DEVROOT/install/amsi/sim/openmpi-1.3.3/ \
	-DCMAKE_C_COMPILER="mpicc" \
	-DCMAKE_CXX_COMPILER="mpicxx" \
	-DCORE_DIR=$DEVROOT/install/core/sim/openmpi-1.3.3 \
	-DSIM_MPI=openmpi14 \
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

