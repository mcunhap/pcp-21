/*
 * =====================================================================================
 *
 *       Filename:  buffer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/08/2021 21:54:20
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
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define N 4
#define P 5
#define C 3
#define I 5

/* passar buffer como argumento das threads, ou manter como variavel global? */
tbuffer *buffer;
sem_t e;
sem_t sp; // semaforo produtor
sem_t sc; // semaforo consumidor

int pb = 0; // produtor bloqueado
int cb = 0; // consumidor bloqueado

int next_free = 0;
int *next_data; // cada pos iniciada com zero
int **falta_ler;

struct sbuffer {
  int numpos;
  int numprod;
  int numcos;
  int *itens;
};

void print_falta_ler() {
  for (int i=0; i<N; i++) {
    for (int j=0; j<C; j++) {
      printf("%d", falta_ler[i][j]);
    }
    printf("\n");
  }
}

void print_buffer(tbuffer* buffer, long thread) {
  printf("[%lu]", thread);
  for(int i=0; i<buffer->numpos; i++) {
    printf("|%d| ", buffer->itens[i]);
  }
  printf("\n");
}

tbuffer* iniciabuffer(int numpos, int numprod, int numcos) {
  tbuffer *buffer = calloc(1, sizeof(tbuffer));

  if(!buffer) {
    return NULL;
  }

  buffer->itens = calloc(numpos, sizeof(int));

  if(!buffer->itens) {
    return NULL;
  }

  buffer->numpos = numpos;
  buffer->numprod = numprod;
  buffer->numcos = numcos;

  return buffer;
}

void finalizabuffer(tbuffer* buffer) {
  free(buffer);
}

int is_empty(int *array, int size) {
  for(int i=0; i<size; i++) {
    if(array[i] > 0) { return 0; }
  }

  return 1;
}

void fill(int *array, int value, int size) {
  for(int i=0; i<size; i++) {
    array[i] = value;
  }
}

int* array_vazio(int tam) {
  int *array = (int*)calloc(tam, sizeof(int));

  return array;
}

int** matriz_vazia(int linhas, int colunas) {
  int **matriz;

  matriz = (int**)calloc(linhas, sizeof(int*));

  for(int i=0; i<linhas; i++) {
    matriz[i] = array_vazio(colunas);
  }

  return matriz;
}

void c_signal() {
  if (pb > 0 && is_empty(falta_ler[next_free], C)) {
    pb--;
    sem_post(&sp);
  /* } else if (cb > 0 && !is_empty(falta_ler[next_data], C)) { */
  /*   cb--; sem_post(&sc); */
  } else {
    sem_post(&e);
  }
}

void deposita(tbuffer* buffer, int item) {
  sem_wait(&e);
  print_buffer(buffer, (long)pthread_self());

  if (!is_empty(falta_ler[next_free], C)) {
    printf("[%lu]WAIT LIST\n", (long)pthread_self());
    pb++;
    sem_post(&e);
    sem_wait(&sp);
  }

  fill(falta_ler[next_free], 1, C);

  printf("[%lu]Producao: %d.\n", (long)pthread_self(), item);
  c_signal();

  /* produz */
  // deveria estar protegido por algum lock?
  buffer->itens[next_free] = item;

  sem_wait(&e);
  next_free = (next_free + 1) % N;
  c_signal();

  printf("\n");
}

int consome (tbuffer* buffer, int meuid) {
  sem_wait(&e);
  /* print_buffer(buffer, (long)pthread_self()); */

  if (falta_ler[next_data[meuid]][meuid] == 0) {
    cb++;
    sem_post(&e);
    sem_wait(&sp);
  }

  falta_ler[next_data[meuid]][meuid] = 0;
  c_signal();

  /* consome */
  int data = buffer->itens[next_data[meuid]];

  sem_post(&e);
  /* printf("%d", data); */
  next_data[meuid] = (next_data[meuid] + 1) % N;
  printf("[%lu]Consumidor: %d. Consumiu: %d\n", pthread_self(), meuid, data);
  c_signal();

  return data;
}

void* produtor () {
  for (int i=0; i<I; i++) {
    int item = rand() % 100;

    deposita(buffer, item);
  }

  return NULL;
}

void* consumidor (void *arg) {
  int *id_consumidor = (int*)arg;

  for (int i=0; i<I; i++) {
    int item = consome(buffer, *id_consumidor);

    item++;
    /* printf("[%lu]Consumidor: %d. Consumiu: %d\n", pthread_self(), *id_consumidor, item); */
  }

  return NULL;
}

int main (void) {
  int error;

  pthread_t *produtores;
  produtores = (pthread_t *)calloc(P, sizeof(pthread_t));

  pthread_t *consumidores;
  consumidores = (pthread_t *)calloc(C, sizeof(pthread_t));


  // Semaforo e inicializado 'liberado'
  int sem_init_error = sem_init(&e, 0, 1);
  if (sem_init_error) { printf("Falha ao iniciar semaforo"); return 1;}

  // Semaforo sp inicializado 'ocupado'
  sem_init_error = sem_init(&sp, 0, 0);
  if (sem_init_error) { printf("Falha ao iniciar semaforo"); return 1;}

  // Semaforo sc inicializado 'ocupado'
  sem_init_error = sem_init(&sc, 0, 0);
  if (sem_init_error) { printf("Falha ao iniciar semaforo"); return 1;}

  /* int numpos, numprod, numcons; */

  /* printf("Digite o numero de espaços na lista:\n"); */
  /* scanf("%d", numpos); */
  /* printf("Digite o numero de produtores:\n"); */
  /* scanf("%d", numprod); */
  /* printf("Digite o numero de consumidores:\n"); */
  /* scanf("%d", numcons) */

  buffer = iniciabuffer(N, P, C);
  next_data = array_vazio(C);
  falta_ler = matriz_vazia(N,C);

  /* printf("tamanho: %d, produtores: %d, consumidores: %d, ", buffer->numpos, buffer->numprod, buffer->numcos); */

  for (int i=0; i<P; i++) {
    error = pthread_create(&produtores[i], NULL, &produtor, NULL);

    if (error) {
      printf("Falha ao criar a thread com id: %lu\n", (long)produtores[i]);
      return 1;
    }
  }

  for (int i=0; i<C; i++) {
    int *id_consumidor = (int*)calloc(1,sizeof(int));
    *id_consumidor = i;

    error = pthread_create(&consumidores[i], NULL, &consumidor, (void*)id_consumidor);

    if (error) {
      printf("Falha ao criar a thread com id: %lu\n", (long)consumidores[i]);
      return 1;
    }
  }

  for (int i=0; i<P; i++) {
    error = pthread_join(produtores[i], NULL);

    if (error) {
      printf("Falha ao esperar execucao da thread: %lu\n", (long)produtores[i]);
      return 1;
    }
  }

  for (int i=0; i<C; i++) {
    error = pthread_join(consumidores[i], NULL);

    if (error) {
      printf("Falha ao esperar execucao da thread: %lu\n", (long)consumidores[i]);
      return 1;
    }
  }

  /* print_buffer(buffer); */

  finalizabuffer(buffer);
  return 0;
}
