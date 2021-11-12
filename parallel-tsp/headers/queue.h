/*
 * =====================================================================================
 *
 *       Filename:  queue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2021 22:04:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tour.h"

typedef struct queue_t queue;

queue* CreateQueue(int max_size);

void FreeQueue(queue* queue_t);

void EnqueueCopy(queue* queue_t, tour* tour_t);

tour* Dequeue(queue* queue_t);

int EmptyQueue(queue* queue_t);

int SizeQueue(queue* queue_t);

// Just for debugging
void PrintQueueInfo(queue* queue_t);
