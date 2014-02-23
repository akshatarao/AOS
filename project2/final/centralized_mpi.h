/**
*@author Akshata Rao
*/

/**
*@file centralized_mpi.h
*/
#ifndef CENTRALIZED_MPI_H
#define CENTRALIZED_MPI_H
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

#define NUMBER_OF_BARRIERS 1

//Sense Variable States
#define SENSE_0 5
#define SENSE_1 10

//Thread States
#define COMPLETED 1
#define NOT_COMPLETED 0 

/**
*
*Process Structure
*/
typedef struct process_struct
{
    int count; //local counter
    int sense; //local sense
    int localBarrierCounter; //localBarrierCounter
    int rank;//Processor ID
}Process;

void centralizedProcessorBarrierLogic(Process **process, int* countOfProcesses, int* globalSense, int barrierID);
void centralizedMPI(int argc, char* argv[], int numberOfBarriers);

#endif
