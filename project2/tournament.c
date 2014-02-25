#include<stdio.h>
#include"mpi.h"
#include <math.h>
#include<stdlib.h>
#include<omp.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#define WINNER 0
#define LOSER 1
#define BYE 2
#define CHAMPION 3
#define DROPOUT 4
#define USELESS -1

int PID;
int numOfProcessors;
int numOfBarriers = 2;
struct timeval start_t,finish_t;
unsigned long start,finish,final_usec,final_sec;

typedef struct processor_struct
{
    int role; //processor role
    int sense; //local sense
    int Pid;//Processor Id
    int opponent;//Opponent Processor Id
    int round;
	
}Processor;

void getElapsedTime(struct timeval *start_t, struct timeval *finish_t,int j){
	start=(start_t->tv_sec*1000000 + start_t->tv_usec);
	finish=(finish_t->tv_sec*1000000 + finish_t->tv_usec);
	final_sec =(finish-start)/1000000;
	final_usec = (finish-start)%1000000;
	printf("Process %d took %ld:%ld seconds to complete barrier %d\n",PID,final_sec,final_usec,j);
}

void tournamentBarrier(Processor processorList[numOfProcessors][20], int numOfRounds, int PID){
	//printf("reaching tournament\n");
	int roundNum=1,message = 1,arrival = 1;
	int i;

	//arrival
	while(arrival == 1){
	//	printf("arrival is = %d\n",arrival);
		switch(processorList[PID][roundNum].role){
				case WINNER:
					MPI_Recv(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD, NULL);
				case LOSER:
					MPI_Send(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD);
					MPI_Recv(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD, NULL);
					arrival=0;
					break;
				case CHAMPION:
					MPI_Recv(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD, NULL);
					MPI_Send(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD);
					fflush(stdout);
					arrival=0;
					break;
				case BYE: break;
				case DROPOUT: break;
		}
		if(arrival ==1 && roundNum <= numOfRounds)
			roundNum = roundNum +1;
	}

	//wakeup
	while(arrival == 0) {
		if( roundNum > 0 )
			roundNum = roundNum - 1;
		switch(processorList[PID][roundNum].role){
				case WINNER:
					MPI_Send(&message, 1, MPI_INT, processorList[PID][roundNum].opponent, 1, MPI_COMM_WORLD);
					fflush(stdout);
					break;
			        case LOSER:break;
				case CHAMPION: break;
				case BYE: break;
				case DROPOUT: arrival = 1;
		}
		

	}


}


int main( int argc, char **argv ) {

	
	double startTime, endTime,totalTime = 0.00;
	int PID;

	MPI_Init(&argc, &argv);
	printf("init success\n");
	
	MPI_Comm_size(MPI_COMM_WORLD,&numOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &PID);
	int numOfRounds = ceil( log(numOfProcessors)/log(2) );
	printf("number of processors is %d\n",numOfProcessors);
	printf("id = %d\n",PID);
	printf("number of rounds = %d\n",numOfRounds);	

	Processor processorList[numOfProcessors][20];
	int i, k, l,n;

	for( k=0; k<=numOfRounds; k++ ) {
		processorList[PID][k].sense = 0;
		processorList[PID][k].role = -1;
		processorList[PID][k].opponent = -1;
	}

/*	for(n=0;n<=numOfRounds;n++)
        printf("%d\t",processorList[PID][n].role);
	printf("succesful initialization of list\n");

*/
	for( k=0; k<=numOfRounds; k++) {
		if( k==0 ){
			processorList[PID][k].role = DROPOUT;
		}
		if((k > 0) && (PID % (int)pow(2,k) == 0) && ((PID + ((int)pow(2,k-1)))< numOfProcessors) && ((int)pow(2,k) < numOfProcessors)){
			processorList[PID][k].role = WINNER;
		}

		if((k > 0) && (PID%(int)pow(2,k) == 0) && ((PID + (int)pow(2,k-1))) >= numOfProcessors){
			processorList[PID][k].role = BYE;
		}

		if((k > 0) && ((PID%(int)pow(2,k) == (int)pow(2,k-1)))){
			processorList[PID][k].role = LOSER;
		}

		if((k > 0) && (PID==0) && ((int)pow(2,k) >= numOfProcessors)){
			processorList[PID][k].role = CHAMPION;
		}
		if( processorList[PID][k].role == LOSER ){
			//marking winners at each round
			processorList[PID][k].opponent = PID - (int)pow(2,k-1);
		}

		if(processorList[PID][k].role == WINNER || processorList[PID][k].role == CHAMPION){
			//marking losers at each round
			processorList[PID][k].opponent = PID + (int)pow(2,k-1);
		}

	}

/*	for(n=0;n<=numOfRounds;n++)
	printf("%d\t",processorList[PID][n].role);
	printf("\nsuccesful allotment of roles\n");
*/
       int j;
       for( j=0; j<numOfBarriers; j++){
		printf("Entered processor %d entered at barrier %d\n", PID,j);
    		startTime = MPI_Wtime();
		gettimeofday(&start_t,NULL);
    		tournamentBarrier(processorList,numOfRounds,PID);
    		endTime = MPI_Wtime();
		gettimeofday(&finish_t,NULL);
		getElapsedTime(&start_t,&finish_t,j);
        	totalTime = totalTime + endTime-startTime;
        	printf("Completed processor %d of %d processors at barrier %d\n", PID, numOfProcessors,j);
	}	
	printf("time taken by one processor %d is %f\n",PID,totalTime/numOfBarriers);
	MPI_Finalize();
	return 0;
}


