#!/bin/bash
cd ../build

notifybuild \
"../amsi_util/ ../amsi_meta/ ../amsi_interface/ ../amsi_control/ ../test/test_interface/linear_elasticity/ ../test/test_interface/nonlinear_elasticity/ ../test/test_interface/partitioned_linear_elastic/" \
'' \
'make install' \
AMSI
