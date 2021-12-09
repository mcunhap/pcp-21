/*
 * =====================================================================================
 *
 *       Filename:  deque.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/12/2021 22:23:18
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
#include "../headers/deque.h"

struct deque_t {
  circular_array* c_array;
  int top;
  int bottom;
  pthread_mutex_t top_mutex;
};

deque* CreateDeque(int capacity) {
  deque* deque_t = (deque*) calloc (1, sizeof(deque));

  if(!deque_t) {
    printf("Failed to create deque.\n");
    exit(-1);
  }

  deque_t->c_array = CreateCircularArray(capacity);
  deque_t->top = 0;
  deque_t->bottom = 0;
  pthread_mutex_init(&deque_t->top_mutex, NULL);

  return deque_t;
}

int GetTop(deque* deque_t) {
  int top;

  pthread_mutex_lock(&deque_t->top_mutex);
  top = deque_t->top;
  pthread_mutex_unlock(&deque_t->top_mutex);

  return top;
}

int EmptyDeque(deque* deque_t) {
 int top = GetTop(deque_t);
 int bottom = deque_t->bottom;

 if(bottom <= top) {
   return 1;
 }

 return 0;
}

void PushBottomDeque(deque* deque_t, tour* tour_t) {
  int old_bottom = deque_t->bottom;
  int old_top = GetTop(deque_t);
  int size = old_bottom - old_top;
  circular_array* current_c_array = deque_t->c_array;

  if(size >= GetCapacity(current_c_array) - 1) {
    current_c_array = Resize(current_c_array, old_bottom, old_top);
    deque_t->c_array = current_c_array;
  }

  Put(deque_t->c_array, old_bottom, tour_t);
  deque_t->bottom = old_bottom + 1;
}

tour* PopTopDeque(deque* deque_t) {
  int old_top = GetTop(deque_t);
  int new_top = old_top + 1;
  int old_bottom = deque_t->bottom;
  int size = old_bottom - old_top;

  if(size <= 0) { return NULL; }

  tour* tour_t = Get(deque_t->c_array, old_top);

  if(deque_t->top == old_top) {
    deque_t->top = new_top;
    return tour_t;
  }

  return NULL;
}

tour* PopBottomDeque(deque* deque_t) {
  deque_t->bottom--;

  int old_top = GetTop(deque_t);
  int new_top = old_top + 1;
  int size = deque_t->bottom - old_top;

  if(size < 0) {
    deque_t->bottom = old_top;
    return NULL;
  }

  tour* tour_t = Get(deque_t->c_array, deque_t->bottom);

  if(size > 0) { return tour_t; }

  if(deque_t->top != old_top) {
    deque_t->top = new_top;
    return NULL;
  }

  deque_t->bottom = old_top + 1;
  return tour_t;
}

void PrintDeque(deque* deque_t) {
  printf("Top: %d, Bottom: %d\n", deque_t->top, deque_t->bottom);
  PrintCArray(deque_t->c_array, deque_t->bottom, deque_t->top);
}




