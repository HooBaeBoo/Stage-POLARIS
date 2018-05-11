// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
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
	
	if (world_rank == 0) {
	 MPI_Send(&world_rank, 1, MPI_INT, 1, 1,MPI_COMM_WORLD);	
  }
	else {
	 
	  MPI_Recv(&buf,1,MPI_INT,world_rank-1,world_rank,MPI_COMM_WORLD,NULL);
	 if (world_rank != world_size-1) {
    MPI_Send(&world_rank, 1, MPI_INT, world_rank+1, world_rank+1,MPI_COMM_WORLD);
    }
    printf("Hi! My name's %s, rank %d. Reporting : message from %d rcvd.\n",
         processor_name, world_rank, buf, world_rank, world_size-1);
  }


  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
