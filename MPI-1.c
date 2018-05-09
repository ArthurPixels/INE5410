#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
  int size, rank, buffer[1];
  MPI_Status st;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0) buffer[0] = 10;
  MPI_Bcast(&buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0) { // rank=0 enviaparatodososoutros processos
    for (int i=1; i< size; i++) {
      MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
      printf("%d ", buffer[0]);
    }
  }
  else {
    buffer[0] += rank;
    MPI_Send(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
