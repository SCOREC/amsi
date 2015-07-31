#!/bin/bash

EXEDIR=$DEVROOT/amsi_sim/build/test/test_interface/mesh_adaptivity/
FILEDIR=$DEVROOT/amsi_sim/test/test_interface/

$1 mpirun $2 -np 2 $EXEDIR/uniform_adapt \
  -l /net/common/meshSim/license/ \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets_2_parts.sms \
  -p $FILEDIR/partitioned_linear_elasticity/petsc_options
