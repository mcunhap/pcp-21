#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
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
  double inicio_us, fim_us;

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

void* calcula_intervalo(void *arg) {
  info_calculo *info = (info_calculo*)arg;
  long long int tempo_execucao_us;
  info->area = calcula_quadratura_adaptativa(info);
  tempo_execucao_us = tempo_execucao_thread_us(&calcula_quadratura_adaptativa, info);

  printf("Thread: %lu. Tempo: %lld us. \n", (long)pthread_self(), tempo_execucao_us);

  pthread_exit(NULL);
}

double execucao_pthread(int num_threads, double a, double b, double tolerancia) {
  int erro;
  double intervalo, espacamento, area_final = 0.0;
  info_calculo *info[num_threads];

  pthread_t *workers = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  intervalo = b - a;
  espacamento = intervalo / num_threads;

  for(int i=0; i<num_threads; i++) {
    info[i] = (info_calculo*)calloc(1, sizeof(info_calculo));
    if(!info[i]) { exit(-1); }

    info[i]->a = a + espacamento*i;
    info[i]->b = info[i]->a + espacamento;

    erro = pthread_create(&workers[i], NULL, &calcula_intervalo, (void*)info[i]);

    if(erro) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      exit(-1);
    }
  }

  for(int i=0; i<num_threads; i++) {
    erro = pthread_join(workers[i], NULL);

    if(erro) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      exit(-1);
    }
  }

  for(int i=0; i<num_threads; i++) {
    area_final = area_final + info[i]->area;
  }

  return area_final;
}

int main(void) {
  entrada_parametros();

  printf("%d thread ## \n", num_threads);
  printf("Tempo: %lld us\n", tempo_execucao_total_us(execucao_pthread, num_threads, a, b, tolerancia));
  return 0;
}
