#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

unsigned int compute_pi(unsigned int, unsigned int);

int main(int argc, char **argv){
  int size, rank, buffer[1];
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  unsigned int pontos;
  unsigned int pontos_no_circulo;
  unsigned int i;

  if(rank == 0 && argc != 2){
    printf("Uso:\n");
    printf("\t%s <numero de pontos a serem sorteados>\n", argv[0]);
    return 1;
  }

  pontos = atoi(argv[1]);

  // retorna quantos pontos sorteados cairam dentro do circulo
  // aqui estamos considerando uma semente para o gerador de
  // numeros aleatorios fixa = 0
  pontos_no_circulo = compute_pi(rank, pontos);

  if (rank == 0) {
    // calcula a aproximacao de Pi baseado nos pontos sorteados
    printf("Pi = %.040f\n", ((double)pontos_no_circulo/(double)pontos)*4);
  }

  MPI_Finalize();
  return 0;
}

unsigned int compute_pi(unsigned int seed, unsigned int pontos){
  MPI_Status st;
  int rank = (int)seed;
  unsigned int i;
  unsigned int pontos_no_circulo;
  double x, y;

  pontos_no_circulo = 0;
  srand(seed);

  for(i=0; i<pontos; i++){
    if(rank == pontos%i){
    	// sorteia um ponto: coordenadas x e y dentro do quadrado
    	// consideramos que R = 1, então x e y pertencem ao intervalo [0; 1]
      x = (double)rand()/(double)(RAND_MAX);
      y = (double)rand()/(double)(RAND_MAX);

      // verifica se o ponto sorteado encontra-se dentro do circulo
      // um ponto (x, y) esta dentro do circulo se: x^2 + y^2 < R^2
      // nesse caso, consideramos R = 1
      if( (x*x + y*y) < 1 ){
        pontos_no_circulo++;
      }
      if (rank != 0) {
        MPI_Send(&pontos_no_circulo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }

    if (rank == 0) {
      for (int i=1; i< pontos; i++) {
        unsigned int pontos_temp = 0;
        MPI_Recv(&pontos_temp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        pontos_no_circulo += pontos_temp;
      }
      pontos_no_circulo;
    }
  }
  return pontos_no_circulo;
}
