/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/11/2021 16:59:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
typedef struct graph_t graph;

graph* CreateGraph(int size, int nodes[size], float** adjency_matrix);

void FreeGraph(graph* graph_t);

float GetEdgeWeight(graph* graph_t, int i, int j);

int NumNodes(graph* graph_t);

// Just for debugging
void PrintGraph(graph *graph_t);
