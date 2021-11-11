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
/* #include <mpi.h> */
#include "headers/tour.h"
#include "headers/stack.h"
#include "headers/graph.h"
#include "headers/tsp.h"
#include "headers/utils.h"

#define HOMETOWN 0
#define NUM_THREADS 2

/* int n_cities; */
/* int* nodes; */
/* float** adj_m; */

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
pthread_mutex_t execute_mutex;
term* term_t;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, best_tour, execute_mutex, term_t, NumNodes(graph_t), HOMETOWN, NUM_THREADS);

  pthread_exit(NULL);
}

void ThreadsSplit(int root_node, int num_threads, stack** stacks, graph* graph_t) {
  int error;
  int n_nodes = NumNodes(graph_t);
  int stack_size = n_nodes * n_nodes;

  InitializeStacks(root_node, num_threads, stack_size, stacks, graph_t);
  FillStacks(root_node, num_threads, stack_size, stacks, graph_t);

  for(int i=0; i < num_threads; i++) {
    PrintStackInfo(stacks[i]);
  }

  /* pthread_t* workers = (pthread_t*) calloc (NUM_THREADS, sizeof(pthread_t)); */
  /* if (!workers) { exit(-1); } */

  /* for(int i=0; i < num_threads; i++) { */
  /*   error = pthread_create(&workers[i], NULL, &execute, (void*)stacks[i]); */
  /*   if(error) { printf("Failed to create thread: %lu\n", (long)workers[i]); exit(-1); } */
  /* } */

  /* for(int i=0; i < num_threads; i++) { */
  /*   error = pthread_join(workers[i], NULL); */
  /*   if(error) { printf("Failed to join thread: %lu\n", (long)workers[i]); exit(-1); } */
  /* } */
}

void ProcessSplit(int root_node, int num_process, stack** stacks, graph* graph_t) {
  int n_nodes = NumNodes(graph_t);
  int stack_size = n_nodes * n_nodes;

  InitializeStacks(root_node, num_process, stack_size, stacks, graph_t);
  FillStacks(root_node, num_process, stack_size, stacks, graph_t);
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
  /* int num_process, process_rank; */

  /* MPI_Init(NULL, NULL); */

  /* MPI_Comm_size(MPI_COMM_WORLD, &num_process); */
  /* MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); */

  /* InitializeInstance(); */

  /* MPI_Bcast(&n_cities, 1, MPI_INT, 0, MPI_COMM_WORLD); */
  /* MPI_Bcast(&nodes, n_cities, MPI_INT, 0, MPI_COMM_WORLD); */

  graph_t = CreateGraph(n_cities, nodes, adj_m);

  /* stack* process_stacks[num_process]; */

  /* ProcessSplit(HOMETOWN, num_process, process_stacks, graph_t); */

  /* stack* process_stack = process_stacks[process_rank]; */
  /* PrintStackInfo(process_stack); */

  int n_cities = NumNodes(graph_t);
  best_tour = CreateTour(n_cities + 1);
  stack_size = (n_cities*n_cities)/2;
  stack* threads_stacks[NUM_THREADS];

  term_t = CreateTerm();

  pthread_mutex_init(&execute_mutex, NULL);

  ThreadsSplit(HOMETOWN, NUM_THREADS, threads_stacks, graph_t);

  printf("\nBEST TOUR: \n");
  PrintTourInfo(best_tour);

  /* for(int i=0; i < num_process; i++) { */
  /*   FreeStack(process_stacks[i]); */
  /* } */

  for(int i=0; i < NUM_THREADS; i++) {
    FreeStack(threads_stacks[i]);
  }

  FreeGraph(graph_t);

  /* MPI_Finalize(); */
  return 0;
}
