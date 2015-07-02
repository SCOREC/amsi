#!/bin/sh -ex


#sbatch -p normal -n 128 ./test-driver.sh
mpirun -np 128 ./test > test_result.txt
