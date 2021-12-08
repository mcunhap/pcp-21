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
#include "headers/utils.h"
#include "headers/queue.h"

#define HOMETOWN 0
#define NUM_THREADS 4
#define FILENAME "instances/5.txt"

int n_cities;
int* nodes;
float** adj_m;
float best_tour;
graph* graph_t;
pthread_mutex_t execute_mutex;
term* term_t;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, &best_tour, execute_mutex, term_t, NumNodes(graph_t), HOMETOWN, NUM_THREADS);

  pthread_exit(NULL);
}

void ThreadsSplit(int num_threads, queue* bfs_queue) {
  int error;
  stack* threads_stacks[num_threads];

  // Initialize threads stacks
  for(int i=0; i<NUM_THREADS; i++) {
    threads_stacks[i] = CreateStack((n_cities*n_cities)/2);
  }

  ShareQueue(num_threads, threads_stacks, bfs_queue);

  pthread_t* workers = (pthread_t*) calloc (NUM_THREADS, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  for(int i=0; i < num_threads; i++) {
    error = pthread_create(&workers[i], NULL, &execute, (void*)threads_stacks[i]);
    if(error) { printf("Failed to create thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  for(int i=0; i < num_threads; i++) {
    error = pthread_join(workers[i], NULL);
    if(error) { printf("Failed to join thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  for(int i=0; i < num_threads; i++) {
    FreeStack(threads_stacks[i]);
  }
}

void AllocateInputs(int n_cities) {
  nodes = (int*) malloc (n_cities*sizeof(int));
  if(!nodes) { printf("Failed to allocate nodes array.\n"); exit(-1); }

  adj_m = (float**) malloc (n_cities*sizeof(float*));
  if(!adj_m) { printf("Failed to allocate adj matrix.\n"); exit(-1); }

  for(int i=0; i < n_cities; i++) {
    adj_m[i] = (float*) malloc (n_cities*sizeof(float));
    if(!adj_m[i]) { printf("Failed to allocate adj matrix.\n"); exit(-1); }
  }
}

void InitializeInstance() {
  for(int i=0; i < n_cities; i++) {
    nodes[i] = i;
  }

  ReadInstance(n_cities, FILENAME, adj_m);
}

int main(void) {
  ReadNCities(&n_cities);
  AllocateInputs(n_cities);
  InitializeInstance();

  // Initialize graph
  graph_t = CreateGraph(n_cities, nodes, adj_m);
  int n_cities = NumNodes(graph_t);

  // Initialize globals
  best_tour = -1;
  term_t = CreateTerm();
  pthread_mutex_init(&execute_mutex, NULL);

  // Initial tour
  tour* initial_tour = CreateTour(n_cities + 1);
  AddCity(initial_tour, graph_t, HOMETOWN);

  // Create bfs queue
  queue* bfs_queue = CreateQueue((n_cities*n_cities)/2);
  FillBFSQueue(NUM_THREADS, graph_t, bfs_queue, initial_tour);

  ThreadsSplit(NUM_THREADS, bfs_queue);

  FreeQueue(bfs_queue);
  FreeGraph(graph_t);

  printf("\nBEST TOUR: \n");
  printf("Best tour: %.2f", best_tour);

  return 0;
}
