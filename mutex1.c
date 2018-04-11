#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global;
pthread_mutex_t mutex;

void* rotina (void* arg) {
  int num_threads = *((int *)arg);
  for(int i = 0; i<num_threads * 1000; i++) {
    pthread_mutex_lock(&mutex);
    contador_global++;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int num_threads;
  pthread_mutex_init(&mutex, NULL);

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

  pthread_mutex_destroy(&mutex);
  printf("\nContador global: %d\n", contador_global);

  return 0;
}
