/*
 * =====================================================================================
 *
 *       Filename:  queue.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2021 22:04:22
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
#include "../headers/queue.h"

struct queue_t {
  tour** tours;
  int first;
  int last;
  int max_size;
  int size;
};

queue* CreateQueue(int max_size) {
  queue* queue_t = (queue*) calloc (1, sizeof(queue));

  if(!queue_t) {
    printf("Failed to create stack.\n");
    exit(-1);
  }

  queue_t->tours = (tour**) calloc (max_size, sizeof(tour*));
  queue_t->max_size = max_size;
  queue_t->size = 0;
  queue_t->first = 0;
  queue_t->last = max_size - 1;

  return queue_t;
}

void FreeQueue(queue* queue_t) {
  for(int i=0; i< queue_t->size; i++) {
    free(queue_t->tours[i]);
  }

  free(queue_t->tours);
}

void EnqueueCopy(queue* queue_t, tour* tour_t) {
  if (queue_t->size == queue_t->max_size) {
    printf("Queue full!\n");
    return;
  }

  int tour_size = GetTourMaxCities(tour_t);
  tour* tour_tmp = CreateTour(tour_size);

  CopyTour(tour_tmp, tour_t);

  queue_t->last = (queue_t->last + 1) % queue_t->max_size;
  queue_t->tours[queue_t->last] = tour_tmp;
  queue_t->size++;
}

tour* Dequeue(queue* queue_t) {
  if (EmptyQueue(queue_t)) {
    printf("Empty queue!\n");
    return NULL;
  }

  tour* aux_tour = queue_t->tours[queue_t->first];
  queue_t->first = (queue_t->first + 1) % queue_t->max_size;
  queue_t->size--;

  return aux_tour;
}

int EmptyQueue(queue* queue_t) {
  if (queue_t->size == 0) { return 1; }

  return 0;
}

int SizeQueue(queue* queue_t) {
  return queue_t->size;
}

int MaxSizeQueue(queue* queue_t) {
  return queue_t->max_size;
}

// Just for debugging
void PrintQueueInfo(queue* queue_t) {
  printf("Queue size: %d\n", queue_t->size);

  int i = queue_t->first;
  int j;
  do {
    PrintTourInfo(queue_t->tours[i]);
    j = i % queue_t->max_size;
    i++;
  } while(j != queue_t->last);
}
