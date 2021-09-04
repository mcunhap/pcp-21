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
#include <unistd.h>
#include "buffer.h"

int N, P, C, I;

tbuffer *buffer;
sem_t e;
sem_t sp;
sem_t sc;

int pb = 0;
int cb = 0;

int produzidos = 0;
int *consumidos;

int next_free = 0;
int *next_data;
int *falta_ler;

struct sbuffer {
  int numpos;
  int numprod;
  int numcos;
  int *itens;
};

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

void entrada_parametros() {
  printf("Digite o numero de espaços na lista:\n");
  scanf("%d", &N);
  printf("Digite o numero de produtores:\n");
  scanf("%d", &P);
  printf("Digite o numero de consumidores:\n");
  scanf("%d", &C);
  printf("Digite o numero de itens:\n");
  scanf("%d", &I);
}

void print_buffer(tbuffer* buffer, long thread) {
  printf("[%lu]", thread);
  for(int i=0; i<buffer->numpos; i++) {
    printf("|%d| ", buffer->itens[i]);
  }
  printf("\n");
}

void print_produtor_wait_list() {
  printf("\033[0;33m[%lu][Produtor] WAIT LIST\033[0m\n", (long)pthread_self());
}

void print_producao(item, produzidos) {
  printf("\033[0;31m[%lu]Producao: %d. Total: %d\033[0m\n", (long)pthread_self(), item, produzidos);
}

void print_consumidor_wait_list(meuid) {
  printf("\033[0;33m[%lu][Consumidor %d] WAIT LIST\033[0m\n", (long)pthread_self(), meuid);
}

void print_consumo(meuid, item, consumidos) {
  printf("\033[0;32m[%lu][Consumidor %d] Consumiu: %d. Total: %d\033[0m\n", pthread_self(), meuid, item, consumidos);
}

int* array_vazio(int tam) {
  int *array = (int*)calloc(tam, sizeof(int));

  return array;
}

void sinal(int tam_buffer) {
  if (pb > 0 && falta_ler[next_free] == 0) {
    pb--; sem_post(&sp);
  } else if (cb > 0 && falta_ler[(next_free - 1 + tam_buffer) % tam_buffer] > 0) {
    cb--; sem_post(&sc);
  } else {
    sem_post(&e);
  }
}

void deposita(tbuffer* buffer, int item) {
  sem_wait(&e);

  if (falta_ler[next_free] > 0) {
    /* log */
    print_produtor_wait_list();

    pb++;
    sem_post(&e);
    sem_wait(&sp);
  }

  falta_ler[next_free] = buffer->numcos;

  buffer->itens[next_free] = item;
  produzidos++;

  /* log */
  print_producao(item, produzidos);
  print_buffer(buffer, (long)pthread_self());

  next_free = (next_free + 1) % buffer->numpos;
  sinal(buffer->numpos);
}

int consome(tbuffer* buffer, int meuid) {
  sem_wait(&e);

  // Colocamos while em vez de if devido aos consumidores gulosos, que tentam comer o que ja comeram. Com o while o teste consumidos[meuid] == produzidos acontece novamente travando eles.
  while(falta_ler[next_data[meuid]] == 0 || consumidos[meuid] == produzidos) {
    /* log */
    print_consumidor_wait_list(meuid);

    cb++;
    sem_post(&e);
    sem_wait(&sc);
  }

  falta_ler[next_data[meuid]]--;
  int item = buffer->itens[next_data[meuid]];
  consumidos[meuid]++;
  next_data[meuid] = (next_data[meuid] + 1) % buffer->numpos;

  /* log */
  print_consumo(meuid, item, consumidos[meuid]);

  sinal(buffer->numpos);

  return item;
}

void* produtor () {
  for (int i=0; i<I; i++) {
    int item = rand() % 1000;

    deposita(buffer, item);
  }

  return NULL;
}

void* consumidor (void *arg) {
  int *id_consumidor = (int*)arg;

  for (int i=0; i<P*I; i++) {
    int item = consome(buffer, *id_consumidor);
  }

  return NULL;
}



int main (void) {
  int error;
  pthread_t *produtores;
  pthread_t *consumidores;

  entrada_parametros();

  produtores = (pthread_t *)calloc(P, sizeof(pthread_t));
  consumidores = (pthread_t *)calloc(C, sizeof(pthread_t));

  // Semaforo e inicializado 'liberado'
  error = sem_init(&e, 0, 1);
  if (error) { printf("Falha ao iniciar semaforo"); return 1;}

  // Semaforo sp inicializado 'ocupado'
  error = sem_init(&sp, 0, 0);
  if (error) { printf("Falha ao iniciar semaforo"); return 1;}

  // Semaforo sc inicializado 'ocupado'
  error = sem_init(&sc, 0, 0);
  if (error) { printf("Falha ao iniciar semaforo"); return 1;}

  buffer = iniciabuffer(N, P, C);
  next_data = array_vazio(C);
  consumidos = array_vazio(C);
  falta_ler = array_vazio(N);

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

  finalizabuffer(buffer);

  return 0;
}
