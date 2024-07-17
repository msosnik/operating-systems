CC = gcc
CFLAGS = -Wall

all: fork, exec

fork: fork.c
	$(CC) $(CFLAGS) -o fork fork.c

exec: exec.c
	$(CC) $(CFLAGS) -o exec exec.c
	
clean:
	rm -f fork exec

