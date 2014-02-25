#!/bin/bash

threadcounts=( 2 4 6 8 )
for k in "${threadcounts[@]}"
do
  mpirun -np 2 ./cent_diss $k 1 1 > a$k.txt
done
