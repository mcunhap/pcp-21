/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/08/2021 22:35:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define N 10
#define P 1
#define C 3
#define I 5

/* passar buffer como argumento das threads, ou manter como variavel global? */
tbuffer *buffer;
sem_t e;

void* produtor () {
  for (int i=0; i<I; i++) {
    int item = rand() % 100;

    printf("Produtor: %d. ", i);
    deposita(buffer, item);
  }

  return NULL;
}

void* consumidor () {
  for (int i=0; i<I; i++) {
    int item = consome(buffer, i);

    printf("Consumidor: %d. Consumiu: %d\n", i, item);
  }

  return NULL;
}

int main (void) {
  pthread_t produtor_thread;
  pthread_t consumidor_thread;
  sem_init(&e, 0, 1);
  /* int numpos, numprod, numcons; */

  /* printf("Digite o numero de espaços na lista:\n"); */
  /* scanf("%d", numpos); */
  /* printf("Digite o numero de produtores:\n"); */
  /* scanf("%d", numprod); */
  /* printf("Digite o numero de consumidores:\n"); */
  /* scanf("%d", numcons) */

  buffer = iniciabuffer(N, P, C);

  printf("size: %d, producers: %d, consumers: %d\n", buffer->numpos, buffer->numprod, buffer->numcos);

  int error = pthread_create(&produtor_thread, NULL, &produtor, NULL);

  if (error) {
    printf("Falha ao criar a thread com id: %lu\n", (long)produtor_thread);
    return 1;
  }

  error = pthread_create(&consumidor_thread, NULL, &consumidor, NULL);

  if (error) {
    printf("Falha ao criar a thread com id: %lu\n", (long)consumidor_thread);
    return 1;
  }

  error = pthread_join(produtor_thread, NULL);

  if (error) {
    printf("Falha ao esperar execucao da thread: %lu\n", (long)produtor_thread);
    return 1;
  }

  error = pthread_join(consumidor_thread, NULL);

  if (error) {
    printf("Falha ao esperar execucao da thread: %lu\n", (long)consumidor_thread);
    return 1;
  }

  finalizabuffer(buffer);
  return 0;
}
