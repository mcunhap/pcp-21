# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get -y install gcc
COPY tour.c ./
COPY tour.h ./
COPY main.c ./
RUN gcc -Wall -pthread main.c tour.c -o main
