#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>

int num_threads;
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
  return x*x + 10*x + 43;
  /* return x*x - 100; */
  /* double res = x != 0 ? fabs(sin(x) / (x)) : 1.0; */
  /* return res; */

  /* double res = x != 0 ? sin(x) / (x) : 1.0; */
  /* return res; */
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

  printf("A: %lf ", info->a);
  printf("B: %lf\n", info->b);
  printf("Area maior: %lf\n", area_trapezio_maior);
  printf("Area esquerda: %lf\n", area_trapezio_esquerdo);
  printf("Area direita: %lf\n", area_trapezio_direito);
  printf("Limite da area: %lf\n", area_trapezio_maior - area_trapezio_maior*tolerancia);
  printf("Area calculada: %lf\n", (area_trapezio_esquerdo + area_trapezio_direito));
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


int main(void) {
  double area_total = 0;
  info_calculo *info[num_threads];

  omp_set_num_threads(num_threads);
  entrada_parametros();

  double intervalo = b - a;
  double espacamento = intervalo / num_threads;
  printf("Espacamento: %f ", espacamento);
  printf("Intervalo: %f\n", intervalo);

  for(int i=0; i<num_threads; i++) {
    info[i] = (info_calculo*)calloc(1, sizeof(info_calculo));
    if(!info[i]) { exit(-1); }

    info[i]->a = a + espacamento*i;
    info[i]->b = info[i]->a + espacamento;
    printf("Intervalo: %d, A: %lf, B: %lf\n", i, info[i]->a, info[i]->b);
  }

  #pragma omp parallel for
  for(int i=0; i<num_threads; i++) {
    double resultado = calcula_quadratura_adaptativa(info[i]);

    #pragma omp critical
    area_total += resultado;
  }

  printf("Area total: %lf", area_total);

  return 0;
}
