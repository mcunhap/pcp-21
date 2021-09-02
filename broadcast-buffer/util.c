/*
 * =====================================================================================
 *
 *       Filename:  array.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/2021 16:50:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

int* array_vazio(int tam) {
  int *array = (int*)calloc(tam, sizeof(int));

  return array;
}

int** matriz_vazia(int linhas, int colunas) {
  int **matriz;

  matriz = (int**)calloc(linhas, sizeof(int*));

  for(int i=0; i<linhas; i++) {
    matriz[i] = array_vazio(colunas);
  }

  return matriz;
}
