#include<stdio.h>
#include<centralized_mpi.h>
#include<centralized_omp.h>

int main(int argc, char* argv[])
{
   int numberOfThreads, numberOfProcesses, numberOfBarriers;
   int choice;

   printf("\n*****************");
   printf("\n1. Centralized OMP");
   printf("\n2. Centralized MPI");
   printf("\n3. Dissemination OMP");
   printf("\n4. Tournament MPI");
   printf("\n5. Dissemination OMP - Centralized MPI");
   printf("\n6. Dissemination OMP - Tournament MPI");
   printf("\nEnter choice:");

   scanf("%d", &choice);

   switch(choice)
   {
      case 1:
	printf("\nEnter the number of threads:");
	scanf("%d", &numberOfThreads);
	
	printf("\nEnter the number of barriers:");
	scanf("%d", &numberOfBarriers);
	
	centralizedOMP(numberOfThreads, numberOfBarriers);
	break;
	
      case 2:
	printf("\nEnter the number of barriers:");
	scanf("%d", &numberOfBarriers);		
	break;
	
      default:
	printf("\nInvalid Option!");	 
   }
    
}
