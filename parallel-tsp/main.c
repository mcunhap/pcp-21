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


int main(void) {
  int max_cities = 3;
  tour *tour_t = CreateTour(max_cities + 1);
  tour *tour_r = CreateTour(max_cities + 1);
  stack *stack_t = CreateStack(max_cities + 1);

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

  return 0;
}
