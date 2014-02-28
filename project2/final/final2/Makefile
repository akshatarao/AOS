CC = gcc
CFLAGS = -I .
MPICC = mpicc
MATHFLAGS = -lm
OMPFLAGS = -fopenmp

all: centompmake centmpimake dissompmake tourmpimake mergedmake baselinempimake baselineompmake

centompmake: 
	$(CC) -fopenmp centralized_omp.c -o centralized_omp $(CFLAGS)

centmpimake: 
	$(MPICC) centralized_mpi.c -o centralized_mpi $(CFLAGS)

dissompmake: 
	$(CC) $(OMPFLAGS) dissemination_omp.c -o dissemination_omp $(CFLAGS) $(MATHFLAGS)

tourmpimake: 
	$(MPICC) tournament_mpi.c -o tournament_mpi $(CFLAGS) $(MATHFLAGS)

mergedmake: 
	$(MPICC) $(OMPFLAGS) centDiss.c -o cent_diss $(CFLAGS) $(MATHFLAGS)

baselinempimake: 
	$(MPICC) baselineMPI.c -o baselineMPI $(CFLAGS)

baselineompmake:
	$(CC) $(OMPFLAGS) baselineOMP.c -o baselineOMP $(CFLAGS)

clean:
	rm -f baselineMPI baselineOMP cent_diss tournament_mpi dissemination_omp centralized_omp centralized_mpi
