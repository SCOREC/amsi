#!/bin/bash

EXEDIR=$DEVROOT/amsi/build_debug/test/interface/nonlinear_elasticity
FILEDIR=$DEVROOT/amsi/test/interface/

$1 mpirun $2 -np 1 $EXEDIR/nonlinear_elastic \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets.sms
