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
  int max_cities;
  int n_cities;
  int cost;
};


tour* CreateTour(int max_cities) {
  tour *tour_t = (tour*) calloc (1, sizeof(tour));

  if(!tour_t) {
    printf("Failed to create tour.\n");
    exit(-1);
  }

  tour_t->cities = (int*) calloc (max_cities, sizeof(int));

  if(!tour_t->cities) {
    printf("Failed to create cities array.\n");
    exit(-1);
  }

  tour_t->max_cities = max_cities;
  tour_t->n_cities = 0;
  tour_t->cost = 0;

  return tour_t;
}

void AddCity(tour* tour_t, int city, float cost) {
  if (tour_t->n_cities == tour_t->max_cities) {
    printf("Tour full!\n");
    return;
  }

  tour_t->cities[tour_t->n_cities] = city;
  tour_t->n_cities++;
  tour_t->cost += cost;
}

int* GetTourCities(tour* tour_t) {
  return tour_t->cities;
}

int GetTourMaxCities(tour* tour_t) {
  return tour_t->max_cities;
}

int GetTourNumberCities(tour* tour_t) {
  return tour_t->n_cities;
}

int GetTourCost(tour* tour_t) {
  return tour_t->cost;
}

void CopyTour(tour* dest, tour* orig) {
  for (int i=0; i < dest->max_cities; i++) {
    dest->cities[i] = orig->cities[i];
  }

  dest->max_cities = orig->max_cities;
  dest->n_cities = orig->n_cities;
  dest->cost = orig->cost;
}

// Just for debugging
void PrintTourInfo(tour* tour_t) {
  for (int i=0; i < tour_t->max_cities; i++) {
    printf("%d ", tour_t->cities[i]);
  }

  printf("\nNCITIES: %d\nCOST: %d\n\n", tour_t->n_cities, tour_t->cost);
}
