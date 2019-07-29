#!/bin/bash

#mpicc=/gpfs/u/home/PASC/PASCmrsn/scratch/test_compile/mpicc
#mpicxx=/gpfs/u/home/PASC/PASCmrsn/scratch/test_compile/mpicxx

  cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_TESTS=OFF \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=0 \
      -DCMAKE_INSTALL_PREFIX=/gpfs/u/home/PASC/PASCmrsn/scratch/dcs/install/amsi  \
      -DCMAKE_C_COMPILER=`which mpicc` \
      -DCMAKE_CXX_COMPILER=`which mpicxx` \
      -DSCOREC_DIR=/gpfs/u/home/PASC/PASCmrsn/scratch/dcs/install/core/lib/cmake/SCOREC/ \
      -DPETSC_DIR="$PETSC_DIR" \
      -DPETSC_ARCH="$PETSC_ARCH" \
      -DENABLE_VERBOSITY:STRING=OFF \
      -DENABLE_ZOLTAN=OFF \
      -DCMAKE_CXX_FLAGS="-Ofast -Wall" \
      /gpfs/u/home/PASC/PASCmrsn/barn/amsi

      #-DBOOST_ROOT=/gpfs/u/barn/PASC/shared/boost_1_56_0/ \
      #-DZOLTAN_PREFIX="$PETSC_DIR" \
      #-DPARMETIS_PREFIX="$PETSC_DIR" \
      #-DPTSCOTCH_PREFIX="$PETSC_DIR" \
