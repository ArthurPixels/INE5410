#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

double *a, *b;
int tam_vet, num_threads;

void* initialize (void* arg) {
  	int start = *((int *)arg);
	free(arg);
  	for(int i = start*(tam_vet/num_threads);
			i<(start+1)*(tam_vet/num_threads) && i<tam_vet; i++) {
    	a[i] = i;
		b[i] = i;
  	}

  	pthread_exit(NULL);
}

void* dotProduct (void* arg) {
  	int start = *((int *)arg);
	free(arg);
	double* sum = malloc(sizeof(double));
	*sum = 0;
  	for(int i = start*(tam_vet/num_threads);
			i<(start+1)*(tam_vet/num_threads) && i<tam_vet; i++) {
    	*sum += a[i] * b[i];
  	}
  	pthread_exit((void *)sum);
}

int main (int argc, char *argv[])
{
	double prod = 0.0;
	num_threads = 4;
	pthread_t threads[4];

    if(argc<2){
      printf("uso %s <tamanho vetores>\n", argv[0]);
      exit(1);
    }

	/* tamanho dos vetores */
    tam_vet = atoi(argv[1]);

	/* alocacao do vetor A */
    a = (double *) malloc(sizeof(double) * tam_vet);

    /* alocacao do vetor B */
    b = (double *) malloc(sizeof(double) * tam_vet);

	printf("Inicializando vetores A, e B...\n");

	/* inicializacao dos vetores */
	for(int i = 0; i<num_threads; i++) {
		int* arg = malloc(sizeof(int));
		*arg = i;
    	pthread_create(&threads[i], NULL, initialize, (void *)arg);
		printf("Thread %d criada.\n", (int)threads[i]);
  	}
	for(int i = 0; i<num_threads; i++) {
    	pthread_join(threads[i], NULL);
    	printf("Thread %d encerrada.\n", (int)threads[i]);
  	}
	printf("Tam_vet: %d\n", tam_vet);
	printf("Calculando...\n");

	/* produto escalar dos vetores */
	for(int i = 0; i<num_threads; i++) {
		int* arg = malloc(sizeof(int));
		*arg = i;
    	pthread_create(&threads[i], NULL, dotProduct, (void *)arg);
		printf("Thread %d criada.\n", (int)threads[i]);
  	}
	for(int i = 0; i<num_threads; i++) {
    	double* temp = NULL;
		pthread_join(threads[i], (void **)&temp);
		prod += *temp;
		free(temp);
    	printf("Thread %d encerrada.\n", (int)threads[i]);
  	}

	printf("Terminou!\n");

    /*** imprime o resultado ***/
    printf("******************************************************\n");
    printf("Produto escalar: %.2f\n", prod);
    printf("******************************************************\n");

    free(a);
    free(b);
}
