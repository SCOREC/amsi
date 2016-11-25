#!/bin/bash -x

EXEDIR=$DEVROOT/amsi/build_debug/test/interface/nonlinear_elasticity
FILEDIR=$DEVROOT/amsi/test/interface/

$1 mpirun $2 -np 1 $EXEDIR/nonlinear_elastic \
  $FILEDIR/models/cube_uniaxial_10kPa.smd \
  $FILEDIR/meshes/cube_24_tets.sms
