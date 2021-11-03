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

  /* int size = 17; */
  /* int nodes[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }; */
  /* float adj_m[17][17] = { */
  /*   {   0, 633, 257,  91, 412, 150,  80, 134, 259, 505, 353, 324,  70, 211, 268, 246, 121 }, */
  /*   { 633,   0, 390, 661, 227, 488, 572, 530, 555, 289, 282, 638, 567, 466, 420, 745, 518 }, */
  /*   { 257, 390,   0, 228, 169, 112, 196, 154, 372, 262, 110, 437, 191,  74,  53, 472, 142 }, */
  /*   {  91, 661, 228,   0, 383, 120,  77, 105, 175, 476, 324, 240,  27, 182, 239, 237,  84 }, */
  /*   { 412, 227, 169, 383,   0, 267, 351, 309, 338, 196,  61, 421, 346, 243, 199, 528, 297 }, */
  /*   { 150, 488, 112, 120, 267,   0,  63,  34, 264, 360, 208, 329,  83, 105, 123, 364,  35 }, */
  /*   {  80, 572, 196,  77, 351,  63,   0,  29, 232, 444, 292, 297,  47, 150, 207, 332,  29 }, */
  /*   { 134, 530, 154, 105, 309,  34,  29,   0, 249, 402, 250, 314,  68, 108, 165, 349,  36 }, */
  /*   { 259, 555, 372, 175, 338, 264, 232, 249,   0, 495, 352,  95, 189, 326, 383, 202, 236 }, */
  /*   { 505, 289, 262, 476, 196, 360, 444, 402, 495,   0, 154, 578, 439, 336, 240, 685, 390 }, */
  /*   { 353, 282, 110, 324,  61, 208, 292, 250, 352, 154,   0, 435, 287, 184, 140, 542, 238 }, */
  /*   { 324, 638, 437, 240, 421, 329, 297, 314,  95, 578, 435,   0, 254, 391, 448, 157, 301 }, */
  /*   {  70, 567, 191,  27, 346,  83,  47,  68, 189, 439, 287, 254,   0, 145, 202, 289,  55 }, */
  /*   { 211, 466,  74, 182, 243, 105, 150, 108, 326, 336, 184, 391, 145,   0,  57, 426,  96 }, */
  /*   { 268, 420,  53, 239, 199, 123, 207, 165, 383, 240, 140, 448, 202,  57,   0, 483, 153 }, */
  /*   { 246, 745, 472, 237, 528, 364, 332, 349, 202, 685, 542, 157, 289, 426, 483,   0, 336 }, */
  /*   { 121, 518, 142,  84, 297,  35,  29,  36, 236, 390, 238, 301,  55,  96, 153, 336,   0 }, */
  /* }; */

  graph *graph_t = CreateGraph(size, nodes, adj_m);

  int n_cities = size;
  int stack_size = (n_cities*n_cities)/2;

  stack *stack_t = CreateStack(stack_size);
  tour *initial_tour = CreateTour(n_cities + 1);
  tour *best_tour = CreateTour(n_cities + 1);
  tour *current_tour;

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
        CopyTour(best_tour, current_tour);
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
    FreeTour(current_tour);
  }

  printf("BEST TOUR: \n");
  PrintTourInfo(best_tour);

  FreeStack(stack_t);
  FreeGraph(graph_t);
  return 0;
}
