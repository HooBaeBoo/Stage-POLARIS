char class;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "smpi/mpi.h"
#include "simgrid/instr.h" //TRACE_

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE
#define PARSIM(min, max, D, i)               \
  if (i < min)                               \
  {                                          \
    i = min;                                 \
    if (DEBUG)                               \
      printf("[%d] Sleeping\n", world_rank); \
    MPI_sleep(D[world_rank]);                \
  }                                          \
  if (i >= max)                              \
  {                                          \
    S[world_rank] = MPI_Wtime(); \
    break;                                   \
  }
// Sleep function
void MPI_sleep(double atime)
{
  double starttime = MPI_Wtime();
  while ((MPI_Wtime() - starttime) < atime )
  {
    sleep(1);
  };
  return;
}

int main(int argc, char **argv)
{

  /* SETTINGS :
  MIN : inferior border for iterations
  MAX : superior border for iterations
  */

  int min = atoi(argv[3]);
  int max = atoi(argv[4]);

  // Iterations number
  int N = atoi(argv[5]);

  MPI_Init(&argc, &argv);
  double starttime = MPI_Wtime();
  int buf;

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  double S[world_size];
  int D[world_size];

  // Delay
  if (argc != 4 + world_size)
  {
    if (DEBUG)
      printf("ERROR : incorrect number of args\n");
    exit(-1);
  }

  for (int i = 0; i < world_size; i++)
  {
    D[i] = atoi(argv[4 + i]);
  }

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  buf = world_rank;

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  if (world_rank == 0 && DEBUG)
  {
    printf("Launching broadcast test\n");
    printf("Parameters : min [%d], max[%d], N [%d]\n", min, max, N);
    printf("MPI configuration done\n");
  }

  srand(time(NULL));

  for (int i = 0; i <= N; i++)
  {
    PARSIM(min, max, D, i);
    if (DEBUG)
      printf("[%d](i:%d): Before Bcast, buf is %d\n", world_rank, i, buf);
    // MPI_Bcast(&buf, 1, MPI_INT, i % world_size, MPI_COMM_WORLD);
    MPI_Bcast(&buf, 1, MPI_INT, world_rank, MPI_COMM_WORLD);

    // Random calculations
    buf = buf + world_rank;
    sleep(rand() % 2);
    if (DEBUG)
      printf("[%d](i:%d): After Bcast, buf is %d\n", world_rank, i, buf);
  }
  printf("From %d -> S[%d] : %1.2f       %1.2f\n", world_rank, world_rank, S[world_rank], starttime);

  MPI_Finalize();
  return 0;
}
