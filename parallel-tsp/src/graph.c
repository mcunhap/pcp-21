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
  float** adjency_matrix;
};

graph* CreateGraph(int size, float adjency_matrix[size][size]) {
  graph *graph_t = (graph*) calloc (1, sizeof(graph));

  if(!graph_t) {
    printf("Failed to create graph.\n");
    exit(-1);
  }

  graph_t->adjency_matrix = (float**) calloc (size, sizeof(float*));

  if(!graph_t->adjency_matrix) {
    printf("Failed to create an adjency matrix rows for a graph.\n");
    exit(-1);
  }

  for(int i=0; i < size; i++) {
    graph_t->adjency_matrix[i] = (float*) calloc (size, sizeof(float));

    if(!graph_t->adjency_matrix[i]) {
      printf("Failed to create an adjency matrix columns for a graph.\n");
      exit(-1);
    }
  }

  for(int i=0; i < size; i++) {
    for(int j=0; j < size; j++) {
      graph_t->adjency_matrix[i][j] = adjency_matrix[i][j];
    }
  }

  return graph_t;
}

float GetEdgeWeight(graph* graph_t, int i, int j) {
  return graph_t->adjency_matrix[i][j];
}

// Just for debugging
void PrintGraph(graph* graph_t, int size) {
  for(int i=0; i < size; i++) {
    for(int j=0; j < size; j++) {
      printf("%.2f ", graph_t->adjency_matrix[i][j]);
    }
    printf("\n");
  }
}
