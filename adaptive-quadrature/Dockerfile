# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get -y install gcc
COPY fila.c ./
COPY fila.h ./
COPY main_tasks.c ./
COPY main_omp.c ./
COPY main.c ./
RUN gcc -Wall -pthread main.c -o main -lm
RUN gcc -Wall -fopenmp main_omp.c -o main_omp -lm
RUN gcc -Wall -fopenmp -pthread fila.c main_tasks.c -o main_tasks -lm
