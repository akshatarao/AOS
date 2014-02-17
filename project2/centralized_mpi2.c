/**
 *@author Akshata Rao
 */

/**
 *@file centralized_mpi.c
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int numberOfThreads;
int threadCounter;
int numberOfBarriers;
int barrierCounter;
int globalSense = 0;

typedef struct thread_struct
{
    int count; //local counter
    int sense; //local sense
    int localBarrierCounter; //localBarrierCounter
    int rank;//Processor ID
}Thread;


/**
 *@brief Centralized Barrier Logic
 *@param thread - Thread structure
 *@param countOfThreads - Global Count of Threads
 *@param globalSense - Global Sense Variable
 *@returns none
 */
void centralizedBarrierLogic(Thread **thread, int* countOfThreads, int* globalSense)
{
   int i = 0;
   int master = *countOfThreads - 1;


   while(i < 9999)
      i++;

  
  (*thread)->sense = !(*thread)->sense;
  
  if(((*thread)->rank) == master)
  {
      int counter = 0;
      int recvd;
      MPI_Status status;

      while(counter < ((*countOfThreads) - 1))
      {
         
	//printf("\nReceiving data from Process %d", counter); 
     	  MPI_Recv(&recvd, 1, MPI_INT, counter, 1, MPI_COMM_WORLD, &status);          //counter++; 
	//printf("\nReceived data %d from Process %d", recvd, counter);	
	counter++;
      }

      //Reverse Sense Variable
      *globalSense = (*thread)->sense;
      //printf("\nReversing Global Sense %d", *globalSense);
  
      int i = 0;
      
      while(i < ((*countOfThreads)-1))
      {
	 MPI_Send(globalSense, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      } 		
  }
  else
  {
      int sent = 1;
      //printf("\nSending Data from process %d", (*thread)->rank);
      MPI_Send(&sent, 1, MPI_INT, master, 1, MPI_COMM_WORLD);
      
      int globalRcvdSense;
      MPI_Recv(&globalRcvdSense, 1, MPI_INT, master, 1, MPI_COMM_WORLD, &status); 	
	printf("\nGlobal Sense %d Received by %d of Local: %d", globalRcvdSense, (*thread)->rank, (*thread)->sense);

  }


}


/**
 *@Main Function
 */
int main(int argc, char *argv[])
{
    int i = 0, j, count = 0, countOfThreads, numberOfBarriers, globalSense = 1;
   
    //printf("\nEnter the number of barriers:");
    //scanf("%d", &numberOfBarriers);

    numberOfBarriers = 2;

    if(numberOfBarriers <= 0)
    {
        printf("\nERROR: Number of barriers cannot be negative!");
        exit(1);
    }

  	MPI_Init(&argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &countOfThreads);

    Thread* thread;
    thread = (Thread*)malloc(sizeof(Thread));
    thread->sense = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &((thread)->rank));

    for(i=0; i < numberOfBarriers; i++)
    {
        j = 0;

        while(j < 9999)
        {
            j++;
        }
         printf("\nEntered thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);

        centralizedBarrierLogic(&thread, &countOfThreads, &globalSense);

        j = 0;
        
        while(j < 9999)
        {
            j++;
        }

        printf("\nCompleted thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);
    }
    
   MPI_Finalize(); 
  return 0;
}
