# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get -y install gcc
COPY fila.c ./
COPY fila.h ./
COPY main_tasks.c ./
RUN gcc -Wall -fopenmp -pthread fila.c main_tasks.c -o fila -lm
CMD ./fila
