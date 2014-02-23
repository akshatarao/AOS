/**
* @author Akshata Rao
*/

/**
* @file centralized_omp.h
*/

#ifndef CENTRALIZED_OMP_H
#define CENTRALIZED_OMP_H
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

int numberOfThreads;
int threadCounter;
int numberOfBarriers;
int barrierCounter;
int globalSense = 0;
float barrierTimeCounter[10];

typedef struct thread_struct
{
    int count; //local counter
    int sense; //local sense
    int localBarrierCounter; //localBarrierCounter
}Thread;

void centralizedBarrierLogic(Thread **thread, int* countOfThreads, int* globalSense);
void centralized_omp(int numberOfThreads, int numberOfBarriers);

#endif
