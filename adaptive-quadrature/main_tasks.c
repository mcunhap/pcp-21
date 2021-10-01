#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <semaphore.h>
#include "fila.h"

int num_threads;
double a, b, tolerancia;
Fila *tarefas;
sem_t sem_tarefas;

void entrada_parametros() {
  printf("Digite o numero de threads:\n");
  scanf("%d", &num_threads);
  printf("Digite o limite inferior (a) do intervalo:\n");
  scanf("%lf", &a);
  printf("Digite o limite superior (b) do intervalo:\n");
  scanf("%lf", &b);
  printf("Digite a tolerancia:\n");
  scanf("%lf", &tolerancia);
}

double area_trapezio(double a, double b, double h) {
  return (a+b)*h/2;
}

double funcao(double x) {
  /* return x*x + 10*x + 43; */
  /* return x*x - 100; */
  double res = x != 0 ? fabs(sin(x) / (x)) : 1.0;
  return res;
  /* double res = x != 0 ? sin(x) / (x) : 1.0; */
  /* return res; */
}

double calcula_quadratura_adaptativa(Elemento* info) {
  double info_a = LeDadoA(info);
  double info_b = LeDadoB(info);

  double area_trapezio_maior = area_trapezio(funcao(info_a), funcao(info_b), info_b-info_a);
  double meio = (info_a + info_b) / 2;
  double area_trapezio_esquerdo = area_trapezio(funcao(info_a), funcao(meio), meio-info_a);
  double area_trapezio_direito = area_trapezio(funcao(meio), funcao(info_b), info_b-meio);

  if(fabs(area_trapezio_maior - (area_trapezio_esquerdo + area_trapezio_direito)) <= fabs(area_trapezio_maior*tolerancia)) {
    return area_trapezio_esquerdo + area_trapezio_direito;
  }

  sem_wait(&sem_tarefas);
  InsereTarefa(tarefas, info_a, meio);
  InsereTarefa(tarefas, meio, info_b);
  sem_post(&sem_tarefas);

  return 0;
}

int main(void) {
  int erro;
  double total = 0;

  tarefas = CriarFila();
  erro = sem_init(&sem_tarefas, 0, 1);
  if (erro) { printf("Falha ao iniciar semaforo"); return 1;}

  omp_set_num_threads(num_threads);
  entrada_parametros();

  InsereTarefa(tarefas, a, b);

  while(!Vazia(tarefas)) {
    #pragma omp parallel for
    for(int i=0; i<num_threads; i++) {
      Elemento *e;
      sem_wait(&sem_tarefas);
      e = RetiraTarefa(tarefas);
      sem_post(&sem_tarefas);

      #pragma omp critical
      total += calcula_quadratura_adaptativa(e);
    }
  }

  printf("Total: %lf\n", total);

  return 0;
}

