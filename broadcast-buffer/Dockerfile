# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get -y install gcc
COPY buffer.c buffer.h ./
RUN gcc -Wall -pthread buffer.c -o buffer
CMD ./buffer
