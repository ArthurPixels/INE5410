#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

double *a, *b, *c;
int tam_vet, num_threads;


void* initialize (void* arg) {
  	int start = *((int *)arg);
  	for(int i = start*(tam_vet/num_threads); i<(start+1)*(tam_vet/num_threads); i++) {
    	a[i] = i;
		b[i] = i;
		c[i] = 0;
  	}
	free(arg);
  	pthread_exit(NULL);
}

void* sumVectors (void* arg) {
  	int start = *((int *)arg);
  	for(int i = start*(tam_vet/num_threads); i<(start+1)*(tam_vet/num_threads); i++) {
    	c[i] = a[i] + b[i];
  	}
	free(arg);
  	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	int i, imprimir = 0;
	num_threads = 4;
	pthread_t threads[4];

	if(argc<2){
      	printf("uso %s <tamanho vetores> [imprimir? 1=sim]\n", argv[0]);
      	exit(1);
    }

    /* tamanho dos vetores */
    tam_vet = atoi(argv[1]);

    if(argc==3 && atoi(argv[2])==1)
    	imprimir = 1;

	/* alocacao do vetor A */
    a = (double *) malloc(sizeof(double) * tam_vet);

    /* alocacao do vetor B */
    b = (double *) malloc(sizeof(double) * tam_vet);

    /* alocacao do vetor C */
    c = (double *) malloc(sizeof(double) * tam_vet);

    printf("Inicializando vetores A, B e C...\n");

	/* inicializacao dos vetores */
	for(int i = 0; i<num_threads; i++) {
		int* arg = malloc(sizeof(int));
		*arg = i;
    	pthread_create(&threads[i], NULL, initialize, (void *)&arg);
  	}
	for(int i = 0; i<num_threads; i++) {
    	pthread_join(threads[i], NULL);
    	printf("Thread %d encerrada.\n", (int)threads[i]);
  	}

	printf("Calculando...\n");

	/* soma dos vetores */
	for(int i = 0; i<num_threads; i++) {
		int* arg = malloc(sizeof(int));
		*arg = i;
    	pthread_create(&threads[i], NULL, sumVectors, (void *)&arg);
  	}
	for(int i = 0; i<num_threads; i++) {
    	pthread_join(threads[i], NULL);
    	printf("Thread %d encerrada.\n", (int)threads[i]);
  	}

	printf("Terminou!\n");

	if(imprimir)
    {
	    /*** imprime os resultados ***/
	    printf("******************************************************\n");
	    printf("Vetor C:\n");
	    for (i=0; i<tam_vet; i++)
	    	printf("%10.2f  ", c[i]);
	    printf("\n");
	    printf("******************************************************\n");
    }

	free(a);
    free(b);
    free(c);
}
