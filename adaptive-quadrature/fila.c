#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct elemento {
  double dados_a;
  double dados_b;
  double area;
  struct elemento *prox;
};

struct fila {
  Elemento *primeiro;
  Elemento *ultimo;
  int tamanho;
};

Fila* CriarFila() {
  Fila *f = (Fila*)calloc(1, sizeof(Fila));
  if(!f) {
    printf("Falha ao criar a fila!");
    exit(-1);
  }

  f->primeiro = NULL;
  f->ultimo = NULL;
  f->tamanho = 0;

  return f;
}

// insere o item do inicio da Fila
void InsereTarefa(Fila *f, double a, double b) {
  Elemento *e = (Elemento*)calloc(1, sizeof(Elemento));
  if(!e) {
    printf("Falha ao inserir novo elemento na fila!");
    exit(-1);
  }

  e->dados_a = a;
  e->dados_b = b;

  // Fila nao ta vazia
  if(f->ultimo != NULL) {
    f->ultimo->prox = e;
  } else {
    f->primeiro = e;
  }

  f->ultimo = e;
  f->tamanho++;
}

// retira o item do inicio da Fila
Elemento *RetiraTarefa(Fila *f) {
  Elemento *e = (Elemento*)calloc(1, sizeof(Elemento));
  if(!e) {
    printf("Falha ao retirar da fila!");
    exit(-1);
  }

  Elemento *p;

  // Fila nao vazia
  if(!Vazia(f)) {
    e->dados_a = f->primeiro->dados_a;
    e->dados_b = f->primeiro->dados_b;
    p = f->primeiro;
    f->primeiro = f->primeiro->prox;

    if(f->primeiro == NULL) {
      f->ultimo = NULL;
    }
    f->tamanho--;

    free(p);
  }

  return e;
}

// Retorna verdadeiro com a Fila vazia
int Vazia(Fila *f) {
  return (f->primeiro == NULL);
}

void ExibeFila(Fila *f) {
  Elemento *e;

  if(Vazia(f)) {
    printf("Fila vazia!\n");
  }

  e = f->primeiro;
  while(e != NULL) {
    printf("A: %lf, B: %lf\n", e->dados_a, e->dados_b);
    e = e->prox;
  }
  printf("\n\n");
}

int LeTamanho(Fila *f) {
  return f->tamanho;
}

void ExibeElemento(Elemento *e) {
  printf("A: %lf, B: %lf\n", e->dados_a, e->dados_b);
}

double LeDadoA(Elemento *e) {
  return e->dados_a;
}

double LeDadoB(Elemento *e) {
  return e->dados_b;
}
