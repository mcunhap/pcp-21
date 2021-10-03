#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>

int num_threads;
double a, b, tolerancia;

struct c {
  double a;
  double b;
  double area;
}; typedef struct c info_calculo;

long long int tempo_execucao_total_us(double (*funcao)(int, double, double, double), int num_threads, double a, double b, double tolerancia) {
  struct timeval tempo_inicio, tempo_fim;
  long long int inicio_us, fim_us;
  double resultado;

  gettimeofday(&tempo_inicio, NULL);
  inicio_us = (tempo_inicio.tv_sec + (tempo_inicio.tv_usec / 1000000.0)) * 1000000;

  resultado = funcao(num_threads, a, b, tolerancia);

  gettimeofday(&tempo_fim, NULL);
  fim_us = (tempo_fim.tv_sec + (tempo_fim.tv_usec / 1000000.0)) * 1000000;

  printf("Resultado: %lf. ", resultado);

  return fim_us - inicio_us;
}

long long int tempo_execucao_thread_us(double (*funcao)(info_calculo*), info_calculo *info) {
  struct timeval tempo_inicio, tempo_fim;
  long long int inicio_us, fim_us;

  gettimeofday(&tempo_inicio, NULL);
  info->area = funcao(info);
  gettimeofday(&tempo_fim, NULL);

  inicio_us = (tempo_inicio.tv_sec + (tempo_inicio.tv_usec / 1000000.0)) * 1000000;
  fim_us = (tempo_fim.tv_sec + (tempo_fim.tv_usec / 1000000.0)) * 1000000;

  printf("Resultado: %lf. ", info->area);

  return fim_us - inicio_us;
}

double area_trapezio(double a, double b, double h) {
  return (a+b)*h/2;
}

double funcao(double x) {
  double res = x != 0 ? fabs(sin(x) / (x)) : 1.0;
  return res;
}

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
    double tempo_execucao_us;

    info[i] = (info_calculo*)calloc(1, sizeof(info_calculo));
    if(!info[i]) { exit(-1); }

    info[i]->a = a + espacamento*i;
    info[i]->b = info[i]->a + espacamento;

    tempo_execucao_us = tempo_execucao_thread_us(calcula_quadratura_adaptativa, info[i]);

    printf("Thread: %d. Tempo: %lld.\n", i, tempo_execucao_us);

    #pragma omp critical
    area_total += info[i]->area;
  }

  return area_total;
}

int main(void) {
  entrada_parametros();

  printf("%d thread ## \n", num_threads);
  printf("Tempo: %lld us.\n", tempo_execucao_total_us(execucao_omp, num_threads, a, b, tolerancia));

  return 0;
}
