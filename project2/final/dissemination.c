#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>


int numberOfThreads;
int threadCounter;
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
  int r,s = 0,t,index,neighborIndex;
  printf("total num of Threads%d\n", numberOfThreads);
  int numOfRounds = ceil(log(numberOfThreads)/log(2));

  //Busy Wait
  while(s < 9999)
      s++;

      printf("index = %d\n",index);
      for(t=0;t<numOfRounds;t++)
	{	//mutex lock
		neighborIndex = fmod((threadId + pow(2,t)),numberOfThreads);
		pthread_mutex_lock(&lock);
		Thread* neighborThread = *(threadList + neighborIndex);
		neighborThread->receivingCount = neighborThread->receivingCount + 1;
		pthread_mutex_unlock(&lock);
		//Thread* curr
		while((*(threadList+threadId))->receivingCount < t+1);
	}

} 

int main(int argc, char** argv)
{

  double startTime,endTime;

  printf("\nEnter the number of threads:");
  scanf("%d", &numberOfThreads);

  printf("\nEnter the number of barriers:");
  scanf("%d", &numberOfBarriers);

  if(numberOfThreads <=0 || numberOfBarriers <= 0)
  {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
  }


  pthread_mutex_init(&lock,NULL);

  int k;
  omp_set_num_threads(numberOfThreads);
  int rounds = ceil(log(numberOfThreads)/log(2));
  Thread** threadList = malloc(numberOfThreads*sizeof(Thread*));

  //Thread Logic begins here
  #pragma omp parallel shared(numberOfThreads,rounds)
  {
      //Update numberOfThreads to the correct value
      numberOfThreads = omp_get_num_threads(); 
      threadCounter = numberOfThreads;
      int threadId = omp_get_thread_num();

      Thread* thread;
      thread = (Thread*)malloc(sizeof(Thread));
      thread->receivingCount = 0;
      thread->sentCount = 0;
      thread->threadID = omp_get_thread_num();

      *(threadList + threadId) = thread;	
      

      int i = 0, j =0,loopCount =0, threadID;

      for(i = 0; i <numberOfBarriers; i++)
      {
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }
        
          printf("\nEntered thread %d  of %d threads at barrier %d", thread->threadID, numberOfThreads, i);
	  

	  /*for(k = 0;k < numberOfThreads;k++){
		for(loopCount = 0;loopCount < rounds;loopCount++){
	 		sentCount++;
			next = (k + pow(2,loopCount))%numOfThreads;*/
			startTime = omp_get_wtime();
          		disseminationBarrier(thread,threadList,threadId);
			endTime = omp_get_wtime();
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }

          printf("\nCompleted thread %d of %d threads at barrier %d", thread->threadID, numberOfThreads, i);

      }   
  printf("Time spent in barrier by thread %d is %f\n", thread->threadID, endTime-startTime);
  }

  return 0;
}
