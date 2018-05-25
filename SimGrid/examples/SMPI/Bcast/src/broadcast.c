char class;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "smpi/mpi.h"
#include "simgrid/instr.h" //TRACE_

#define TRUE 1
#define FALSE 0
#define N 2
#define PARSIM(min,max,D,i) if(i<=min){i=min; MPI_sleep(D[i]); } if (i>=max) {break;}

void MPI_sleep(int time)
{
  printf("Sleeping");
  int starttime = MPI_Wtime();
  while ((starttime + time) - MPI_Wtime() > 0)
  {
  };
  return 0;
}

int main(int argc, char **argv)
{
  int min = atoi(argv[3]);
  int max = atoi(argv[4]);
  int D[N] = {0, 1};
  MPI_Init(&argc, &argv);
  int buf;
  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (world_rank == 0)
  {
    buf = 7;
  }
  else
  {
    buf = world_rank;
  }
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  for (int i = 0; i < N; i++)
  {
    PARSIM(min, max, D, i);
    printf("[%d]: Before Bcast, buf is %d\n", world_rank, buf);
    MPI_Bcast(&buf, 1, MPI_INT, i % world_size, MPI_COMM_WORLD);
    printf("[%d]: After Bcast, buf is %d\n", world_rank, buf);
  }

  MPI_Finalize();
  return 0;
}
