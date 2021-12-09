.SILENT:
.DEFAULT_GOAL := help

COLOR_RESET = \033[0m
COLOR_COMMAND = \033[36m
COLOR_YELLOW = \033[33m
COLOR_GREEN = \033[32m
COLOR_RED = \033[31m

PROJECT := Parallel TSP Workstealing

CFLAGS = -Wall

## Object files
_OBJ_FILES = tour.o stack.o graph.o tsp.o utils.o queue.o circular_array.o deque.o
OBJ_FILES = $(_OBJ_FILES:%.o=./src/obj/%.o)

## Build imagem Docker
build:
	docker build -t tsp-workstealing .

## Run project
run: build
	docker run -it tsp-workstealing ./main

## Make tour dependency
tour: ./src/tour.c ./headers/tour.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make stack dependency
stack: ./src/stack.c ./headers/stack.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make graph dependency
graph: ./src/graph.c ./headers/graph.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make tsp dependency
tsp: ./src/tsp.c ./headers/tsp.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make utils dependency
utils: ./src/utils.c ./headers/utils.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make queue dependency
queue: ./src/queue.c ./headers/queue.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make circular array dependency
circular_array: ./src/circular_array.c ./headers/circular_array.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make deque dependency
deque: ./src/deque.c ./headers/deque.h
	gcc $(CFLAGS) -c $< -o ./src/obj/$@.o

## Make project dependencies
dependencies:
	make tour
	make stack
	make graph
	make tsp
	make utils
	make queue
	make circular_array
	make deque

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
