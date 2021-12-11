/*
 * =====================================================================================
 *
 *       Filename:  tsp.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/11/2021 22:37:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pthread.h>
#include "../headers/stack.h"
#include "../headers/tour.h"
#include "../headers/graph.h"
#include "../headers/queue.h"
#include "../headers/deque.h"

typedef struct term_t term;

term* CreateTerm();

void EvaluateTours(deque** deques, graph* graph_t, float* best_tour, pthread_mutex_t evaluate_mutex, term* term_t, int n_cities, int hometown, int num_threads, int my_id);

void FillBFSQueue(int num_instances, graph* graph_t, queue* bfs_queue, tour* initial_tour);

void ShareQueue(int num_instances, deque** deques, queue* queue_t);
