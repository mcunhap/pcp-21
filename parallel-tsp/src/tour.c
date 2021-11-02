/*
 * =====================================================================================
 *
 *       Filename:  tour.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2021 18:43:07
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
#include "../headers/tour.h"

struct tour_t {
  int *cities;
  int n_cities;
  int cost;
};


tour* CreateTour(int max_cities) {
  tour *tour_t = (tour*) calloc (1, sizeof(tour));

  if(!tour_t) {
    printf("Failed to create tour");
    exit(-1);
  }

  tour_t->cities = (int*) calloc (max_cities, sizeof(int));
  tour_t->n_cities = 0;
  tour_t->cost = 0;

  return tour_t;
}

int* GetTourCities(tour* tour_t) {
  return tour_t->cities;
}

int GetTourNumberCities(tour* tour_t) {
  return tour_t->n_cities;
}

int GetTourCost(tour* tour_t) {
  return tour_t->cost;
}
