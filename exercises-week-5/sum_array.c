#include <stdio.h>
#include <stdlib.h>

/* Se paralelizarmos esse metodo para melhorar a execucao continuaremos executando em tempo O(n),
 * pois cada posicao do array de soma depende do resultado da posicao anterior para calcular o seu.*/
int summation_with_array(int* numbers, int size) {
  int *sum = (int*)calloc(size, sizeof(int));

  sum[size] = numbers[size];

  for(int i=0; i<size; i++) {
    sum[i] = numbers[i] + sum[i-1];
  }

  return sum[size-1];
}

/* Podemos paralelizar cada uma das chamadas recursivas. Pensando na recursao como uma arvore cada
 * nivel da arvore precisa esperar o nivel de baixo executar para computar a sua soma. Paralelizando
 * as chamadas conseguimos executar em tempo O(logn)*/
int summation_with_divide_and_conquer(int* numbers, int size) {
  if(size == 0) {
    return 0;
  } else if(size == 1) {
    return numbers[0];
  }

  int mid = size/2;

  /*soma vetor da esquerda e da direita recursivamente*/
  return summation_with_divide_and_conquer(numbers, mid) + summation_with_divide_and_conquer(&numbers[mid], size - mid);
}

int main(void) {
  int numbers[] = {1,2,3,4,5};
  int numbers_size = sizeof(*numbers) + 1;

  int array_sum = summation_with_array(numbers, numbers_size);
  int dq_sum = summation_with_divide_and_conquer(numbers, numbers_size);

  printf("Array sum: %d\n", array_sum);
  printf("DQ sum: %d", dq_sum);

  return 0;
}
