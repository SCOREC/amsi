#!/bin/bash

EXEDIR=$DEVROOT/amsi/build_debug/test/interface/dirichlet_bc/
FILEDIR=$DEVROOT/amsi/test/interface/

$1 mpirun $2 -np 1 $EXEDIR/dirichlet_test \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets.sms \
  -d $FILEDIR/dirichlet_bc/dirichlet_spec.txt
