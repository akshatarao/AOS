/**
 *@author Akshata Rao
 */

/**
 *@file centralized_mpi.c
 */

#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<centralized_mpi.h>

FILE* fp;

/**
 *@brief Centralized Barrier Logic
 *@param thread - Thread structure
 *@param countOfThreads - Global Count of Threads
 *@param globalSense - Global Sense Variable
 *@returns none
 */
void centralizedBarrierLogic(Thread **thread, int* countOfThreads, int* globalSense, int barrierID)
{
   int i = 0;
   int master = *countOfThreads - 1;
   double startTime, endTime, totalTime;

  //Busy Wait	
  while(i < 9999)
      i++;

  //Record time when barrier begins
  startTime = MPI_Wtime();
 
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
	}while(recvd != COMPLETED);
	
	/*fp = fopen("test.log","a+");
	fprintf(fp,"\nReceived: %d from thread %d\n", recvd, (*thread)->rank);	
  	fclose(fp);*/
	//fflush(stdout);

        //Announce each time a thread completes
	counter++;
	MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
      }

      //Add last thread as completed
      counter++;
      MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
	      
      fp = fopen("test.log","a+");
      fprintf(fp,"\nCompleted all threads\n");
      fclose(fp);

      *globalSense = (*thread)->sense;	
      fp = fopen("test.log","a+"); 
      fprintf(fp,"\nReversing Global Sense %d\n", *globalSense);
      fclose(fp);
      
      endTime = MPI_Wtime(); 
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

     //Spin on the thread completed count
      do
      {
	
	do //As Bcast Receive is not blocking, wait until a valid counter value has been passed
	{
		threadCounter = 0;
		MPI_Bcast(&threadCounter, 1, MPI_INT, master, MPI_COMM_WORLD);
	}while(threadCounter == 0);

	(*thread)->count = threadCounter;	
		
      }while((*thread)->count < *countOfThreads);
	
      //Spin on sense reversal
      //Block the thread till it has received the Global Received Sense Flag from Master
      do
      {
	globalRcvdSense = 0;
	MPI_Bcast(&globalRcvdSense, 1, MPI_INT, master, MPI_COMM_WORLD);

      } while((globalRcvdSense == 0) || (globalRcvdSense != (*thread)->sense));

      endTime = MPI_Wtime();
      fp = fopen("test.log", "a+");
      fprintf(fp, "\nTotal time spent by Processor %d at barrier %d is %lf",(*thread)->rank, barrierID, (double)endTime - startTime);
      fclose(fp);
   /*   fp = fopen("test.log","a+");
      fprintf(fp,"\nGlobal Sense %d Received by %d of Local: %d\n", globalRcvdSense, (*thread)->rank, (*thread)->sense);
      fclose(fp);	 */
  }


}

/**
*@brief - Centralized MPI function
*@param - numberOfBarriers - Number of Barriers
*@returns - None
*/
void centralizedMPI(int argc, char* argv[], int numberOfBarriers)
{
    int i = 0, j, count = 0, countOfThreads, globalSense = 1;

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

        centralizedBarrierLogic(&thread, &countOfThreads, &globalSense, i);

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
   
 centralizedMPI(argc, argv, numberOfBarriers);
 return 0;
}
