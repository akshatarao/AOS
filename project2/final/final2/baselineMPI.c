/**
 *@author Akshata Rao
 */

/**
 *@file baselinempi.c
 */


#include "mpi.h"
#include <stdio.h>
#include<stdlib.h>

/**
 *@brief: Main Function 
 *@returns 0
*/
int main(int argc, char *argv[])
{

    int rank, nprocs;
    struct timeval startTime, endTime;
    double totalTime = 0;
    int numberOfBarriers;

    //Get the number of barriers in the command line argument
    numberOfBarriers = atoi(argv[1]);

    if(argc < 2)
    {
        printf("Syntax: %s numberOfBarriers");
        exit(1);
    }

    //Initialize MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int i = 0;
   
    //Iterate through barriers
    for(i = 0; i < numberOfBarriers; i++)
    {
        //Record start time for barrier
        gettimeofday(&startTime, NULL);

        //MPI Barrier
        MPI_Barrier(MPI_COMM_WORLD);

        //Record end time for barrier
        gettimeofday(&endTime, NULL);

        //Increment total time for all barriers  
        totalTime += ((double)(endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec));

    }

    //Get average time spent for a barrier
    printf("\nAverage Time: %lf", (float)totalTime/numberOfBarriers);

    MPI_Finalize();
    return 0;
}

