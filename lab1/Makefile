CC=gcc
FLAGS=-Wall -Wextra -march=native -O1

default: main
	
main: main.c run.s
	$(CC) $(FLAGS) -o main main.c run.s


clean:
	rm main
