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
#include <errno.h>
#include "buffer.h"

#define N 4
#define P 1
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
/* int falta_ler[N][C]; */
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

void print_buffer(tbuffer* buffer) {
  for(int i=0; i<buffer->numpos; i++) {
    printf("posição %d: %d ", i, buffer->itens[i]);
  }
  printf("\n");
}

tbuffer* iniciabuffer (int numpos, int numprod, int numcos) {
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

void finalizabuffer (tbuffer* buffer) {
  free (buffer);
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
  /* if (pb > 0 && falta_ler[next_free] == 0) { */
  /*   pb--; sem_post(&sp); */
  /* } else if (cb > 0 && falta_ler[next_data] > 0) { */
  /*   cb--; sem_post(&sc); */
  /* } else { */
    /* sem_post(&e); */
  /* } */
}

void deposita (tbuffer* buffer, int item) {
  printf("Produziu: %d\n", item);

  /* <await (is_empty(falta_ler[next_free]) fill(falta_ler[next_free], 1)> */
  /* P(e); */
  sem_wait(&e);
  /* if (!is_empty(falta_ler[next_free])) { pb++; V(e); P(sp); } */

  print_falta_ler();
  printf("next_free: %d", next_free);
  printf("\n");
  if (!is_empty(falta_ler[next_free], C)) {
    printf("WAIT LIST\n");
    pb++;
    sem_post(&e);
    sem_wait(&sp);
  }
  /* fill(falta_ler[next_free], 1) */
  fill(falta_ler[next_free], 1, C);
  print_falta_ler();
  /* SIGNAL */
  c_signal();

  /* produz */
  buffer->itens[next_free] = item;

  /* <next_free = (next_free + 1) % N> */
  /* P(e); */
  sem_wait(&e);
  next_free = (next_free + 1) % N;
  /* SIGNAL */
  c_signal();

  printf("\n");
}

int consome (tbuffer* buffer, int meuid) {
  return 6;
}

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

  int sem_init_error = sem_init(&e, 0, 4);
  if (sem_init_error) { printf("Falha ao iniciar semaforo"); return 1;}

  sem_init_error = sem_init(&sp, 0, 1);
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

  print_buffer(buffer);
  print_falta_ler();

  int error = pthread_create(&produtor_thread, NULL, &produtor, NULL);

  if (error) {
    printf("Falha ao criar a thread com id: %lu\n", (long)produtor_thread);
    return 1;
  }

/*   error = pthread_create(&consumidor_thread, NULL, &consumidor, NULL); */

/*   if (error) { */
/*     printf("Falha ao criar a thread com id: %lu\n", (long)consumidor_thread); */
/*     return 1; */
/*   } */

  error = pthread_join(produtor_thread, NULL);

  if (error) {
    printf("Falha ao esperar execucao da thread: %lu\n", (long)produtor_thread);
    return 1;
  }

  /* error = pthread_join(consumidor_thread, NULL); */

  /* if (error) { */
  /*   printf("Falha ao esperar execucao da thread: %lu\n", (long)consumidor_thread); */
  /*   return 1; */
  /* } */

  print_buffer(buffer);

  finalizabuffer(buffer);
  return 0;
}
