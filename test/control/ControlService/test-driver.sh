#!/bin/sh -ex

$1 mpirun $2 -np 32 ./test -a amsi_options_8_24
