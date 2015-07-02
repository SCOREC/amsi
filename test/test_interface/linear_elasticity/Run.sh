#!/bin/bash

EXEDIR=$DEVROOT/amsi/build/test/test_interface/linear_elasticity/
FILEDIR=$DEVROOT/amsi/test/test_interface/

$1 mpirun $2 -np 1 $EXEDIR/linear_elastic \
  -l /net/common/meshSim/license/ \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets.sms \
  -p $FILEDIR/linear_elasticity/petsc_options
#  -c "uniaxial_extension_z_10kPa"
