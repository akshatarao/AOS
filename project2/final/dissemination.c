#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>


int numberOfThreads;
int numberOfBarriers;

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
        
		#pragma omp critical
		{		
		neighborIndex = fmod((threadId + pow(2,t)),numberOfThreads);
		printf("Thread %d has neighbor index %d in round %d\n",threadId, neighborIndex, t);
		Thread* neighborThread = *(threadList + neighborIndex);
		thread->sentCount = thread->sentCount + 1;
		neighborThread->receivingCount = neighborThread->receivingCount + 1;
		printf("%d thread's receiving count is %d\n",thread->threadID,thread->receivingCount);
		printf("%d Neighbor thread's receiving count is %d\n",neighborThread->threadID,neighborThread->receivingCount);
		printf("%d thread's sent count is %d\n",thread->threadID,thread->sentCount);
		}
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

int main(int argc, char** argv)
{

    struct timeval startTime,endTime;

    printf("\nEnter the number of threads: ");
    scanf("%d", &numberOfThreads);

    printf("Enter the number of barriers: ");
    scanf("%d", &numberOfBarriers);

    if(numberOfThreads <=0 || numberOfBarriers <= 0) {
      printf("\nERROR: Number of threads/barriers cannot be negative!");
      exit(1);
    }

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
	   	printf("barrierCheck is %d\n", barrierArray[i]);  
	   }	           

	   while(barrierArray[i]!= numberOfThreads);
	   
           gettimeofday(&endTime, NULL);
	}
        printf("Time spent in barrier by thread %d is %f\n",thread->threadID, (double)(endTime.tv_sec * 1000000 + endTime.tv_usec)- (startTime.tv_sec * 1000000 + startTime.tv_usec));
    }
return 0;
}
