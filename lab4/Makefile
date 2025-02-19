CC=gcc
FLAGS=-Wall -Wextra -O1 -march=native -g

default: main

main: main.c memory.s
	$(CC) $(FLAGS) -o $@ main.c memory.s

clean:
