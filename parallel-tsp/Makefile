.SILENT:
.DEFAULT_GOAL := help

COLOR_RESET = \033[0m
COLOR_COMMAND = \033[36m
COLOR_YELLOW = \033[33m
COLOR_GREEN = \033[32m
COLOR_RED = \033[31m

PROJECT := Parallel TSP

CFLAGS = -Wall

## Object files
_OBJ_FILES = tour.o
OBJ_FILES = $(_OBJ_FILES:%.o=./src/obj/%.o)

## Build imagem Docker
build:
	docker build -t parallel-tsp .

## Run project
run: build
	docker run -it parallel-tsp ./main

## Make tour dependency
tour: ./src/tour.c ./headers/tour.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make project dependencies
dependencies:
	make tour

## Make entire app
app:
	make dependencies
	gcc $(CFLAGS) -pthread -o main main.c $(OBJ_FILES)

## Commands
help:
	printf "\n${COLOR_YELLOW}${PROJECT}\n--------------------\n${COLOR_RESET}"
	awk '/^[a-zA-Z\-\_0-9\.%]+:/ { \
		helpMessage = match(lastLine, /^## (.*)/); \
		if (helpMessage) { \
			helpCommand = substr($$1, 0, index($$1, ":")); \
			helpMessage = substr(lastLine, RSTART + 3, RLENGTH); \
			printf "${COLOR_COMMAND}$$ make %s${COLOR_RESET} %s\n", helpCommand, helpMessage; \
		} \
	} \
	{ lastLine = $$0 }' $(MAKEFILE_LIST) | sort
	printf "\n"
