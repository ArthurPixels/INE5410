#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global;

void* rotina (void* arg) {
  int num_threads = *((int *)arg);
  for(int i = 0; i<num_threads * 1000; i++) {
    contador_global++;
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int num_threads;

  contador_global = 0;
  num_threads = atoi(argv[1]);
  pthread_t threads[num_threads];
  for(int i = 0; i<num_threads; i++) {
    pthread_create(&threads[i], NULL, rotina, (void *)&num_threads);
  }

  for(int i = 0; i<num_threads; i++) {
    pthread_join(threads[i], NULL);
    printf("Thread %d encerrada.\n", (int)threads[i]);
  }

  printf("\nContador global: %d\n", contador_global);

  return 0;
}
