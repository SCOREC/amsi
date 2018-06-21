#!/bin/bash -e 
usage="Usage: <path to petsc source>"
[ $# -ne 1 ] && echo $usage && exit 1
src=$1

#-qnosave
cd ${src}
set -x
./configure \
  --known-sdot-returns-double=0 \
  --known-snrm2-returns-double=0 \
  --with-x=0 \
  --with-ssl=0 \
  --with-shared-libraries=0 \
  --with-debugging=false \
  --with-cc=/bgsys/drivers/ppcfloor/comm/xl/bin/mpixlc_r \
  --with-cxx=/bgsys/drivers/ppcfloor/comm/xl/bin/mpixlcxx_r \
  --with-fc=/bgsys/drivers/ppcfloor/comm/xl/bin/mpixlf77_r\
  --with-fortran-kernels=1 \
  --with-color-is-value-type=short \
  --with-batch=1 \
  --known-bits-per-byte=8 \
  --known-level1-dcache-assoc=0 \
  --known-level1-dcache-size=16384 \
  --known-level1-dcache-linesize=64 \
  --with-memalign=4 \
  --known-endian=big \
  --known-memcmp-ok=1 \
  --known-mpi-int64_t=1 \
  --known-mpi-c-double-complex=1 \
  --known-mpi-long-double=1 \
  --known-mpi-shared-libraries=0 \
  --known-sizeof-MPI_Comm=4 \
  --known-sizeof-MPI_Fint=4 \
  --known-sizeof-char=1 \
  --known-sizeof-double=8 \
  --known-sizeof-float=4 \
  --known-sizeof-int=4 \
  --known-sizeof-long-long=8 \
  --known-sizeof-long=8 \
  --known-sizeof-short=2 \
  --known-sizeof-size_t=8 \
  --known-sizeof-void-p=8 \
  --known-64-bit-blas-indices=0 \
  -COPTFLAGS=" -O3 -qhot=level=0 -qsimd=auto -qmaxmem=-1 -qstrict -qstrict_induction" \
  -CXXOPTFLAGS=" -O3 -qhot=level=0 -qsimd=auto -qmaxmem=-1 -qstrict -qstrict_induction" \
  -FOPTFLAGS=" -O3 -qhot=level=0 -qsimd=auto -qmaxmem=-1 -qstrcit -qstrict_induction" \
  --download-superlu_dist=/gpfs/u/scratch/PASC/shared/petsc/petsc-3.9.2/externalpackages/superlu_dist-5.3.tar.gz \
  --download-parmetis=/gpfs/u/scratch/PASC/shared/petsc/petsc-3.9.2/externalpackages/petsc-pkg-parmetis-4.0.3-p3.tar.gz \
  --download-metis=/gpfs/u/scratch/PASC/shared/petsc/petsc-3.9.2/externalpackages/petsc-pkg-metis-5.1.0-p3.tar.gz \
  --download-zoltan=/gpfs/u/scratch/PASC/shared/petsc/petsc-3.9.2/externalpackages/zoltan_distrib_v3.83.tar.gz \
  --download-ptscotch=/gpfs/u/scratch/PASC/shared/petsc/petsc-3.9.2/externalpackages/scotch_6.0.4-p1.tar.gz \
  --with-blacs-include=/gpfs/u/software/bgq/scalapack/xl/2.0.2/ \
  --with-blacs-lib=/gpfs/u/software/bgq/scalapack/xl/2.0.2/lib/libscalapack.a \
  --with-blas-lapack-lib="-L/gpfs/u/software/bgq/lapack/xl/3.5.0/lib -llapack -L/gpfs/u/software/bgq/lapack/xl/3.5.0/lib -lblas" \
  --prefix=/gpfs/u/scratch/PASC/shared/install/petsc-3.9.2/xl/
set +x