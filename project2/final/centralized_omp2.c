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
  
  i = 0;
  
  while((*thread)->sense != *globalSense)
  {
	i++;
  }

} 


void centralizedOMP(int numberOfThreads, int numberOfBarriers)
{
  if(numberOfThreads <=0 || numberOfBarriers <= 0)
  {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
  }

  int i = 0;

  for(i = 0; i < numberOfBarriers; i++)
  {
       barrierTimeCounter[i] = 0.0;
  } 

  omp_set_num_threads(numberOfThreads);

  globalThreadSense = 1;

  for(i = 0; i < numberOfBarriers; i++)
  {

  	 threadCounter = numberOfThreads;
 
	   //Thread Logic begins here
	   #pragma omp parallel shared(globalThreadSense, numberOfThreads)
   	   {
	
     struct timeval startTime, endTime;
     double totalTime;	
      //Update numberOfThreads to the correct value
      numberOfThreads = omp_get_num_threads(); 

      Thread* thread;
      thread = (Thread*)malloc(sizeof(Thread));
      thread->sense = 1;

      int i = 0, j =0, threadID;

      threadID = omp_get_thread_num();

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

          printf("\nCompleted thread %d of %d threads at barrier %d %d", threadID, numberOfThreads, i, thread->sense, globalThreadSense);  
 	 }
  }

  float totalBarrierTime = 0.0;
  for(i = 0; i < numberOfBarriers; i++)
  {
       /* fp = fopen("omp.log", "a+");
 	fprintf(fp, "\nTime taken at Barrier %d is %f", i, barrierTimeCounter[i]);
        fclose(fp);
  */
        totalBarrierTime += barrierTimeCounter[i];
  }

  printf("\nAverage time taken at Barriers is %f\n", (float)totalBarrierTime/numberOfBarriers); 	

}

/**
 *@brief - Main Function
 *@param - argv[1] - Number of Threads
 *@param - argv[2] - Number of Barriers
 *@returns none
 */
int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("\nSyntax: <executable> numberOfThreads numberOfBarriers");
        exit(1);
    }

     	
    int numberOfThreads = atoi(argv[1]);
    int numberOfBarriers = atoi(argv[2]);

    if(numberOfThreads <= 0 || numberOfBarriers <= 0)
    {
        printf("\nInvalid Inputs!");
        exit(1);
    }

    centralizedOMP(numberOfThreads,numberOfBarriers);
}
