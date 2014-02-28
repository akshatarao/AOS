/**
 *@author Akshata Rao
**/

/**
 *@file baseline_omp.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

#define MAX_NUMBER_OF_BARRIERS 100000

float barrierTimeCounter[MAX_NUMBER_OF_BARRIERS];

/**
 *@brief Baseline OMP barrier
 *@param numberOfThreads - Number of Threads forked
 *@param numberOfBarrier - Number of Barriers
 *@returns none
 */
void baselineOMP(int numberOfThreads, int numberOfBarriers)
{
  //Validate arguments
  if(numberOfThreads <=0 || numberOfBarriers <= 0)
  {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
  }

  int i = 0;

  //Initialize Barrier Counter
  for(i = 0; i < MAX_NUMBER_OF_BARRIERS; i++)
  {
       barrierTimeCounter[i] = 0.0;
  }

  omp_set_num_threads(numberOfThreads);


  //Thread Logic begins here
  #pragma omp parallel shared(numberOfThreads)
  {

     struct timeval startTime, endTime;
     double totalTime;
      //Update numberOfThreads to the correct value
	      numberOfThreads = omp_get_num_threads();

      int i = 0, j =0, threadID;

      threadID = omp_get_thread_num();

      //Iterate through barriers
      for(i = 0; i <numberOfBarriers; i++)
      {
          //Busy Wait
          j = 0;
          while(j < 9999)
          {
              j++;
          }

          printf("\nEntered thread %d  of %d threads at barrier %d", threadID, numberOfThreads, i);
	
	  //Get start time for barrier
          gettimeofday(&startTime, NULL);
          #pragma omp barrier
	  {
          } 
	  //Get end time for barrier	
          gettimeofday(&endTime, NULL);

	  //Calculate total time for barrier
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

          printf("\nCompleted thread %d of %d threads at barrier %d", threadID, numberOfThreads, i);

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
 */
int main(int argc, char** argv)
{
  int numberOfThreads, numberOfBarriers;

  if(argc < 3)
  {
	printf("\nSyntax: %s numberOfThreads numberOfBarriers\n", argv[0]);
	exit(1);
  }

  numberOfThreads = atoi(argv[1]);
  numberOfBarriers = atoi(argv[2]);

  if(numberOfBarriers > MAX_NUMBER_OF_BARRIERS)
  {
	printf("\nNumber of Barriers exceeded");
	exit(1);
  }

  //Execute baselineOMP  
  baselineOMP(numberOfThreads, numberOfBarriers);
 
  return 0;
}

