#ifndef TOURNAMENT_H
#define TOURNAMENT_H

/**
 * @author Akshata Rao
 */

/**
 *@file tournament.h
 */

#include<stdio.h>
#include<stdbool.h>

#define MAX_PROCESSORS 64

typedef enum role
{
    WINNER,
    LOSER
}Role;

typedef struct processor
{
    int processorID;
    int roundNumber;
    int sense;
    Role role; 
}Processor;

Processor processors[MAX_PROCESSORS];


#endif


