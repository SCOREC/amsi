#!/bin/bash
# CMake config for AMSI
#
# usage: ./configamsi.sh [build_type] [install_prefix]
#

HOSTNAME=`hostname`

module load parmetis/OpenMPI-1.3.3
module load zoltan/OpenMPI-1.3.3
module unload parmetis/OpenMPI-1.3.3
module load ~/develop/petsc-3.5.3/arch-linux2-c-debug/lib/modules/3.5.3-arch-linux2-c-debug
module load simmetrix/simModSuite

if [ ! -d ../build ]; then
  mkdir ../build
fi

  cd ../build
  rm -rf ./* #stupid and dangerous

  cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=$DEVROOT/install \
    -DSIMMETRIX_BUILD=TRUE \
    -DSIM_MPI=openmpi14 \
    ..

# Special parameters for running the test on the Q 
# are also passed to CMake in the prior if statement

# Locations of some mpi libraries
# (those that are found automatically by find_package(MPI) on monopoly) 
# on the q if they are needed in the future. Set by using:
# SET(MPI_LIBRARIES ${MPI_LIBRARIES} example.a)
# /bgsys/drivers/ppcfloor/comm/gcc/lib/
# /bgsys/drivers/ppcfloor/gnu-linux/powerpc64-bgq-linux/lib/

