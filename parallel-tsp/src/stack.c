/*
 * =====================================================================================
 *
 *       Filename:  stack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2021 21:23:32
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
#include "../headers/stack.h"


struct stack_t {
  tour** tours;
  int max_size;
  int size;
};

stack* CreateStack(int max_cities) {
  stack *stack_t = (stack*) calloc (1, sizeof(stack));

  if(!stack_t) {
    printf("Failed to create stack.\n");
    exit(-1);
  }

  stack_t->tours = (tour**) calloc (max_cities, sizeof(tour*));
  stack_t->max_size = max_cities;
  stack_t->size = 0;

  return stack_t;
}

void PushCopy(stack* stack_t, tour* tour_t) {
  if (stack_t->size == stack_t->max_size) {
    printf("Stack full!\n");
    return;
  }

  int loc = stack_t->size;
  int tour_size = GetTourMaxCities(tour_t);
  tour* tour_tmp = CreateTour(tour_size);

  CopyTour(tour_tmp, tour_t);

  stack_t->tours[loc] = tour_tmp;
  stack_t->size++;

  printf("Pushed tour\n");
  PrintTourInfo(tour_tmp);
}

tour* Pop(stack* stack_t) {
  if (Empty(stack_t)) {
    printf("Empty stack!\n");
    return NULL;
  }

  stack_t->size--;

  printf("Poped tour\n");
  PrintTourInfo(stack_t->tours[stack_t->size]);

  return stack_t->tours[stack_t->size];
}

int Empty(stack* stack_t) {
  if (stack_t-> size == 0) { return 1; }

  return 0;
}

tour** GetTours(stack* stack_t) {
  return stack_t->tours;
}

int GetSize(stack* stack_t) {
  return stack_t->size;
}
