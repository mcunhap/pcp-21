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

typedef struct term_t term;

term* CreateTerm();

void EvaluateTours(stack* stack_t, graph* graph_t, tour* best_tour, pthread_mutex_t evaluate_mutex, term* term_t, int n_cities, int hometown, int num_threads);

void InitializeStacks(int root_node, int n_stacks, int stack_size, stack* stacks[n_stacks], graph* graph_t);

void FillStacks(int num_threads, int root_node, int stack_size, graph* graph_t, stack* stacks[num_threads]);

