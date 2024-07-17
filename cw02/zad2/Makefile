CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

all: reverse1 reverse1024

reverse1: reverse1.c
	$(CC) $(CFLAGS) -o $@ $<

reverse1024: reverse1024.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f reverse1 reverse1024 *.txt

