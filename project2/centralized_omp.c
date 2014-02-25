/**
 *@author Akshata Rao
 */

/**
 *@file centralized_omp.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<centralized_omp.h>

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
//      printf("\nThread %d is still spinning on thread Sense %d", (*thread)->sense, (*thread)->sense);
  }

} 


/**
 *@brief Centralized OMP Function
 *@param numberOfThreads - Number of Threads  
 *@param numberOfBarriers - Number of Barriers
 *@returns none
 */
void centralizedOMP(int numberOfThreads, int numberOfBarriers)
{
  double totalTime = 0.0;

  if(numberOfThreads <=0 || numberOfBarriers <= 0)
  {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
  }

  omp_set_num_threads(numberOfThreads);

  globalSense = 1;

  //Thread Logic begins here
  #pragma omp parallel shared(globalSense, numberOfThreads)
  {
      double startTime, endTime, barrierTime;
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

          startTime = omp_get_wtime();
          centralizedBarrierLogic(&thread, &threadCounter, &globalSense);
          endTime = omp_get_wtime();
          barrierTime = endTime - startTime;
          printf("\nSingle barrier time %f for thread %d", barrierTime, threadID);

          totalTime += barrierTime; 
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }

          printf("\nCompleted thread %d of %d threads at barrier %d %d", threadID, numberOfThreads, i, thread->sense, globalSense);

      }   
  
  }

  double averageBarrierTime = (double)totalTime/numberOfThreads;
  averageBarrierTime = (double)averageBarrierTime/numberOfBarriers;

  printf("\nTotal barrier time: %f", averageBarrierTime);
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
