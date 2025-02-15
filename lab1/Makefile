CC=gcc
ASM=-S
FLAGS=-Wall -Wextra -march=native -O

default: main main.s

main: main.c
	$(CC) $(FLAGS) -o $@ $<

main.s: main.c
	$(CC) $(FLAGS) $(ASM) -o $@ $<


clean:
	rm main main.s
