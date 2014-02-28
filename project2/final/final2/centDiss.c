/**
 *@author Akshata Rao
 */

/**
 *@file centDiss.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<mpi.h>


struct timeval startTime, endTime;
double totalTime = 0.0;
/**
 *Thread Structure
 */
typedef struct thread_struct
{
    int receivingCount; //local counter
    int sentCount;
    int threadID;
}Thread;

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

/**
 *@brief - Dissemination Barrier Logic
 *@param - thread - Thread
 *@param - threadList - List of other threads
 *@param - threadID - Thread ID
 *@returns none
 */
void disseminationBarrier(Thread* thread,Thread** threadList,int threadId, int numberOfThreads)
{
    int r,s = 0,t,neighborIndex;
    int numOfRounds = ceil(log(numberOfThreads)/log(2));


    for(t=0;t<numOfRounds;t++) 
    {	

        #pragma omp critical
        {
            neighborIndex = fmod((threadId + pow(2,t)),numberOfThreads);
		
            Thread* neighborThread = *(threadList + neighborIndex);
		        thread->sentCount = thread->sentCount + 1;
		        neighborThread->receivingCount = neighborThread->receivingCount + 1;
	      }	
		
        while((*(threadList+threadId))->receivingCount < t+1);
		    while((*(threadList+threadId))->sentCount != t+1);	
	}
	
	while((*(threadList+threadId))->receivingCount != numOfRounds);
  while((*(threadList+threadId))->sentCount != numOfRounds);
        
  (*(threadList+threadId))->receivingCount = 0;
  (*(threadList+threadId))->sentCount = 0;
} 

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

}

/**
 *@brief Begin the threads that synchronize using dissemination barrier
 *@param numberOfThreads - Number of Threads
 *@param numberOfBarriers - Number of Barriers
 *@returns none
 */
void disseminationBarrierInit(int numberOfThreads, int numberOfBarriers)
{

    int k,l;
    
    int barrierArray[numberOfBarriers]; 
    omp_set_num_threads(numberOfThreads);
    int rounds = ceil(log(numberOfThreads)/log(2));
    Thread** threadList = malloc(numberOfThreads*sizeof(Thread*));
 
    for(k = 0; k < numberOfThreads; k++){
        Thread* thread;
        thread = (Thread*) malloc(sizeof(Thread));
        thread->receivingCount = 0;
        thread->sentCount = 0;
        thread->threadID = k;
        *(threadList + k) = thread;
    } 

    for(l = 0;l < numberOfBarriers; l++) 
     	barrierArray[l] =  0;

    //Thread Logic begins here
    #pragma omp parallel shared(numberOfThreads,rounds)
    {
      //Update numberOfThreads to the correct value

        int i = 0,j;
        
        numberOfThreads = omp_get_num_threads(); 
        int threadId = omp_get_thread_num();
        Thread* thread = *(threadList + threadId);
 
        for(i = 0; i <numberOfBarriers; i++) {
      
	  
           disseminationBarrier(thread,threadList,threadId, numberOfThreads);
           #pragma omp critical
           {
	          barrierArray[i] = barrierArray[i] + 1;
	         }
           
           while(barrierArray[i]!= numberOfThreads);
	    }
    }

}

/**
*@brief - Centralized MPI function
*@param - numberOfBarriers - Number of Barriers
*@returns - None
*/
void centralizedMPI(int argc, char* argv[], int numberOfBarriers, int numberOfThreads, int numberOfThreadBarriers)
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
	gettimeofday(&startTime,NULL);

	//Fork the threads on this processor - Await completion for this barrier
	disseminationBarrierInit(numberOfThreads, numberOfThreadBarriers);

        centralizedProcessorBarrierLogic(&process, &countOfProcesses, &globalSense, i);
	totalTime += ((double)(endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec));

    }
  
   printf("\n%lf", (double)totalTime/numberOfBarriers);
  //Finalize MPI  
  MPI_Finalize(); 
  
}


/**
 *@Main Function
 *@param - argv[1] - Number of Threads
 *@param - argv[2] - Number of Thread Barriers for dissemination Barriers
 *@param - argv[3] - Number of Processor Barriers for centralizedMPI Barriers
 */
int main(int argc, char** argv)
{
   int numberOfThreads;
   int numberOfThreadBarriers;
   int numberOfProcessorBarriers;

  if(argc < 4)
  {
    printf("\nSyntax: <executable> numberOfThreads numberOfThreadBarriers numberOfProcessorBarriers\n");
    exit(1);
  }
	
   numberOfThreads = atoi(argv[1]);
   numberOfThreadBarriers = atoi(argv[2]);
   numberOfProcessorBarriers = atoi(argv[3]);

   if(numberOfThreads <= 0 || numberOfThreadBarriers <= 0 || numberOfProcessorBarriers <= 0)
   {
       printf("\nAll inputs to program must be positive!");
       exit(1);
   }

    centralizedMPI(argc, argv, numberOfProcessorBarriers, numberOfThreads, numberOfThreadBarriers);
}    
