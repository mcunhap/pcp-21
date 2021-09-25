#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct Fila {
	int capacidade;
	float *dados_a;
  float *dados_b;
	int primeiro;
	int ultimo;
	int nItens; 
};

void CriarFila( struct Fila *f, int c ) { 
	f->capacidade = c;
	f->dados_a = (float*) malloc (f->capacidade * sizeof(float));
	f->dados_b = (float*) malloc (f->capacidade * sizeof(float));
	f->primeiro = 0;
	f->ultimo = -1;
	f->nItens = 0; 
}

// insere o item do inicio da Fila
void InsereTarefa(struct Fila *f, int a, int b) {
	if (f->ultimo == f->capacidade-1) {
		f->ultimo = -1;
  }
	f->ultimo++;
	f->dados_a[f->ultimo] = a;
	f->dados_b[f->ultimo] = b;
	f->nItens++;
}

// retira o item do inicio da Fila
void RetiraTarefa(struct Fila *f, float* result) { 
  int i = f->primeiro;
  int temp = i++;
  result[0] = f->dados_a[temp];
  result[1] = f->dados_b[temp];
	if (f->primeiro == f->capacidade) {
		f->primeiro = 0;
    }
	f->nItens--;
}

 // Retorna verdadeiro com a Fila vazia
int Vazia(struct Fila *f) {
	return (f->nItens==0);
}

 // Retorna verdadeiro com a Fila cheia
int Cheia(struct Fila *f) {
	return (f->nItens==f->capacidade);
}

void ExibeFila(struct Fila *f) {
	int cont, i;
	for (cont=0, i= f->primeiro; cont < f->nItens; cont++) {
    int temp = i++;
		printf("%.2f - %.2f \t",f->dados_a[temp], f->dados_b[temp]);
		if (i == f->capacidade){
			i=0;
    }
	}
	printf("\n\n");
}