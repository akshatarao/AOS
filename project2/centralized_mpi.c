/**
 *@author Akshata Rao
 */

/**
 *@file centralized_mpi.c
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

#define NUMBER_OF_BARRIERS 3

int numberOfThreads; //Number of threads
int threadCounter; //Counter for Threads
int numberOfBarriers; //Number for Barriers
int barrierCounter; //Counter for Barriers
int globalSense = 0; //Global Sense

//Thread Structure
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


  //Busy Wait	
   while(i < 9999)
      i++;

  //Reverse Thread Sense
  (*thread)->sense = !(*thread)->sense;
  
  //If the Thread is the main Master
  if(((*thread)->rank) == master)
  {
      int counter = 0;
      int recvd;
      MPI_Status status;

      //Wait till all the other threads have completed
      while(counter < ((*countOfThreads) - 1))
      {
         
	//printf("\nReceiving data from Process %d", counter); 
	//printf("\nReceived data %d from Process %d", recvd, counter);	
	counter++;
      }

      //Reverse Sense Variable
      *globalSense = (*thread)->sense;
      printf("\nReversing Global Sense %d", *globalSense);

      int i = 0;
      
      // printf("\nSending Global Sense %d", *globalSense); 
      MPI_Bcast(globalSense, 1, MPI_INT, master, MPI_COMM_WORLD);
  }
  else
  {
      MPI_Status status;

      int sent = 1;
      //printf("\nSending Data from process %d", (*thread)->rank);
      MPI_Send(&sent, 1, MPI_INT, master, 1, MPI_COMM_WORLD);
      printf("\nThread work completed %d", (*thread)->rank);	
	 
      int globalRcvdSense = 4;//arbitrary value
      
      do
      {
        //printf("\n%d Trying to Receive from master", (*thread)->rank);	
	MPI_Bcast(&globalRcvdSense, 1, MPI_INT, master, MPI_COMM_WORLD); 	
      } while(globalRcvdSense != (*thread)->sense);
	
       	//printf("\nGlobal Sense %d Received by %d of Local: %d", globalRcvdSense, (*thread)->rank, (*thread)->sense);
	
	
  }


}

void centralizedMPI(int numberOfBarriers)
{
    int i = 0, j, count = 0, countOfThreads, globalSense = 1;
   
    //printf("\nEnter the number of barriers:");
    //scanf("%d", &numberOfBarriers);


    //Initialize MPI Datastructures
    MPI_Init(&argc, &argv);

   //Obtain the number of threads finally allocated
    MPI_Comm_size( MPI_COMM_WORLD, &countOfThreads);

    Thread* thread;
    thread = (Thread*)malloc(sizeof(Thread));
    thread->sense = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &((thread)->rank));

    //Iterating through the barriers
    for(i=0; i < numberOfBarriers; i++)
    {
        j = 0;

	//Busy Wait
        while(j < 9999)
        {
            j++;
        }

        printf("\nEntered thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);

        //Enter thread + barrier logic
	centralizedBarrierLogic(&thread, &countOfThreads, &globalSense);

        j = 0;
        
	//Busy Wait
        while(j < 9999)
        {
            j++;
        }

 //       printf("\nCompleted thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);
    }
    
  MPI_Finalize(); 
  
  return 0;
}

/**
 *@Main Function
 */
int main(int argc, char *argv[])
{
   numberOfBarriers = NUMBER_OF_BARRIERS;

   //Incase Number of Barriers is taken as user input
    if(numberOfBarriers <= 0)
    {
        printf("\nERROR: Number of barriers cannot be negative!");
        exit(1);
    }
   
 centralizedMPI(numberOfBarriers);
 return 0;
}
