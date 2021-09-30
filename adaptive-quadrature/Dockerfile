# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get -y install gcc
COPY main_omp.c ./
RUN gcc -Wall -fopenmp main_omp.c -o main_omp -lm
CMD ./main_omp
