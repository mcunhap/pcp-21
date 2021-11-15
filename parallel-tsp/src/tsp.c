/*
 * =====================================================================================
 *
 *       Filename:  tsp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/11/2021 22:37:48
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
#include "../headers/tsp.h"

struct term_t {
  stack* new_stack;
  int threads_in_cond_wait;
  pthread_cond_t term_cond_var;
  pthread_mutex_t term_mutex;
};

term* CreateTerm() {
  term* term_t = (term*) calloc (1, sizeof(term));

  if(!term_t) { printf("Failed to create term.\n"); exit(-1); }

  term_t->new_stack = NULL;
  term_t->threads_in_cond_wait = 0;
  pthread_cond_init(&term_t->term_cond_var, NULL);
  pthread_mutex_init(&term_t->term_mutex, NULL);

  return term_t;
}

int Termination(stack* my_stack, term* term_t, int num_threads) {
  int my_stack_size = GetSize(my_stack);

  if(my_stack_size >= 2 && term_t->threads_in_cond_wait > 0 && term_t->new_stack == NULL) {
    pthread_mutex_lock(&term_t->term_mutex);

    if(term_t->threads_in_cond_wait > 0 && term_t->new_stack == NULL) {
      /* printf("splitting stack...\n"); */
      term_t->new_stack = SplitStack(my_stack);

      pthread_cond_signal(&term_t->term_cond_var);
    }

    pthread_mutex_unlock(&term_t->term_mutex);

    return 0;
  } else if (!Empty(my_stack)) {
    return 0;
  } else {
    pthread_mutex_lock(&term_t->term_mutex);

    if(term_t->threads_in_cond_wait == num_threads - 1) {
      // last thread running
      term_t->threads_in_cond_wait++;
      pthread_cond_broadcast(&term_t->term_cond_var);

      pthread_mutex_unlock(&term_t->term_mutex);

      return 1;
    } else {
      // other threads stil working... wait for work
      term_t->threads_in_cond_wait++;
      while(pthread_cond_wait(&term_t->term_cond_var, &term_t->term_mutex) != 0);

      if(term_t->threads_in_cond_wait < num_threads) {
        CopyStack(term_t->new_stack, my_stack);

        term_t->new_stack = NULL;
        term_t->threads_in_cond_wait--;

        pthread_mutex_unlock(&term_t->term_mutex);

        return 0;
      } else {
        pthread_mutex_unlock(&term_t->term_mutex);

        return 1;
      }
    }
  }
}

void CheckNewBestTour(float* best_tour, int src) {
  int msg_available;
  float received_cost;
  MPI_Status status;
  MPI_Message msg;

  MPI_Improbe(src, 0, MPI_COMM_WORLD, &msg_available, &msg, &status);
  if(msg_available) {
    MPI_Mrecv(&received_cost, 1, MPI_FLOAT, &msg, MPI_STATUS_IGNORE);
  }
}

void SendNewBestTour(float* best_tour, int num_processes, int process_rank) {
  for(int dest = 0; dest < num_processes; dest++) {
    if(dest != process_rank)
      MPI_Send(best_tour, 1, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
  }
}

void EvaluateTours(stack* stack_t, graph* graph_t, float* best_tour, pthread_mutex_t evaluate_mutex, term* term_t, int n_cities, int hometown, int num_threads, int num_processes, int process_rank) {
  tour* current_tour;

  while(!Termination(stack_t, term_t, num_threads)) {
    current_tour = Pop(stack_t);

    if(GetTourNumberCities(current_tour) == n_cities) {
      // add hometown to current tour to compute the final cost
      AddCity(current_tour, graph_t, hometown);

      for(int src = 0; src < num_processes; src++)
        CheckNewBestTour(best_tour, src);

      if(BestTour(current_tour, *best_tour)) {
        pthread_mutex_lock(&evaluate_mutex);

        /* printf("Update best tour!\n"); */
        /* PrintTourInfo(current_tour); */

        *best_tour = GetTourCost(current_tour);

        SendNewBestTour(best_tour, num_processes, process_rank);

        pthread_mutex_unlock(&evaluate_mutex);
      }
    } else {
      for (int nbr=n_cities-1; nbr >= 0; nbr--) {
        // dont go back to hometown, with this we can set hometown to any city
        if (nbr == hometown) { continue; }

        if(!TourContainCity(current_tour, nbr)) {
          AddCity(current_tour, graph_t, nbr);
          PushCopy(stack_t, current_tour);
          RemoveLastCity(current_tour, graph_t);
        }
      }
    }
    FreeTour(current_tour);
  }
}

void FillBFSQueue(int num_instances, graph* graph_t, queue* bfs_queue, tour* initial_tour) {
  int num_cities = NumNodes(graph_t);
  int visited_nodes[num_cities];
  tour* current_tour;

  EnqueueCopy(bfs_queue, initial_tour);

  // initialize visited nodes array
  for(int i=0; i < num_cities; i++) {
    if(TourContainCity(initial_tour, i)) {
      visited_nodes[i] = 1;
    } else {
      visited_nodes[i] = 0;
    }
  }

  // bug with five city instance...
  while(SizeQueue(bfs_queue) < num_instances) {
    current_tour = Dequeue(bfs_queue);
    int last_city = GetTourLastCity(current_tour);

    for(int nbr=num_cities-1; nbr >= 0; nbr--) {
      int nbr_cost = GetEdgeWeight(graph_t, last_city, nbr);

      // current node not neighbour from nbr... this kind of loop is not so efficient, we always itera through all nodes - 1
      if (nbr_cost == 0.0 || visited_nodes[nbr] == 1) { continue; }

      AddCity(current_tour, graph_t, nbr);
      EnqueueCopy(bfs_queue, current_tour);
      RemoveLastCity(current_tour, graph_t);

      visited_nodes[nbr] = 1;
    }
  }
}

void ShareQueue(int num_instances, stack** stacks, queue* queue_t) {
  int i = 0;

  while(!EmptyQueue(queue_t)) {
    stack* current_stack = stacks[i % num_instances];
    PushCopy(current_stack, Dequeue(queue_t));

    i++;
  }
}
