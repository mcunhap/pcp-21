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

int Termination(deque** deques, int my_id, int num_threads, pthread_mutex_t top_mutex, term* term_t) {
  deque* my_deque = deques[my_id];
  tour* top_tour = NULL;
  /* int i = 0; */

  if (!EmptyDeque(my_deque)) {
    return 0;
  } else {
    pthread_mutex_lock(&top_mutex);

    // can iterate in all deque and still get NULL, so in external loop we jump to try again
    for(int i=0; i < num_threads; i++) {
      if(i == my_id) { continue; }

      deque* current_deque = deques[i];
      top_tour = PopTopDeque(current_deque);

      if(top_tour != NULL) {
        PushBottomDeque(my_deque, top_tour);
        pthread_mutex_unlock(&top_mutex);
        return 0;
      }
    }
  }

  pthread_mutex_unlock(&top_mutex);
  return 1;
}

void EvaluateTours(deque** deques, graph* graph_t, float* best_tour, pthread_mutex_t evaluate_mutex, pthread_mutex_t top_mutex, term* term_t, int n_cities, int hometown, int num_threads, int my_id) {
  tour* current_tour;
  deque* deque_t = deques[my_id];

  while(!Termination(deques, my_id, num_threads, top_mutex, term_t)) {
    current_tour = PopBottomDeque(deque_t);
    // if after try get job from anothers deques still get NULL job, then skip to try again
    if(current_tour == NULL) { continue; }

    if(GetTourNumberCities(current_tour) == n_cities) {
      // add hometown to current tour to compute the final cost
      AddCity(current_tour, graph_t, hometown);

      if(BestTour(current_tour, *best_tour)) {
        pthread_mutex_lock(&evaluate_mutex);
        *best_tour = GetTourCost(current_tour);
        pthread_mutex_unlock(&evaluate_mutex);
      }
    } else {
      for (int nbr=n_cities-1; nbr >= 0; nbr--) {
        // dont go back to hometown, with this we can set hometown to any city
        if (nbr == hometown) { continue; }

        if(!TourContainCity(current_tour, nbr)) {
          AddCity(current_tour, graph_t, nbr);
          if(GetTourCost(current_tour) > *best_tour && *best_tour != -1) {
            RemoveLastCity(current_tour, graph_t);
            continue;
          }

          pthread_mutex_lock(&top_mutex);
          PushBottomDeque(deque_t, current_tour);
          pthread_mutex_unlock(&top_mutex);
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

  while(SizeQueue(bfs_queue) <= num_instances) {
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

void ShareQueue(int num_instances, deque** deques, queue* queue_t) {
  int i = 0;

  while(!EmptyQueue(queue_t)) {
    deque* current_deque = deques[i % num_instances];
    PushBottomDeque(current_deque, Dequeue(queue_t));

    i++;
  }
}
