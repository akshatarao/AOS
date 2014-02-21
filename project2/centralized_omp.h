#include<omp.h>
#include<stdio.h>
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

void centralizedBarrierLogic(Thread **thread, int* countOfThreads, int* globalSense);
void centralized_omp(int numberOfThreads, int numberOfBarriers);


