NUM_NOPS?=0

CC=gcc
FLAGS=-Wall -Wextra -O1 -DNUM_NOPS=$(NUM_NOPS)

default: main main.s

main: main.c
	$(CC) $(FLAGS) -o $@ $<

main.s: main.c
	$(CC) $(FLAGS) -S -o $@ $<


clean:
	rm main main.s
