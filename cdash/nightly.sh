#!/bin/bash

# load modules to get packages that biotissue/amsi were compiled with
source $DEVROOT/config_modules.sh

#cdash output root
cd $DEVROOT/test/amsi/cdash
#remove compilation directories created by previous nightly.cmake runs
rm -rf build/

#run nightly.cmake script
ctest --output-on-failure --script $DEVROOT/amsi/cdash/jenga-nightly.cmake &> cmake_log.txt
