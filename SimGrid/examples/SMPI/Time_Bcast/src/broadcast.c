#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "smpi/mpi.h"
#include "simgrid/instr.h" //TRACE_

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE
#define BUF_SIZE 10000
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
    S[world_rank] = MPI_Wtime();             \
    break; \
  }                                   \
  // Sleep function
void MPI_sleep(double atime)
{
  FILE *p;
  p = fopen("mpi_sleep","ab+");
  fprintf(p,"%g\n",atime);
  fclose(p);
  SMPI_SAMPLE_DELAY(atime)
  /*
  double starttime = MPI_Wtime();
  while ((MPI_Wtime() - starttime) < atime)
  {
    sleep(1);
  };
  */
  return;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen("scores","ab+");
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
  int buf[BUF_SIZE];

  for (int i = 0; i < BUF_SIZE ; i++){
    buf[i] = 96;
  }

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  double S[world_size];
  double D[world_size];

  // Delay
  if (argc != 4 + world_size)
  {
    if (DEBUG)
      printf("ERROR : incorrect number of args\n");
    exit(-1);
  }

  for (int i = 0; i < world_size; i++)
  {
    D[i] = atof(argv[4 + i]);
    fprintf(fp,"%g\n",D[i]);
   }
  fclose(fp);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
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

  for (int i = 0; i <= N; i++)
  {
    PARSIM(min, max, D, i);
    if (DEBUG)
      printf("[%d](i:%d): Before Bcast, buf is %d\n", world_rank, i, buf);

    MPI_Bcast(&buf, BUF_SIZE, MPI_INT, i % world_size, MPI_COMM_WORLD);

    // Calculs

    /*for (int i = 0; i < world_rank * 100; i++)

    {
      buf[i] = buf[i] + world_rank;
    }*/
    if (DEBUG)
      printf("[%d](i:%d): After Bcast, buf is %d\n", world_rank, i, buf[0]);
  }
  printf("From %d -> %g       \n", world_rank, S[world_rank]);

  MPI_Finalize();
  return 0;
}
