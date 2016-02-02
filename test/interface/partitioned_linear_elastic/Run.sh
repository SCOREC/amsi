#!/bin/bash

EXEDIR=$DEVROOT/amsi/build_debug/test/interface/partitioned_linear_elastic/
FILEDIR=$DEVROOT/amsi/test/interface/

$1 mpirun $2 -np 2 $EXEDIR/partitioned_linear_elastic \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets_2_parts.sms