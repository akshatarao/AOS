/**
 *@author Akshata Rao
 */

/**
 *@file centralized_omp.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int numberOfThreads;
int threadCounter;
int numberOfBarriers;
int barrierCounter;
int globalSense = 0;

typedef struct thread_struct
{
    int count; //local counter
    int sense; //local sense
    int localBarrierCounter; //localBarrierCounter
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

  int i = 0;
  while((*thread)->sense != *globalSense)
  {
    i++;
      //printf("\nThread %d is still spinning on thread Sense %d", (*thread)->sense, (*thread)->sense);
  }
      ;

} 


/**
 *@brief Main Function
 *@param argc - Argument Count  
 *@param argv - Argument Values
 */
int main(int argc, char** argv)
{
  printf("\nEnter the number of threads:");
  scanf("%d", &numberOfThreads);

  printf("\nEnter the number of barriers:");
  scanf("%d", &numberOfBarriers);

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

          centralizedBarrierLogic(&thread, &threadCounter, &globalSense);

          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }

          printf("\nCompleted thread %d of %d threads at barrier %d", threadID, numberOfThreads, i, thread->sense, globalSense);

      }   
  
  }

  return 0;
}
