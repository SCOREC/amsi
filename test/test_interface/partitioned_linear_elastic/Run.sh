#!/bin/bash

EXEDIR=$DEVROOT/amsi_sim/build_debug/test/test_interface/partitioned_linear_elastic/
FILEDIR=$DEVROOT/amsi_sim/test/test_interface/

$1 mpirun $2 -np 2 $EXEDIR/partitioned_linear_elastic \
  -l /net/common/meshSim/license/ \
  -g $DEVROOT/biotissue/macro/test/dogBone/models/dirichlet/dogBone.smd \
  -m $DEVROOT/biotissue/macro/test/dogBone/meshes/dogbone_2_parts.sms \
  -p $FILEDIR/partitioned_linear_elasticity/petsc_options
