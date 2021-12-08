/*
 * =====================================================================================
 *
 *       Filename:  graph.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/11/2021 16:59:05
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
#include "../headers/graph.h"

struct graph_t {
  int* nodes;
  int n_nodes;
  float** adjency_matrix;
};

graph* CreateGraph(int size, int nodes[size], float** adjency_matrix) {
  graph* graph_t = (graph*) calloc (1, sizeof(graph));

  if(!graph_t) { printf("Failed to create graph.\n"); exit(-1); }

  graph_t->n_nodes = size;

  graph_t->nodes = (int*) calloc (size, sizeof(int));
  if(!graph_t->nodes) { printf("Failed to create nodes for a graph.\n"); exit(-1); }

  for(int i=0; i < size; i++) {
    graph_t->nodes[i] = nodes[i];
  }

  graph_t->adjency_matrix = (float**) calloc (size, sizeof(float*));
  if(!graph_t->adjency_matrix) { printf("Failed to create an adjency matrix rows for a graph.\n"); exit(-1); }

  for(int i=0; i < size; i++) {
    graph_t->adjency_matrix[i] = (float*) calloc (size, sizeof(float));
    if(!graph_t->adjency_matrix[i]) { printf("Failed to create an adjency matrix columns for a graph.\n"); exit(-1); }
  }

  for(int i=0; i < size; i++) {
    for(int j=0; j < size; j++) {
      graph_t->adjency_matrix[i][j] = adjency_matrix[i][j];
    }
  }

  return graph_t;
}

void FreeGraph(graph* graph_t) {
  for(int i=0; i< graph_t->n_nodes; i++) {
    free(graph_t->adjency_matrix[i]);
  }
  free(graph_t->adjency_matrix);
  free(graph_t->nodes);
}

float GetEdgeWeight(graph* graph_t, int i, int j) {
  return graph_t->adjency_matrix[i][j];
}

int NumNodes(graph* graph_t) {
  return graph_t->n_nodes;
}

// Just for debugging
void PrintGraph(graph* graph_t) {
  printf("Nodes: \n");
  for(int i=0; i < graph_t->n_nodes; i++) {
    printf("%d ", graph_t->nodes[i]);
  }
  printf("\n");

  printf("Adjency Matrix: \n");
  for(int i=0; i < graph_t->n_nodes; i++) {
    for(int j=0; j < graph_t->n_nodes; j++) {
      printf("%.2f ", graph_t->adjency_matrix[i][j]);
    }
    printf("\n");
  }
}
