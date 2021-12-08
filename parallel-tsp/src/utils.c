/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2021 21:40:36
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

void ReadNCities(int* n_cities) {
  printf("Cities number:\n");
  scanf("%d", n_cities);
}

int ReadInstance(int n_cities, char* filename, float** m) {
  FILE *f;

  f = fopen(filename, "r");
  if(f == NULL) {
    printf("Failed to open file\n");
    return 0;
  }

  for(int i=0; i < n_cities; i++) {
    for(int j=0; j < n_cities; j++) {
      fscanf(f, "%f", &m[i][j]);
    }
  }

  fclose(f);

  return 1;
}
