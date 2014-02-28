#!/bin/sh

#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:05:00
#PBS -N OMP_Cent_T6

cd $PBS_O_WORKDIR

#Baseline OMP Test Harness
#ObjectFile Number_of_Threads Number_of_Barriers
./baselineOMP 6 100

