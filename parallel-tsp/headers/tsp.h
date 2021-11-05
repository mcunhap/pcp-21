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

#include "../headers/stack.h"
#include "../headers/tour.h"
#include "../headers/graph.h"

void EvaluateTours(stack* stack_t, graph* graph_t, tour* best_tour, int n_cities, int hometown);

