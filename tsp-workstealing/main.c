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
#include "headers/deque.h"

#define HOMETOWN 0
#define NUM_THREADS 4
#define FILENAME "instances/8.txt"

int n_cities;
int* nodes;
float** adj_m;
float best_tour;
graph* graph_t;
pthread_mutex_t execute_mutex;
term* term_t;
deque* threads_deque[NUM_THREADS];

void* execute(void* arg) {
  int my_deque = (int)arg;

  EvaluateTours(threads_deque[my_deque], graph_t, &best_tour, execute_mutex, term_t, NumNodes(graph_t), HOMETOWN, NUM_THREADS);

  pthread_exit(NULL);
}

void ThreadsSplit(queue* bfs_queue) {
  int error;

  // Initialize threads deque
  for(int i=0; i<NUM_THREADS; i++) {
    threads_deque[i] = CreateDeque((n_cities*n_cities)/2);
  }

  ShareQueue(NUM_THREADS, threads_deque, bfs_queue);

  pthread_t* workers = (pthread_t*) calloc (NUM_THREADS, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  for(int i=0; i < NUM_THREADS; i++) {
    error = pthread_create(&workers[i], NULL, &execute, (void*)i);
    if(error) { printf("Failed to create thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  for(int i=0; i < NUM_THREADS; i++) {
    error = pthread_join(workers[i], NULL);
    if(error) { printf("Failed to join thread: %lu\n", (long)workers[i]); exit(-1); }
  }

  for(int i=0; i < NUM_THREADS; i++) {
    FreeDeque(threads_deque[i]);
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

  ThreadsSplit(bfs_queue);

  FreeQueue(bfs_queue);
  FreeGraph(graph_t);

  printf("\nBEST TOUR: \n");
  printf("Best tour: %.2f", best_tour);

  /* deque* deque_t = CreateDeque(5); */
  /* tour* tour_t = CreateTour(n_cities + 1); */
  /* tour* tour_s = CreateTour(n_cities + 1); */
  /* tour* tour_q = CreateTour(n_cities + 1); */
  /* AddCity(tour_t, graph_t, 1); */
  /* AddCity(tour_t, graph_t, 2); */
  /* AddCity(tour_s, graph_t, 3); */
  /* AddCity(tour_q, graph_t, 4); */

  /* PushBottomDeque(deque_t, tour_t); */
  /* PushBottomDeque(deque_t, tour_s); */
  /* PushBottomDeque(deque_t, tour_q); */
  /* PrintDeque(deque_t); */

  /* PrintTourInfo(PopTopDeque(deque_t)); */
  /* PrintTourInfo(PopBottomDeque(deque_t)); */
  /* PopBottomDeque(deque_t); */

  /* PrintDeque(deque_t); */
  return 0;
}
