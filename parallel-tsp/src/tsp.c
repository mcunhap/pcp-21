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

void EvaluateTours(stack* stack_t, graph* graph_t, tour* best_tour, sem_t evaluate_sem, int n_cities, int hometown) {
  tour* current_tour;

  while(!Empty(stack_t)) {
    current_tour = Pop(stack_t);

    if(GetTourNumberCities(current_tour) == n_cities) {
      // add hometown to current tour to compute the final cost
      AddCity(current_tour, graph_t, hometown);

      if(BestTour(current_tour, best_tour)) {
        sem_wait(&evaluate_sem);

        printf("Update best tour!\n");
        PrintTourInfo(current_tour);
        CopyTour(best_tour, current_tour);

        sem_post(&evaluate_sem);
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


int AnyStackNotFilled(int num_threads, stack* stacks[num_threads]) {
  for(int i=0; i < num_threads; i++) {
    if(GetTourNumberCities(GetLastTour(stacks[i])) == 1 ) { return 1; }
  }

  return 0;
}

void InitializeStacks(int root_node, int n_stacks, int stack_size, stack* stacks[n_stacks], graph* graph_t) {
  tour* initial_tour = CreateTour(NumNodes(graph_t) + 1);
  AddCity(initial_tour, graph_t, root_node);

  // each stack initialize with minimal tour, that is only root node
  for(int i=0; i < n_stacks; i++) {
    stacks[i] = CreateStack(stack_size);
    PushCopy(stacks[i], initial_tour);
  }
}

void FillStacks(int num_threads, int root_node, int stack_size, graph* graph_t, stack* stacks[num_threads]) {
  float nbr_cost;
  int num_nodes = NumNodes(graph_t);
  int visited_nodes[num_nodes], current_node;
  tour* current_tour;
  stack* current_stack;

  // initialize visited nodes array
  for(int i=0; i < num_nodes; i++) { visited_nodes[i] = 0; }

  current_node = root_node;

  while(AnyStackNotFilled(num_threads, stacks)) {
    for(int nbr=num_nodes-1; nbr >= 0; nbr--) {
      nbr_cost = GetEdgeWeight(graph_t, current_node, nbr);

      // current node not neighbour from nbr... this kind of loop is not so efficient, we always itera through all nodes - 1
      if (nbr_cost == 0.0 || visited_nodes[current_node] == 1) { continue; }

      current_stack = stacks[nbr % num_threads];
      current_tour = GetLastTour(current_stack);
      AddCity(current_tour, graph_t, nbr);
      PushCopy(current_stack, current_tour);

      visited_nodes[nbr] = 1;
    }

    current_node = GetTourLastCity(GetLastTour(stacks[0]));
  }
}
