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

#include <semaphore.h>
#include "../headers/stack.h"
#include "../headers/tour.h"
#include "../headers/graph.h"

void EvaluateTours(stack* stack_t, graph* graph_t, tour* best_tour, sem_t evaluate_sem, int n_cities, int hometown);

void InitializeStacks(int root_node, int n_stacks, int stack_size, stack* stacks[n_stacks], graph* graph_t);

void FillStacks(int num_threads, int root_node, int stack_size, graph* graph_t, stack* stacks[num_threads]);

