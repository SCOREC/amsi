#!/bin/sh -ex

mpirun -np 8 ./test > test_result.txt
