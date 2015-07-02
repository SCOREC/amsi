#!/bin/sh -ex

mpirun -np 32 ./test > test_result.txt
