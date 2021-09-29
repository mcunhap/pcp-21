#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define NUM_THREADS 4

double a, b, meio, tolerancia;

struct c {
  double a;
  double b;
  double area;
}; typedef struct c info_calculo;

// a base maior - b base menor - h altura
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

double calcula_quadratura_adaptativa(info_calculo* info) {
  double area_trapezio_maior = area_trapezio(funcao(info->a), funcao(info->b), info->b-info->a);
  double meio = (info->a + info->b) / 2;
  double area_trapezio_esquerdo = area_trapezio(funcao(info->a), funcao(meio), meio-info->a);
  double area_trapezio_direito = area_trapezio(funcao(meio), funcao(info->b), info->b-meio);

  printf("A: %f ", info->a);
  printf("B: %f\n", info->b);
  printf("Area maior: %f\n", area_trapezio_maior);
  printf("Area esquerda: %f\n", area_trapezio_esquerdo);
  printf("Area direita: %f\n", area_trapezio_direito);
  printf("Limite da area: %f\n", area_trapezio_maior - area_trapezio_maior*tolerancia);
  printf("Area calculada: %f\n", (area_trapezio_esquerdo + area_trapezio_direito));
  printf("Area diff: %f\n", fabs(area_trapezio_maior - (area_trapezio_esquerdo + area_trapezio_direito)));

  if(fabs(area_trapezio_maior - (area_trapezio_esquerdo + area_trapezio_direito)) <= fabs(area_trapezio_maior*tolerancia)) {
    printf("DEU BOM!\n\n");
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
  info->area = calcula_quadratura_adaptativa(info);

  printf("Sub area: %f \n\n\n", info->area);

  pthread_exit(NULL);
}

int main(void) {
  int erro;
  double area_final = 0;
  info_calculo *info[NUM_THREADS];
  pthread_t *workers = (pthread_t *)calloc(NUM_THREADS, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  a = -100;
  b = 100;
  meio = (a + b) / 2;
  tolerancia = 0.00001;
  /* tolerancia = 0.05; */

  double intervalo = b - a;
  double espacamento = intervalo / NUM_THREADS;
  printf("Espacamento: %f ", espacamento);
  printf("Intervalo: %f\n", intervalo);

  for(int i=0; i<NUM_THREADS; i++) {
    info[i] = (info_calculo*)calloc(1, sizeof(info_calculo));
    if(!info[i]) { exit(-1); }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    info[i]->a = a + espacamento*i;
    info[i]->b = info[i]->a + espacamento;

    erro = pthread_create(&workers[i], NULL, &calcula_intervalo, (void*)info[i]);

    if(erro) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      return 1;
    }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    erro = pthread_join(workers[i], NULL);

    if(erro) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      return 1;
    }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    area_final = area_final + info[i]->area;
  }

  printf("Area final: %f\n", area_final);

  return 0;
}
