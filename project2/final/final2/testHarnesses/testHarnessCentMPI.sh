#!/bin/sh

#PBS -q cs6210
#PBS -l nodes=12:sixcore
#PBS -l walltime=00:05:00
#PBS -N MPI_P12
OMPI_MCA_mpi_yield_when_idle=0

cd $PBS_O_WORKDIR

#Centralized MPI Test Harness
#mpirun -np Number_Of_Processors ObjectFile NumberOfBarriers
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 12 centralized_mpi 100

