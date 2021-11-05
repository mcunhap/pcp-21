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

void EvaluateTours(stack* stack_t, graph* graph_t, tour* best_tour, int n_cities, int hometown) {
  tour* current_tour;

  while(!Empty(stack_t)) {
    current_tour = Pop(stack_t);

    if(GetTourNumberCities(current_tour) == n_cities) {
      // add hometown to current tour to compute the final cost
      AddCity(current_tour, graph_t, hometown);

      if(BestTour(current_tour, best_tour)) {
        printf("Update best tour!\n");
        PrintTourInfo(current_tour);
        CopyTour(best_tour, current_tour);
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
