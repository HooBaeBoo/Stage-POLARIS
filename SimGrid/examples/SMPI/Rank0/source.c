#include <mpi.h>

int main(int argc, char**argv) {

	int x;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &x);
	
	if (x == 0) { // rank
		x = 42;
		MPI_Send(&x, 1, MPI_INT, 1, 1,MPI_COMM_WORLD);
	} else {
		MPI_Recv(&x, 1, MPI_INT, 0, 1,MPI_COMM_WORLD, NULL);
		printf("Got %d from rank 0",x);
	}
	MPI_Finalize();
}
