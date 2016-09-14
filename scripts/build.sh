#!/bin/bash
ROOT=$DEVROOT/amsi
if [ -z $1 ]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=$1
fi
if [ "$BUILD_TYPE" == "Debug" ]; then
  BUILD_DIR=$ROOT/build_debug
elif [ "$BUILD_TYPE" == "Release" ] ; then
  BUILD_DIR=$ROOT/build_release
fi
if [ ! -d $BUILD_DIR ]; then
  mkdir $BUILD_DIR
fi
cd $BUILD_DIR
INSTALL_DIR=$DEVROOT/install/amsi/sim/bgq/xl/
rm -rf $INSTALL_DIR
make install
if [ "$?" == 0 ] ; then
  chmod -R g+rw $INSTALL_DIR
  chmod -R g+rw $BUILD_DIR
fi