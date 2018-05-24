char class;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "smpi/mpi.h"
#include "simgrid/instr.h" //TRACE_

#define TRUE 1
#define FALSE 0
#define N 10

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int buf;
  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  for (int i = 0; i < N; i++)
  {
    printf("[%d]: Before Bcast, buf is %d\n", world_rank, buf);
    MPI_Bcast(&buf, 1, MPI_INT, i % world_size, MPI_COMM_WORLD);
    printf("[%d]: After Bcast, buf is %d\n", world_rank, buf);
  }

  MPI_Finalize();
  return 0;
}
