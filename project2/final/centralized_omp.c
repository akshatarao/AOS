/**
 *@author Akshata Rao
**/

/**
 *@file centralized_omp.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<centralized_omp.h>

/**
 *@brief Centralized Thread Barrier Logic
 *@param thread - Thread structure
 *@param countOfThreads - Global Count of Threads
 *@param globalSense - Global Sense Variable
 *@returns none
 */

int globalThreadSense = 0;
void centralizedThreadBarrierLogic(Thread **thread, int* countOfThreads, int* globalSense)
{
  int i = 0;

  //Busy Wait
  while(i < 9999)
      i++;

  //Toggle its local sense
  (*thread)->sense = !(*thread)->sense;

  #pragma omp critical
  {
      *countOfThreads = *countOfThreads - 1;
      //printf("\nThread Counter: %d", *countOfThreads);
  
  
    if(*countOfThreads == 0)
    {
       printf("\nSwitching Global Sense to %d", (*thread)->sense);
        *countOfThreads = numberOfThreads;
        *globalSense = (*thread)->sense;
    }
  }

  while((*thread)->sense != *globalSense)
  {
    i++;
      //printf("\nThread %d is still spinning on thread Sense %d", (*thread)->sense, (*thread)->sense);
  }
      ;

} 


void centralizedOMP(int numberOfThreads, int numberOfBarriers)
{
  if(numberOfThreads <=0 || numberOfBarriers <= 0)
  {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
  }

  int i = 0;

  for(i = 0; i < 10; i++)
  {
       barrierTimeCounter[i] = 0.0;
  } 

  omp_set_num_threads(numberOfThreads);

  globalThreadSense = 1;

  //Thread Logic begins here
  #pragma omp parallel shared(globalThreadSense, numberOfThreads)
  {

     struct timeval startTime, endTime;
     double totalTime;	
      //Update numberOfThreads to the correct value
      numberOfThreads = omp_get_num_threads(); 
      threadCounter = numberOfThreads;

      Thread* thread;
      thread = (Thread*)malloc(sizeof(Thread));
      thread->sense = 1;

      int i = 0, j =0, threadID;

      threadID = omp_get_thread_num();

      for(i = 0; i <numberOfBarriers; i++)
      {
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }
        
          printf("\nEntered thread %d  of %d threads at barrier %d", threadID, numberOfThreads, i);

	  gettimeofday(&startTime, NULL);		
          centralizedThreadBarrierLogic(&thread, &threadCounter, &globalThreadSense);
	  gettimeofday(&endTime, NULL);

	  totalTime = (endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec); 	
	
	  printf("\nTotal time at barrier %d by thread %d is %f", i, threadID, totalTime);	
	  #pragma omp critical 
	  {
		barrierTimeCounter[i] += (float)totalTime;
	  }
		
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }

          printf("\nCompleted thread %d of %d threads at barrier %d", threadID, numberOfThreads, i, thread->sense, globalThreadSense);

      }   
  
  }

  float totalBarrierTime = 0.0;
  for(i = 0; i < numberOfBarriers; i++)
  {
 	printf("\nTime taken at Barrier %d is %f", i, barrierTimeCounter[i]);
        totalBarrierTime += barrierTimeCounter[i];
  }

  printf("\nAverage time taken at Barriers is %f\n", (float)totalBarrierTime/numberOfBarriers); 	
}


/**
 *@brief Main Function
 *@param argc - Argument Count  
 *@param argv - Argument Values
 *
int main(int argc, char** argv)
{
  printf("\nEnter the number of threads:");
  scanf("%d", &numberOfThreads);

  printf("\nEnter the number of barriers:");
  scanf("%d", &numberOfBarriers);

  centralizedOMP(numberOfThreads, numberOfBarriers);
  return 0;
}*/
