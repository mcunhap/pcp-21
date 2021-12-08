/*
 * =====================================================================================
 *
 *       Filename:  circular_array.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/12/2021 20:18:26
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
#include "../headers/circular_array.h"

struct circular_array_t {
  int capacity;
  tour** tours;
};

circular_array* CreateCircularArray(int capacity) {
  circular_array* c_array = (circular_array*) calloc (1, sizeof(circular_array));

  if(!c_array) {
    printf("Failed to create circular array.\n");
    exit(-1);
  }

  c_array->tours = (tour**) calloc (capacity, sizeof(tour*));
  c_array->capacity = capacity;

  return c_array;
}

tour* Get(circular_array* c_array, int i) {
  return c_array->tours[i % c_array->capacity];
}

void Put(circular_array* c_array, int i, tour* tour) {
  c_array->tours[i * c_array->capacity] = tour;
}

void FreeCircularArray(circular_array* c_array) {
  for(int i=0; i< c_array->capacity; i++) {
    free(c_array->tours[i]);
  }

  free(c_array->tours);
}

circular_array* Resize(circular_array* c_array, int bottom, int top) {
  circular_array* new_c_array = CreateCircularArray(c_array + 1);

  for(int i=top; i < bottom; i++) {
    Put(new_c_array, i, Get(c_array, i));
  }

  FreeCircularArray(c_array);
  return new_c_array;
}
