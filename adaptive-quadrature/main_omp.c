#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>

double a, b, tolerancia;

struct c {
  double a;
  double b;
  double area;
}; typedef struct c info_calculo;

double tempo_execucao_total_ms(double (*funcao)(int, double, double, double), int num_threads, double a, double b, double tolerancia) {
  struct timeval tempo_inicio, tempo_fim;
  double inicio_ms, fim_ms, resultado;

  gettimeofday(&tempo_inicio, NULL);
  resultado = funcao(num_threads, a, b, tolerancia);
  gettimeofday(&tempo_fim, NULL);

  inicio_ms = (double)tempo_inicio.tv_sec + ((double)tempo_inicio.tv_usec / 1000000.0) * 1000;
  fim_ms = (double)tempo_fim.tv_sec + ((double)tempo_fim.tv_usec / 1000000.0) * 1000;

  printf("Resultado: %lf. ", resultado);

  return fim_ms - inicio_ms;
}

double tempo_execucao_thread_ms(double (*funcao)(info_calculo*), info_calculo *info) {
  struct timeval tempo_inicio, tempo_fim;
  double inicio_ms, fim_ms;

  gettimeofday(&tempo_inicio, NULL);
  info->area = funcao(info);
  gettimeofday(&tempo_fim, NULL);

  inicio_ms = (double)tempo_inicio.tv_sec + ((double)tempo_inicio.tv_usec / 1000000.0) * 1000;
  fim_ms = (double)tempo_fim.tv_sec + ((double)tempo_fim.tv_usec / 1000000.0) * 1000;

  printf("Resultado: %lf. ", info->area);

  return fim_ms - inicio_ms;
}

double tempo_execucao(double (*funcao)(info_calculo*), info_calculo *parametros) {
  struct timeval tempo_atual, tempo_depois;

  gettimeofday(&tempo_atual, NULL);
  parametros->area = funcao(parametros);
  gettimeofday(&tempo_depois, NULL);

  return (double)tempo_depois.tv_sec + tempo_depois.tv_usec / 1000000.0 - (double)tempo_atual.tv_sec + tempo_atual.tv_usec / 1000000.0;
}

double area_trapezio(double a, double b, double h) {
  return (a+b)*h/2;
}

double funcao(double x) {
  double res = x != 0 ? fabs(sin(x) / (x)) : 1.0;
  return res;
}

void entrada_parametros() {
  printf("Digite o limite inferior (a) do intervalo:\n");
  scanf("%lf", &a);
  printf("Digite o limite superior (b) do intervalo:\n");
  scanf("%lf", &b);
  printf("Digite a tolerancia:\n");
  scanf("%lf", &tolerancia);
}

double calcula_quadratura_adaptativa(info_calculo* info) {
  double area_trapezio_maior = area_trapezio(funcao(info->a), funcao(info->b), info->b-info->a);
  double meio = (info->a + info->b) / 2;
  double area_trapezio_esquerdo = area_trapezio(funcao(info->a), funcao(meio), meio-info->a);
  double area_trapezio_direito = area_trapezio(funcao(meio), funcao(info->b), info->b-meio);

  if(fabs(area_trapezio_maior - (area_trapezio_esquerdo + area_trapezio_direito)) <= fabs(area_trapezio_maior*tolerancia)) {
    return area_trapezio_esquerdo + area_trapezio_direito;
  }

  info_calculo *info_esquerda = (info_calculo*)calloc(1, sizeof(info_calculo));
  if(!info_esquerda) { pthread_exit(NULL); };

  info_calculo *info_direita = (info_calculo*)calloc(1, sizeof(info_calculo));
  if(!info_direita) { pthread_exit(NULL); };

  info_esquerda->a = info->a;
  info_esquerda->b = meio;
  info_direita->a = meio;
  info_direita->b = info->b;

  double area_esquerda = calcula_quadratura_adaptativa((void*)info_esquerda);
  double area_direita = calcula_quadratura_adaptativa((void*)info_direita);

  return area_esquerda + area_direita;
}

double execucao_omp(int num_threads, double a, double b, double tolerancia) {
  double area_total = 0;
  info_calculo *info[num_threads];

  omp_set_num_threads(num_threads);

  double intervalo = b - a;
  double espacamento = intervalo / num_threads;

  #pragma omp parallel for
  for(int i=0; i<num_threads; i++) {
    double tempo_execucao_ms;

    info[i] = (info_calculo*)calloc(1, sizeof(info_calculo));
    if(!info[i]) { exit(-1); }

    info[i]->a = a + espacamento*i;
    info[i]->b = info[i]->a + espacamento;

    tempo_execucao_ms = tempo_execucao_thread_ms(calcula_quadratura_adaptativa, info[i]);

    printf("Thread: %d. Tempo: %lf.\n", i, tempo_execucao_ms);

    #pragma omp critical
    area_total += info[i]->area;
  }

  return area_total;
}

int main(void) {
  entrada_parametros();

  printf("1 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_omp, 1, a, b, tolerancia));
  printf("2 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_omp, 2, a, b, tolerancia));
  printf("4 thread ## \n");
  printf("Tempo: %lf ms.\n", tempo_execucao_total_ms(execucao_omp, 4, a, b, tolerancia));

  return 0;
}
