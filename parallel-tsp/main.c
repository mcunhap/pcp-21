/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2021 18:40:37
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
#include "headers/tour.h"
#include "headers/stack.h"
#include "headers/graph.h"
#include "headers/tsp.h"

#define HOMETOWN 0
#define NUM_THREADS 2

//Instance
/* int n_cities = 5; */
/* int nodes[] = { 0,1,2,3,4 }; */
/* float adj_m[5][5] = { */
/*   { 0.0, 3.0, 4.0, 2.0, 7.0 }, */
/*   { 3.0, 0.0, 4.0, 6.0, 3.0 }, */
/*   { 4.0, 4.0, 0.0, 5.0, 8.0 }, */
/*   { 2.0, 6.0, 5.0, 0.0, 6.0 }, */
/*   { 7.0, 3.0, 8.0, 6.0, 0.0 }, */
/* }; */

int n_cities = 13;
int nodes[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
float adj_m[13][13] = {
  {  0, 29, 82, 46, 68, 52, 72, 42, 51, 55, 29, 74, 23 },
  { 29,  0, 55, 46, 42, 43, 43, 23, 23, 31, 41, 51, 11 },
  { 82, 55,  0, 68, 46, 55, 23, 43, 41, 29, 79, 21, 64 },
  { 46, 46, 68,  0, 82, 15, 72, 31, 62, 42, 21, 51, 51 },
  { 68, 42, 46, 82,  0, 74, 23, 52, 21, 46, 82, 58, 46 },
  { 52, 43, 55, 15, 74,  0, 61, 23, 55, 31, 33, 37, 51 },
  { 72, 43, 23, 72, 23, 61,  0, 42, 23, 31, 77, 37, 51 },
  { 42, 23, 43, 31, 52, 23, 42,  0, 33, 15, 37, 33, 33 },
  { 51, 23, 41, 62, 21, 55, 23, 33,  0, 29, 62, 46, 29 },
  { 55, 31, 29, 42, 46, 31, 31, 15, 29,  0, 51, 21, 41 },
  { 29, 41, 79, 21, 82, 33, 77, 37, 62, 51,  0, 65, 42 },
  { 74, 51, 21, 51, 58, 37, 37, 33, 46, 21, 65,  0, 61 },
  { 23, 11, 64, 51, 46, 51, 51, 33, 29, 41, 42, 61,  0 },
};


//Global for all threads
tour* best_tour;
graph* graph_t;
int stack_size;
sem_t execute_sem;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, best_tour, execute_sem, n_cities, HOMETOWN);

  pthread_exit(NULL);
}

int main(void) {
  int error;
  best_tour = CreateTour(n_cities + 1);
  graph_t = CreateGraph(n_cities, nodes, adj_m);
  stack_size = (n_cities*n_cities)/2;
  stack* stacks[NUM_THREADS];

  error = sem_init(&execute_sem, 0, 1);
  if (error) { printf("Falha ao iniciar semaforo"); return 1; }

  InitializeStacks(HOMETOWN, NUM_THREADS, stack_size, stacks, graph_t);
  FillStacks(NUM_THREADS, HOMETOWN, stack_size, graph_t, stacks);

  pthread_t* workers = (pthread_t*) calloc (NUM_THREADS, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  for(int i=0; i < NUM_THREADS; i++) {
    error = pthread_create(&workers[i], NULL, &execute, (void*)stacks[i]);
    if(error) { printf("Failed to create thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  for(int i=0; i < NUM_THREADS; i++) {
    error = pthread_join(workers[i], NULL);
    if(error) { printf("Failed to join thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  printf("BEST TOUR: \n");
  PrintTourInfo(best_tour);

  for(int i=0; i < NUM_THREADS; i++) {
    FreeStack(stacks[i]);
  }

  FreeGraph(graph_t);
  return 0;
}
