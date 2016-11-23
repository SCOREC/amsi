#!/bin/bash -x

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

notifybuild \
"../util/ ../meta/ ../analysis/ ../control/ ../test/analysis/linear_elasticity/ ../test/analysis/nonlinear_elasticity/ ../test/analysis/boundary_conditions/neumann/ ../test/analysis/boundary_conditions/dirichlet ../test/analysis/mesh_adaptivity/" \
'' \
'make install' \
"AMSI_$BUILD_TYPE"
