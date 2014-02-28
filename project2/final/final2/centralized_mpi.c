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
double totalTime = 0.0;

/**
 *@brief Centralized Processor Barrier Logic
 *@param process - Process structure
 *@param countOfProcesses - Global Count of Processes
 *@param globalSense - Global Sense Variable
 *@returns none
 */
void centralizedProcessorBarrierLogic(Process **process, int* countOfProcesses, int* globalSense, int barrierID)
{
   int i = 0;
   int master = *countOfProcesses - 1;
   struct timeval startTime, endTime;

  //Busy Wait	
  while(i < 9999)
      i++;

  //Record time when barrier begins
  gettimeofday(&startTime, NULL);
 
  //Flip the value of process sense
  (*process)->sense = ((*process)->sense == SENSE_0 )? SENSE_1 : SENSE_0;

  //If the process is the Master Process awaiting other processes 
  if(((*process)->rank) == master)
  {
      int counter = 0;
      int recvd = 0;
      MPI_Status status;

      //Await the other processes
      while(counter < ((*countOfProcesses) - 1))
      {
        
	//Wait till you receive a completed signal from each of the processes 
	do  
	{
		MPI_Recv(&recvd, 1, MPI_INT, counter, 1, MPI_COMM_WORLD, &status);         
	}while(recvd != COMPLETED);
	

        //Announce each time a process completes
	counter++;
	MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
      }

      //Add last process as completed
      counter++;
      MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
	      
      *globalSense = (*process)->sense;	
      
      gettimeofday(&endTime, NULL); 
      //As all processes have completed, broadcast the green signal! 
      MPI_Bcast(globalSense, 1, MPI_INT, master, MPI_COMM_WORLD);
  }
  else//For a non-master process
  {
      MPI_Status status;

      int sent = COMPLETED;

      //As the process has completed, broadcast the go signal
      MPI_Send(&sent, 1, MPI_INT, master, 1, MPI_COMM_WORLD);
      
      int globalRcvdSense = 0;

      int processCounter;    

     //Spin on the process completed count
      do
      {
	
	do //As Bcast Receive is not blocking, wait until a valid counter value has been passed
	{
		processCounter = 0;
		MPI_Bcast(&processCounter, 1, MPI_INT, master, MPI_COMM_WORLD);
	}while(processCounter == 0);

	(*process)->count = processCounter;	
		
      }while((*process)->count < *countOfProcesses);
	
      //Spin on sense reversal
      //Block the process till it has received the Global Received Sense Flag from Master
      do
      {
	globalRcvdSense = 0;
	MPI_Bcast(&globalRcvdSense, 1, MPI_INT, master, MPI_COMM_WORLD);

      } while((globalRcvdSense == 0) || (globalRcvdSense != (*process)->sense));

      gettimeofday(&endTime, NULL);       

  }

   totalTime += ((double)(endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec));  	

}

/**
*@brief - Centralized MPI function
*@param - numberOfBarriers - Number of Barriers
*@returns - None
*/
void centralizedMPI(int argc, char* argv[], int numberOfBarriers)
{
    int i = 0, j, count = 0, countOfProcesses, globalSense = 1;

    //Initialize the MPI datastructures 	
    MPI_Init(&argc, &argv);
    
    //Check what is the number of processes that have been allocated
    MPI_Comm_size( MPI_COMM_WORLD, &countOfProcesses);

    //Initialize the process rank
    Process* process;
    process = (Process*)malloc(sizeof(Process));
    process->sense = SENSE_1;
    MPI_Comm_rank(MPI_COMM_WORLD, &((process)->rank));

    //Iterate through the barriers
    for(i=0; i < numberOfBarriers; i++)
    {
        j = 0;

        //Busy Wait
        while(j < 9999)
        {
            j++;
        }
        
        centralizedProcessorBarrierLogic(&process, &countOfProcesses, &globalSense, i);

        //Busy Wait
        j = 0;
        
        while(j < 9999)
        {
            j++;
        }

    }
  
  printf("\nAverage Time: %lf", (float)totalTime/numberOfBarriers); 
  //Finalize MPI  
  MPI_Finalize(); 
  
}
/*
**
 *@Main Function
*/ 
int main(int argc, char *argv[])
{

	if(argc < 2)
	{
         	printf("\nSyntax: %s numberOfBarriers", argv[0]);
		exit(1);
	}
    
     int   numberOfBarriers = atoi(argv[1]);
    
       //Incase Number of Barriers is taken as user input
       if(numberOfBarriers <= 0)
       {   
         printf("\nERROR: Number of barriers cannot be negative!");
         exit(1);
       }
   
 centralizedMPI(argc, argv, numberOfBarriers);
 return 0;
}
