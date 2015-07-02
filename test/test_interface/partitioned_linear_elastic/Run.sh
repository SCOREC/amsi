#!/bin/bash

EXEDIR=$DEVROOT/amsi/build/test/test_interface/partitioned_linear_elastic/
FILEDIR=$DEVROOT/amsi/test/test_interface/

$1 mpirun $2 -np 2 $EXEDIR/partitioned_linear_elastic \
  -l /net/common/meshSim/license/ \
  -g $FILEDIR/models/cube_uniaxial_10kPa.smd \
  -m $FILEDIR/meshes/cube_24_tets_2_parts.sms \
  -p $FILEDIR/partitioned_linear_elasticity/petsc_options
#  -c "uniaxial_extension_z_10kPa"
