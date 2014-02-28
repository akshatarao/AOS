#!/bin/sh

#PBS -q cs6210
#PBS -l nodes=4:sixcore
#PBS -l walltime=00:05:00
#PBS -N MPI_P4_T8_100000
OMPI_MCA_mpi_yield_when_idle=0

cd $PBS_O_WORKDIR
#Centralized MPI - Dissemination OMP - Merged Barrier
#mpirun -np Number_of_Processes ObjectFile Number_of_Threads Number_of_OMP_Barriers Number_of_MPI_Barriers
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 4 cent_diss 8 1 100000



