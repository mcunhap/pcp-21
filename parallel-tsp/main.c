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
  int size = 5;
  float adj_m[5][5] = {
    { 0.0, 3.0, 4.0, 2.0, 7.0 },
    { 3.0, 0.0, 4.0, 6.0, 3.0 },
    { 4.0, 4.0, 0.0, 5.0, 8.0 },
    { 2.0, 6.0, 5.0, 0.0, 6.0 },
    { 7.0, 3.0, 8.0, 6.0, 0.0 },
  };

  int max_cities = 3;
  float edge_weight;
  tour *tour_t = CreateTour(max_cities + 1);
  tour *tour_r = CreateTour(max_cities + 1);
  stack *stack_t = CreateStack(max_cities + 1);
  graph *graph_t = CreateGraph(size, adj_m);

  AddCity(tour_t, 5);
  AddCity(tour_t, 2);
  AddCity(tour_t, 8);
  AddCity(tour_t, 2);
  AddCity(tour_r, 7);

  PushCopy(stack_t, tour_t);
  PushCopy(stack_t, tour_r);

  tour *tour_e = Pop(stack_t);
  tour_e = Pop(stack_t);
  tour_e = Pop(stack_t);

  PushCopy(stack_t, tour_t);

  tour_e = Pop(stack_t);
  tour_e = Pop(stack_t);

  PrintGraph(graph_t, size);

  edge_weight = GetEdgeWeight(graph_t, 0, 3);
  printf("%.2f\n", edge_weight);

  return 0;
}
