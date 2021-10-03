#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <semaphore.h>
#include <sys/time.h>
#include "fila.h"

int num_threads;
double a, b, tolerancia;
Fila *tarefas;
sem_t sem_tarefas;

void entrada_parametros() {
  printf("Digite o limite inferior (a) do intervalo:\n");
  scanf("%lf", &a);
  printf("Digite o limite superior (b) do intervalo:\n");
  scanf("%lf", &b);
  printf("Digite a tolerancia:\n");
  scanf("%lf", &tolerancia);
}

double tempo_execucao_total_ms(double (*funcao)(int, double, double, double), int num_threads, double a, double b, double tolerancia) {
  struct timeval tempo_inicio, tempo_fim;
  double inicio_ms, fim_ms, resultado;

  gettimeofday(&tempo_inicio, NULL);
  inicio_ms = (double)tempo_inicio.tv_sec + ((double)tempo_inicio.tv_usec / 1000000.0) * 1000;

  resultado = funcao(num_threads, a, b, tolerancia);
  gettimeofday(&tempo_fim, NULL);
  fim_ms = (double)tempo_fim.tv_sec + ((double)tempo_fim.tv_usec / 1000000.0) * 1000;

  printf("Resultado: %lf. ", resultado);

  return fim_ms - inicio_ms;
}

double area_trapezio(double a, double b, double h) {
  return (a+b)*h/2;
}

double funcao(double x) {
  double res = x != 0 ? fabs(sin(x) / (x)) : 1.0;
  return res;
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

double execucao_tarefas_omp(int num_threads, double a, double b, double tolerancia) {
  int erro;
  double total = 0;

  tarefas = CriarFila();
  erro = sem_init(&sem_tarefas, 0, 1);
  if (erro) { printf("Falha ao iniciar semaforo"); return 1;}

  omp_set_num_threads(num_threads);

  InsereTarefa(tarefas, a, b);

  while(!Vazia(tarefas)) {
    #pragma omp parallel for
    for(int i=0; i<LeTamanho(tarefas); i++) {
      Elemento *e;
      sem_wait(&sem_tarefas);
      e = RetiraTarefa(tarefas);
      sem_post(&sem_tarefas);

      #pragma omp critical
      total += calcula_quadratura_adaptativa(e);
    }
  }

}

int main(void) {
  entrada_parametros();

  printf("1 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_tarefas_omp, 1, a, b, tolerancia));
  printf("2 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_tarefas_omp, 2, a, b, tolerancia));
  printf("4 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_tarefas_omp, 4, a, b, tolerancia));

  return 0;
}

