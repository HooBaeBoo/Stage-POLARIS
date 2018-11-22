#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
/* #include "smpi/mpi.h" */
/* #include "simgrid/instr.h" //TRACE_ */

int mysleep(double duration);
int mysleep(double duration) {
  struct timespec ts;
  ts.tv_sec=(long) (floor(duration));
  ts.tv_nsec=(long) (floor((duration-floor(duration))*1E9));
  nanosleep(&ts,NULL);
  return 0;
}
#define TRUE 1
#define FALSE 0
#define DEBUG FALSE
#define BUF_SIZE 10000
#define PARSIM(min, max, D, i)               \
  if (i < min)                               \
  {                                          \
    i = min;                                 \
    if (DEBUG)                               \
      printf("[%d:%g] Sleeping for %g\n", world_rank, MPI_Wtime(),D[world_rank]);	\
    mysleep(D[world_rank]);                    \
    if (DEBUG)                               \
      printf("[%d:%g] Waking up\n", world_rank, MPI_Wtime());	\
  }                                          \
  if (i >= max)                              \
  {                                          \
    S[world_rank] = MPI_Wtime();             \
    break; \
  }                                   \

int main(int argc, char **argv)
{
  /* SETTINGS :
  MIN : inferior border for iterations
  MAX : superior border for iterations
  */

  MPI_Init(&argc, &argv);

  if(DEBUG)
    for (int i = 0; i < argc; i++)
      printf("argc[%d] = %s\n",i,  argv[i]);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  double *S = calloc(world_size, sizeof(double));
  double *D = calloc(world_size, sizeof(double));
  
  int min = atoi(argv[1]);
  int max = atoi(argv[2]);

 
  // Iterations number
  int N = atoi(argv[3]);

  // Delay
  if (argc != 4 + world_size)
  {
    if (DEBUG)
      printf("ERROR : incorrect number of args\n");
    exit(-1);
  }

  if (DEBUG) {
    printf("Min = %d\n", min);
    printf("Max = %d\n", max);
    printf("N = %d\n", N);
  }

  for (int i = 0; i < world_size; i++)
  {
    D[i] = atof(argv[4 + i]);
    if (DEBUG) printf("D[%d] = %g\n",i,D[i]);
   }


 
  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  int buf[BUF_SIZE];
  for (int i = 0; i < BUF_SIZE ; i++){
    buf[i] = world_rank;
  }

  if (world_rank == 0 && DEBUG)
  {
    printf("Launching broadcast test\n");
    printf("Parameters : min [%d], max[%d], N [%d]\n", min, max, N);
    printf("MPI configuration done\n");
  }

  for (int i = 0; i <= N; i++)
  {
    PARSIM(min, max, D, i);
    if (DEBUG) {
      printf("************** %d [P%d: %g] ***************\n", i, world_rank, MPI_Wtime());
      /* printf("[%d](i:%d): Before Bcast, buf is %d\n", world_rank, i, buf[i]); */
    }
    MPI_Bcast(&buf, BUF_SIZE, MPI_INT, i % world_size, MPI_COMM_WORLD);

    // Calculs

    /*for (int i = 0; i < world_rank * 100; i++)

    {
      buf[i] = buf[i] + world_rank;
    }*/
    if (DEBUG)
      printf("[%d](i:%d): After Bcast, buf is %d\n", world_rank, i, buf[i]);
  }
  printf("From %d -> %g       \n", world_rank, S[world_rank]);

  MPI_Finalize();
  return 0;
}
