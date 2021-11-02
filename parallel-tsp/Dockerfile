# syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update && \
    apt-get install make && \
    apt-get -y install gcc
ADD headers ./headers
ADD src ./src
COPY main.c ./
COPY Makefile ./

RUN make app
