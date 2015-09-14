#!/bin/bash

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
"../amsi_util/ ../amsi_meta/ ../amsi_interface/ ../amsi_control/ ../test/test_interface/linear_elasticity/ ../test/test_interface/nonlinear_elasticity/ ../test/test_interface/partitioned_linear_elastic/ ../test/test_interface/mesh_adaptivity/" \
'' \
'make install' \
"AMSI_$BUILD_TYPE"
