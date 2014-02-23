#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<mpi.h>
#include<centralized_mpi.h>


#define NUMBER_OF_THREADS 2
#define NUMBER_OF_THREAD_BARRIERS 1

int numberOfThreads;
int numberOfBarriers;

pthread_mutex_t lock;

typedef struct thread_struct
{
    int receivingCount; //local counter
    int sentCount;
    int threadID;
}Thread;

void disseminationBarrier(Thread* thread,Thread** threadList,int threadId)
{
    int r,s = 0,t,neighborIndex;
    int numOfRounds = ceil(log(numberOfThreads)/log(2));

    //Busy Wait
    while(s < 9999)
        s++;

    for(t=0;t<numOfRounds;t++) {	
        pthread_mutex_lock(&lock);
		neighborIndex = fmod((threadId + pow(2,t)),numberOfThreads);
		printf("Thread %d has neighbor index %d in round %d\n",threadId, neighborIndex, t);
		Thread* neighborThread = *(threadList + neighborIndex);
		thread->sentCount = thread->sentCount + 1;
		neighborThread->receivingCount = neighborThread->receivingCount + 1;
		printf("%d thread's receiving count is %d\n",thread->threadID,thread->receivingCount);
		printf("%d Neighbor thread's receiving count is %d\n",neighborThread->threadID,neighborThread->receivingCount);
		printf("%d thread's sent count is %d\n",thread->threadID,thread->sentCount);
		pthread_mutex_unlock(&lock);
		while((*(threadList+threadId))->receivingCount < t+1);
		while((*(threadList+threadId))->sentCount != t+1);	
	}
	
        printf("%d exit check begin\n", thread->threadID);
	while((*(threadList+threadId))->receivingCount != numOfRounds);
        while((*(threadList+threadId))->sentCount != numOfRounds);
        (*(threadList+threadId))->receivingCount = 0;
        (*(threadList+threadId))->sentCount = 0;
        printf("%d exit check end\n", thread->threadID);
}

void dissemination_ThreadInit()
{

    numberOfThreads = NUMBER_OF_THREADS;
    numberOfBarriers = NUMBER_OF_THREAD_BARRIERS;

    pthread_mutex_init(&lock,NULL);

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
      double startTime, endTime;
        
      numberOfThreads = omp_get_num_threads(); 
        int threadId = omp_get_thread_num();
        Thread* thread = *(threadList + threadId);
 
        for(i = 0; i <numberOfBarriers; i++) {
          //Busy Wait
            j = 0;
            while(j < 9999){
                j++;
            }
            printf("Entered thread %d of %d threads at barrier %d\n", thread->threadID, numberOfThreads-1, i);
	  
	        startTime = omp_get_wtime();
           disseminationBarrier(thread,threadList,threadId);
	   printf("Completed thread %d of %d threads at barrier %d\n", thread->threadID, numberOfThreads-1, i);
	   pthread_mutex_lock(&lock);
	   barrierArray[i] = barrierArray[i] + 1;
	   printf("barrierCheck is %d\n", barrierArray[i]);  
	   pthread_mutex_unlock(&lock);
           while(barrierArray[i]!= numberOfThreads);
	   endTime = omp_get_wtime();     
	}
        printf("Time spent in barrier by thread %d is %f\n",thread->threadID, endTime-startTime);
    }
    
}

void centralizedProcessorBarrierLogic(Process **process, int* countOfProcesses, int* globalSense, int barrierID)
{
   int i = 0;
   int master = *countOfProcesses - 1;
   struct timeval startTime, endTime;

  //Busy Wait	
  while(i < 9999)
      i++;
  
  dissemination_ThreadInit();
  printf("\nThreads complete for barrier %d", barrierID);

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
	
	/*fp = fopen("test.log","a+");
	fprintf(fp,"\nReceived: %d from process %d\n", recvd, (*process)->rank);	
  	fclose(fp);*/
	//fflush(stdout);

        //Announce each time a process completes
	counter++;
	MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
      }

      //Add last process as completed
      counter++;
      MPI_Bcast(&counter, 1, MPI_INT, master, MPI_COMM_WORLD);
	      
//      fp = fopen("test.log","a+");
      //printf("\nCompleted all processes\n");
  //    fclose(fp);

      *globalSense = (*process)->sense;	
//      fp = fopen("test.log","a+"); 
      //printf("\nReversing Global Sense %d\n", *globalSense);
  //    fclose(fp);
      
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

   /*   fp = fopen("test.log","a+");
      fprintf(fp,"\nGlobal Sense %d Received by %d of Local: %d\n", globalRcvdSense, (*process)->rank, (*process)->sense);
      fclose(fp);	 */
  }
    //  fp = fopen("test.log", "a+");
      printf("\nTotal time spent by Processor %d at barrier %d is %lf",(*process)->rank, barrierID, (double)(endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec));
     // fclose(fp);
}

/*
**@Main Function
*/ 
int main(int argc, char *argv[])
{
   int numberOfBarriers = NUMBER_OF_BARRIERS;

   //Incase Number of Barriers is taken as user input
    if(numberOfBarriers <= 0)
    {
        //printf("\nERROR: Number of barriers cannot be negative!");
        exit(1);
    }
   
 centralizedMPI(argc, argv, numberOfBarriers);
 return 0;
}

