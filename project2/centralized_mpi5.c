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

//Sense Variable States
#define SENSE_0 5
#define SENSE_1 10

//Thread States
#define COMPLETED 1
#define NOT_COMPLETED 0 

int numberOfThreads;
int threadCounter;
int numberOfBarriers;
int barrierCounter;
int globalSense = 0;

/**
*
*Thread Structure
*/
typedef struct thread_struct
{
    int count; //local counter
    int sense; //local sense
    int localBarrierCounter; //localBarrierCounter
    int rank;//Processor ID
}Thread;

FILE* fp;

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

  //Flip the value of thread sense
  (*thread)->sense = ((*thread)->sense == SENSE_0 )? SENSE_1 : SENSE_0;

  //If the thread is the Master Thread awaiting other threads 
  if(((*thread)->rank) == master)
  {
      int counter = 0;
      int recvd = 0;
      MPI_Status status;

      //Await the other threads
      while(counter < ((*countOfThreads) - 1))
      {
        
	//Wait till you receive a completed signal from each of the threads 
	do  
	{
		MPI_Recv(&recvd, 1, MPI_INT, counter, 1, MPI_COMM_WORLD, &status);         
	//	printf("\nReceived: %d", recvd); 
	}while(recvd != COMPLETED);
	
	/*fp = fopen("test.log","a+");
	fprintf(fp,"\nReceived: %d from thread %d\n", recvd, (*thread)->rank);	
  	fclose(fp);*/
	//fflush(stdout);

	counter++;
	MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
      }

      //Reverse Sense Variable
      *globalSense = (*thread)->sense;

      counter++;
      MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
	      
      fp = fopen("test.log","a+");
      fprintf(fp,"\nCompleted all threads\n");
      fclose(fp);

//      fflush(stdout);
      fp = fopen("test.log","a+"); 
      fprintf(fp,"\nReversing Global Sense %d\n", *globalSense);
      fclose(fp);
//      fflush(stdout);

      //As all threads have completed, broadcast the green signal! 
      MPI_Bcast(globalSense, 1, MPI_INT, master, MPI_COMM_WORLD);
  }
  else//For a non-master thread
  {
      MPI_Status status;

      int sent = COMPLETED;

      //As the thread has completed, broadcast the go signal
      MPI_Send(&sent, 1, MPI_INT, master, 1, MPI_COMM_WORLD);
      
      int globalRcvdSense = 0;

      int threadCounter;    

      do
      {
	
	do
	{
		threadCounter = 0;
		MPI_Bcast(&threadCounter, 1, MPI_INT, master, MPI_COMM_WORLD);
	}while(threadCounter == 0);

	(*thread)->count = threadCounter;	
		
      }while((*thread)->count < *countOfThreads);
	
      //Block the thread till it has received the Global Received Sense Flag from Master
      do
      {
	globalRcvdSense = 0;
	MPI_Bcast(&globalRcvdSense, 1, MPI_INT, master, MPI_COMM_WORLD);

      } while((globalRcvdSense == 0) || (globalRcvdSense != (*thread)->sense));

   /*   fp = fopen("test.log","a+");
      fprintf(fp,"\nGlobal Sense %d Received by %d of Local: %d\n", globalRcvdSense, (*thread)->rank, (*thread)->sense);
      fclose(fp);	 */
      //fflush(stdout);	
  }


}


/**
 *@Main Function
 */
int main(int argc, char *argv[])
{
    int i = 0, j, count = 0, countOfThreads, numberOfBarriers, globalSense = 1;

//    fp = fopen("test.log", "a");	   
    numberOfBarriers = NUMBER_OF_BARRIERS;

    //Initialize the MPI datastructures 	
    MPI_Init(&argc, &argv);
    
    //Check what is the number of threads that have been allocated
    MPI_Comm_size( MPI_COMM_WORLD, &countOfThreads);

    //Initialize the thread rank
    Thread* thread;
    thread = (Thread*)malloc(sizeof(Thread));
    thread->sense = SENSE_1;
    MPI_Comm_rank(MPI_COMM_WORLD, &((thread)->rank));

    //Iterate through the barriers
    for(i=0; i < numberOfBarriers; i++)
    {
        j = 0;

        //Busy Wait
        while(j < 9999)
        {
            j++;
        }
        
	fp = fopen("test.log", "a+"); 
        fprintf(fp,"\nEntered thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);
        fclose(fp);	
	//fflush(stdout);

        centralizedBarrierLogic(&thread, &countOfThreads, &globalSense);

        //Busy Wait
        j = 0;
        
        while(j < 9999)
        {
            j++;
        }

        //printf("\nCompleted thread %d  of %d threads at barrier %d", thread->rank, countOfThreads, i);
    }
  
  //Finalize MPI  
  MPI_Finalize(); 
  
  return 0;
}
