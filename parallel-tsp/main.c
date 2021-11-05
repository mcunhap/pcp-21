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
#include "headers/tour.h"
#include "headers/stack.h"
#include "headers/graph.h"
#include "headers/tsp.h"

#define HOMETOWN 0
#define NUM_THREADS 2

//Instance
int n_cities = 5;
int nodes[] = { 0,1,2,3,4 };
float adj_m[5][5] = {
  { 0.0, 3.0, 4.0, 2.0, 7.0 },
  { 3.0, 0.0, 4.0, 6.0, 3.0 },
  { 4.0, 4.0, 0.0, 5.0, 8.0 },
  { 2.0, 6.0, 5.0, 0.0, 6.0 },
  { 7.0, 3.0, 8.0, 6.0, 0.0 },
};

//Global for all threads
tour* best_tour;
graph* graph_t;
int stack_size;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, best_tour, n_cities, HOMETOWN);

  pthread_exit(NULL);
}

void InitializeThreadsStack(graph* graph_t, int n_nodes_limit, stack* stacks[n_nodes_limit], int root_node) {
  int n_nodes = 0;

  while(n_nodes < n_nodes_limit) {

  }
}

int main(void) {
  int error;
  best_tour = CreateTour(n_cities + 1);
  graph_t = CreateGraph(n_cities, nodes, adj_m);
  stack_size = (n_cities*n_cities)/2;
  tour* initial_tour = CreateTour(n_cities + 1);
  stack* stacks[NUM_THREADS];

  // each stack initialize with minimal tour, that is only hometown
  for(int i=0; i < NUM_THREADS; i++) {
    stacks[i] = CreateStack(stack_size);
    AddCity(initial_tour, graph_t, HOMETOWN);
    PushCopy(stacks[i], initial_tour);
  }

  InitializeThreadsStack(graph_t, NUM_THREADS, stacks, HOMETOWN);

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

  FreeStack(stack_t);
  FreeGraph(graph_t);
  return 0;
}
