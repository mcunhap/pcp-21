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
  float adj_m[5][5] = {
    { 0.0, 3.0, 4.0, 2.0, 7.0 },
    { 3.0, 0.0, 4.0, 6.0, 3.0 },
    { 4.0, 4.0, 0.0, 5.0, 8.0 },
    { 2.0, 6.0, 5.0, 0.0, 6.0 },
    { 7.0, 3.0, 8.0, 6.0, 0.0 },
  };
  graph *graph_t = CreateGraph(size, adj_m);

  int n_cities = 5;
  stack *stack_t = CreateStack(n_cities + 1);
  tour *initial_tour = CreateTour(n_cities + 1);

  cost = GetEdgeWeight(graph_t, hometown, hometown);
  AddCity(initial_tour, hometown, cost);

  PushCopy(stack_t, initial_tour);

  return 0;
}
