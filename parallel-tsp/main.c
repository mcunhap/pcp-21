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


int main(void) {
  int max_cities = 5;
  tour *tour_t = CreateTour(max_cities);
  int *tour_cities = GetTourCities(tour_t);

  for (int i=0; i < max_cities; i++) {
    printf("%d ", tour_cities[i]);
  }

  printf("\n %d, %d", GetTourNumberCities(tour_t), GetTourCost(tour_t));

  return 0;
}
