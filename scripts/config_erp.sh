#!/bin/bash
  cmake \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DBUILD_TESTS=OFF \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=0 \
      -DCMAKE_INSTALL_PREFIX=/gpfs/u/home/PASC/PASCmrsn/scratch/erp/install/amsi  \
      -DCMAKE_C_COMPILER=`which mpicc` \
      -DCMAKE_CXX_COMPILER=`which mpicxx` \
      -DSCOREC_DIR="/gpfs/u/home/PASC/PASCmrsn/scratch/erp/install/core/lib/cmake/SCOREC/" \
      -DPETSC_DIR="$PETSC_DIR" \
      -DPETSC_ARCH=/ \
      -DENABLE_VERBOSITY:STRING=OFF \
      -DENABLE_ZOLTAN=OFF \
      /gpfs/u/home/PASC/PASCmrsn/barn/amsi

      #-DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=signed-integer-overflow" \
