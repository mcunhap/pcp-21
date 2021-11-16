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
#define FILENAME "instances/13.txt"

int n_cities;
int* nodes;
float** adj_m;
float best_tour;
int num_processes;
int process_rank;
graph* graph_t;
pthread_mutex_t execute_mutex;
term* term_t;

void* execute(void* arg) {
  stack* my_stack = (stack*)arg;

  EvaluateTours(my_stack, graph_t, &best_tour, execute_mutex, term_t, NumNodes(graph_t), HOMETOWN, NUM_THREADS, num_processes, process_rank);

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
  int provided;
  double start, end;
  // Initialize the MPI
  MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);

  if(provided < MPI_THREAD_MULTIPLE) {
    printf("Failed to initialize MPI with thread level MPI_THREAD_MULTIPLE");
    MPI_Abort(MPI_COMM_WORLD, 0);
  }

  // Number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

  // Rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  MPI_Barrier(MPI_COMM_WORLD);

  if(process_rank == 0) {
    ReadNCities(&n_cities);
    AllocateInputs(n_cities);
    InitializeInstance();

    for (int dest = 0; dest < num_processes; dest++) {
      if (dest != process_rank) {
        MPI_Send(&n_cities, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(nodes, n_cities, MPI_INT, dest, 0, MPI_COMM_WORLD);
        for(int i=0; i < n_cities; i++)
          MPI_Send(adj_m[i], n_cities, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
      }
    }
  } else {
    MPI_Recv(&n_cities, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    AllocateInputs(n_cities);

    MPI_Recv(nodes, n_cities, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i=0; i < n_cities; i++)
      MPI_Recv(adj_m[i], n_cities, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  start = MPI_Wtime();

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
  FillBFSQueue(num_processes, graph_t, bfs_queue, initial_tour);

  ProcessesSplit(num_processes, process_rank, bfs_queue, graph_t);

  FreeQueue(bfs_queue);

  FreeGraph(graph_t);

  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();

  // Finalize the MPI
  MPI_Finalize();

  if (process_rank == 0) {
    printf("\nBEST TOUR: \n");
    printf("Best tour: %.2f", best_tour);
    printf("\nTotal execution time: %.2fs\n", end-start);
  }

  return 0;
}
