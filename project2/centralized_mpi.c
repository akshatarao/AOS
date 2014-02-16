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
   int master = 0;


   while(i < 9999)
      i++;

  
  (*thread)->sense = !(*thread)->sense;
  
  if(((*thread)->rank) == 0)
  {
      int counter = 0;
      int recvd = 0;
      MPI_Status status;

      while(counter != ((*countOfThreads) - 1))
      {
          
          MPI_Recv(&recvd, 1, MPI_INT, counter, 1, MPI_COMM_WORLD, &status);          counter++; 
      }

      //Reverse Sense Variable
      *globalSense = !*globalSense;
  }
  else
  {
      int sent = 0;
      MPI_Send(&sent, 1, MPI_INT, master, 1, MPI_COMM_WORLD);
  }

  //Receive or broadcast sense variable 
  MPI_Bcast(globalSense, 1, MPI_INT, master, MPI_COMM_WORLD);

  i = 0;
  while((*thread)->sense != *globalSense)
  {
      i++;
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

    numberOfBarriers = 3;

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
    
    
  return 0;
}
