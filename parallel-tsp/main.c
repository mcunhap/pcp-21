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
#include <mpi.h>
#include "headers/tour.h"
#include "headers/stack.h"
#include "headers/graph.h"
#include "headers/tsp.h"
#include "headers/utils.h"
#include "headers/queue.h"

#define HOMETOWN 0
#define NUM_THREADS 4

/* int n_cities; */
/* int* nodes; */
/* float** adj_m; */

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
pthread_mutex_t execute_mutex;
term* term_t;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, best_tour, execute_mutex, term_t, NumNodes(graph_t), HOMETOWN, NUM_THREADS);

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

void ProcessesSplit(int num_processes, int process_rank, queue* bfs_queue, graph* graph_t) {
  stack* my_stack;
  queue* threads_bfs_queue = CreateQueue(MaxSizeQueue(bfs_queue));
  stack* processes_stacks[num_processes];

  for(int i=0; i < num_processes; i++) {
    processes_stacks[i] = CreateStack((n_cities*n_cities)/2);
  }

  ShareQueue(num_processes, processes_stacks, bfs_queue);
  my_stack = processes_stacks[process_rank];

  while(!Empty(my_stack)) {
    FillBFSQueue(NUM_THREADS, graph_t, threads_bfs_queue, Pop(my_stack));
  }

  ThreadsSplit(NUM_THREADS, threads_bfs_queue);

  for(int i=0; i < num_processes; i++) {
    FreeStack(processes_stacks[i]);
  }

  FreeQueue(threads_bfs_queue);
}

/* void InitializeInstance() { */
/*   ReadNCities(&n_cities); */

/*   nodes = (int*) malloc (n_cities*sizeof(int)); */
/*   if(!nodes) { printf("Failed to allocate nodes array.\n"); exit(-1); } */

/*   for(int i=0; i < n_cities; i++) { */
/*     nodes[i] = i; */
/*   } */

/*   adj_m = (float**) malloc (n_cities*sizeof(float*)); */
/*   if(!adj_m) { printf("Failed to allocate adj matrix.\n"); exit(-1); } */

/*   for(int i=0; i < n_cities; i++) { */
/*     adj_m[i] = (float*) malloc (n_cities*sizeof(float)); */
/*     if(!adj_m[i]) { printf("Failed to allocate adj matrix.\n"); exit(-1); } */
/*   } */

/*   ReadInstance(n_cities, "instances/five.txt", adj_m); */
/* } */

int main(void) {
  /* InitializeInstance(); */
  // Initialize the MPI
  MPI_Init(NULL, NULL);

  // Number of processes
  int num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

  // Rank of the process
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  // Initialize graph
  graph_t = CreateGraph(n_cities, nodes, adj_m);
  int n_cities = NumNodes(graph_t);

  // Initialize globals
  best_tour = CreateTour(n_cities + 1);
  term_t = CreateTerm();
  pthread_mutex_init(&execute_mutex, NULL);

  // Initial tour
  tour* initial_tour = CreateTour(n_cities + 1);
  AddCity(initial_tour, graph_t, HOMETOWN);

  // Create bfs queue
  queue* bfs_queue = CreateQueue((n_cities*n_cities)/2);
  FillBFSQueue(num_processes, graph_t, bfs_queue, initial_tour);

  ProcessesSplit(num_processes, process_rank, bfs_queue, graph_t);

  printf("\nBEST TOUR: \n");
  PrintTourInfo(best_tour);

  FreeQueue(bfs_queue);

  FreeGraph(graph_t);

  // Finalize the MPI
  MPI_Finalize();
  return 0;
}
