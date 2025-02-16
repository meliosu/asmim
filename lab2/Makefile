CC=gcc
OPTLEVEL=1
FLAGS=-Wall -Wextra -O$(OPTLEVEL)

default: main main.s

main: main.c
	$(CC) $(FLAGS) -o $@ $<

main.s: main.c
	$(CC) $(FLAGS) -S -o $@ $<


clean:
	rm main main.s
