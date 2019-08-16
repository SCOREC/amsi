#!/bin/bash
  cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_TESTS=OFF \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=0 \
      -DCMAKE_INSTALL_PREFIX=/gpfs/u/home/PASC/PASCmrsn/scratch/install-erp/amsi  \
      -DCMAKE_C_COMPILER=`which mpicc` \
      -DCMAKE_CXX_COMPILER=`which mpicxx` \
      -DSCOREC_DIR=/gpfs/u/home/PASC/PASCmrsn/scratch/install-erp/core/lib/cmake/SCOREC/ \
      -DPETSC_DIR="$PETSC_DIR" \
      -DPETSC_ARCH=/ \
      -DENABLE_VERBOSITY:STRING=OFF \
      -DZOLTAN_PREFIX="$PETSC_DIR" \
      -DPARMETIS_PREFIX="$PETSC_DIR" \
      -DPTSCOTCH_PREFIX="$PETSC_DIR" \
      -DBOOST_ROOT="$BOOST_INCLUDE_DIR" \
      /gpfs/u/home/PASC/PASCmrsn/barn/amsi
