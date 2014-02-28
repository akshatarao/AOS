/**
 *@author Spoorthi Ravi
 */

/**
 *@file dissemination_omp.c
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>


int numberOfThreads = 8;
int numberOfBarriers = 100000;

/**
*Thread Structure
*/
typedef struct thread_struct
{
    int receivingCount; //local counter
    int sentCount;
    int threadID;
}Thread;

/**
 *@brief getting elapsed time between entry and exit of thread at barrier
 *@param start_t - entry time of thread 
 *@param finish_t - exit time of thread                         
 *@returns elapsed time for the barrier
 */
double getElapsedTime(struct timeval *start_t, struct timeval *finish_t){
        unsigned long start,finish,final_usec,final_sec;
        start=(start_t->tv_sec*1000000 + start_t->tv_usec);
        finish=(finish_t->tv_sec*1000000 + finish_t->tv_usec);
        final_sec =(finish-start)/1000000;
        final_usec = (finish-start)%1000000;
        return (finish-start);
        }

/**
 *@brief dissemination barrier logic
 *@param thread - thread structure
 *@param threadList - list of threads
 *@param threadId - ID of thread                                
 *@returns none
 **/
void disseminationBarrier(Thread* thread,Thread** threadList,int threadId)
{
    int r,s = 0,t,neighborIndex;
    int numOfRounds = ceil(log(numberOfThreads)/log(2));

    //Busy Wait
    while(s < 9999)
        s++;

    for(t=0;t<numOfRounds;t++) {

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
 *@brief Main Function
 *
 */
int main(int argc, char** argv)
{

   if(argc < 3)
   {
	printf("\nSyntax: %s numberOfThreads numberOfBarriers", argv[0]);
	fflush(stdout);
	exit(1);
   }
	 
    numberOfThreads = atoi(argv[1]);
    numberOfBarriers = atoi(argv[2]);
     	
    if(numberOfThreads <=0 || numberOfBarriers <= 0) {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
    }

    int k,l;

    int barrierArray[numberOfBarriers];
    
    //Set number of threads
    omp_set_num_threads(numberOfThreads);
    

    int rounds = ceil(log(numberOfThreads)/log(2));
    Thread** threadList = malloc(numberOfThreads*sizeof(Thread*));

    //Initialize Threads
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

        int i = 0,j;
        struct timeval startTime,endTime;
        double totalTime = 0.00;
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

           gettimeofday(&startTime, NULL);
           disseminationBarrier(thread,threadList,threadId);
           printf("Completed thread %d of %d threads at barrier %d\n", thread->threadID, numberOfThreads-1, i);

           #pragma omp critical
           {
                barrierArray[i] = barrierArray[i] + 1;
                //printf("barrierCheck is %d\n", barrierArray[i]);
           }

           while(barrierArray[i]!= numberOfThreads);

           gettimeofday(&endTime, NULL);
           totalTime = totalTime + getElapsedTime(&startTime,&endTime);
           }
           printf("Time spent in barrier by thread %d is %f\n",thread->threadID,(totalTime/numberOfBarriers));


    }
return 0;
}

