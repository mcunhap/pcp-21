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
#include "headers/tour.h"
#include "headers/stack.h"
#include "headers/graph.h"

int main(void) {
  float cost;
  int hometown = 0;

  int size = 5;
  int nodes[] = { 0,1,2,3,4 };
  float adj_m[5][5] = {
    { 0.0, 3.0, 4.0, 2.0, 7.0 },
    { 3.0, 0.0, 4.0, 6.0, 3.0 },
    { 4.0, 4.0, 0.0, 5.0, 8.0 },
    { 2.0, 6.0, 5.0, 0.0, 6.0 },
    { 7.0, 3.0, 8.0, 6.0, 0.0 },
  };
  graph *graph_t = CreateGraph(size, nodes, adj_m);

  int n_cities = 5;
  // TODO: check stack size
  stack *stack_t = CreateStack(99);
  tour *initial_tour = CreateTour(n_cities + 1);
  tour *current_tour, *best_tour;

  cost = GetEdgeWeight(graph_t, hometown, hometown);
  AddCity(initial_tour, hometown, cost);

  PushCopy(stack_t, initial_tour);

  while(!Empty(stack_t)) {
    current_tour = Pop(stack_t);

    if(GetTourNumberCities(current_tour) == n_cities) {
      // add hometown to current tour to compute the final cost
      cost = GetEdgeWeight(graph_t, LastCity(current_tour), hometown);
      AddCity(current_tour, hometown, cost);

      if(BestTour(current_tour, best_tour)) {
        printf("Update best tour!\n");
        PrintTourInfo(current_tour);
        best_tour = current_tour;
      }
    } else {
      for (int nbr=n_cities-1; nbr >= 1; nbr--) {
        if(!TourContainCity(current_tour, nbr)) {
          cost = GetEdgeWeight(graph_t, LastCity(current_tour), nbr);
          AddCity(current_tour, nbr, cost);
          PushCopy(stack_t, current_tour);
          RemoveLastCity(current_tour, cost);
        }
      }
    }
  }

  printf("BEST TOUR: \n");
  PrintTourInfo(best_tour);

  return 0;
}
