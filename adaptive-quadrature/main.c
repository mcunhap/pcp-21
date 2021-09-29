#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define NUM_THREADS 1

double a, b, middle, tolerance;

struct c {
  double a;
  double b;
  double area;
}; typedef struct c calculation_info;

// a base maior - b base menor - h altura
double trapezoid_area(double a, double b, double h) {
  return (a+b)*h/2;
}

double function(double x) {
  /* return x*x + 10*x + 43; */
  /* return x*x - 100; */
  /* double res = x != 0 ? fabs(sin(x) / (x)) : 1.0; */
  /* return res; */

  double res = x != 0 ? sin(x) / (x) : 1.0;
  return res;
}

//TODO: get better name
double calculate_adaptive_quadrature(calculation_info* info) {
  double bigger_trapezoid_area = trapezoid_area(function(info->a), function(info->b), info->b-info->a);
  double middle = (info->a + info->b) / 2;
  double left_trapezoid_area = trapezoid_area(function(info->a), function(middle), middle-info->a);
  double right_trapezoid_area = trapezoid_area(function(middle), function(info->b), info->b-middle);

  printf("A: %f ", info->a);
  printf("B: %f\n", info->b);
  printf("Bigger Area: %f\n", bigger_trapezoid_area);
  printf("Left Area: %f\n", left_trapezoid_area);
  printf("Right Area: %f\n", right_trapezoid_area);
  printf("Area limit: %f\n", bigger_trapezoid_area - bigger_trapezoid_area*tolerance);
  printf("Area calculated: %f\n", (left_trapezoid_area + right_trapezoid_area));
  printf("Area diff: %f\n", fabs(bigger_trapezoid_area - (left_trapezoid_area + right_trapezoid_area)));

  if(fabs(bigger_trapezoid_area - (left_trapezoid_area + right_trapezoid_area)) <= fabs(bigger_trapezoid_area*tolerance)) {
    printf("DEU BOM!\n\n");
    return left_trapezoid_area + right_trapezoid_area;
  }

  calculation_info *left_info = (calculation_info*)calloc(1, sizeof(calculation_info));
  if(!left_info) { pthread_exit(NULL); };

  calculation_info *right_info = (calculation_info*)calloc(1, sizeof(calculation_info));
  if(!right_info) { pthread_exit(NULL); };

  left_info->a = info->a;
  left_info->b = middle;
  right_info->a = middle;
  right_info->b = info->b;

  double left_area = calculate_adaptive_quadrature((void*)left_info);
  double right_area = calculate_adaptive_quadrature((void*)right_info);

  return left_area + right_area;
}

void* compute_interval(void *arg) {
  calculation_info *info = (calculation_info*)arg;
  info->area = calculate_adaptive_quadrature(info);

  printf("Sub area: %f \n\n\n", info->area);

  pthread_exit(NULL);
}

int main(void) {
  int error;
  double final_area = 0;
  calculation_info *c[NUM_THREADS];
  pthread_t *workers = (pthread_t *)calloc(NUM_THREADS, sizeof(pthread_t));
  if (!workers) { exit(-1); }

  a = -100;
  b = 100;
  middle = (a + b) / 2;
  tolerance = 0.00001;
  /* tolerance = 0.05; */

  double interval = b - a;
  double space = interval / NUM_THREADS;
  printf("Space: %f ", space);
  printf("Interval: %f\n", interval);

  for(int i=0; i<NUM_THREADS; i++) {
    c[i] = (calculation_info*)calloc(1, sizeof(calculation_info));
    if(!c[i]) { exit(-1); }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    c[i]->a = a + space*i;
    c[i]->b = c[i]->a + space;

    error = pthread_create(&workers[i], NULL, &compute_interval, (void*)c[i]);

    if(error) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      return 1;
    }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    error = pthread_join(workers[i], NULL);

    if(error) {
      printf("Falha ao criar a thread com id: %lu\n", (long)workers[i]);
      return 1;
    }
  }

  for(int i=0; i<NUM_THREADS; i++) {
    final_area = final_area + c[i]->area;
  }

  printf("Final area: %f\n", final_area);

  return 0;
}
