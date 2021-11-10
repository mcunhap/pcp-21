/*
 * =====================================================================================
 *
 *       Filename:  mpi_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/11/2021 09:58:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int n_cities = 5;
  int nodes[] = { 0,1,2,3,4 };
  float adj_m[5][5] = {
    { 0.0, 3.0, 4.0, 2.0, 7.0 },
    { 3.0, 0.0, 4.0, 6.0, 3.0 },
    { 4.0, 4.0, 0.0, 5.0, 8.0 },
    { 2.0, 6.0, 5.0, 0.0, 6.0 },
    { 7.0, 3.0, 8.0, 6.0, 0.0 },
  };
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  MPI_Bcast(&n_cities, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&nodes, n_cities, MPI_INT, 0, MPI_COMM_WORLD);
  for(int i=0; i < n_cities; i++)
    MPI_Bcast(adj_m[i], n_cities, MPI_FLOAT, 0, MPI_COMM_WORLD);

  /* printf("Rank: %d, n_cities: %d, nodes: ", world_rank, n_cities); */
  /* for(int i=0; i < n_cities; i++) { */
  /*   printf("%d ", nodes[i]); */
  /* } */
  /* printf("\n"); */

  /* for(int i=0; i < n_cities; i++) { */
  /*   for(int j=0; j < n_cities; j++) { */
  /*     printf("%.2f ", adj_m[i][j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */

  // Finalize the MPI environment.
  MPI_Finalize();
}

