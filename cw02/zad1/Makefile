CC = gcc
CFLAGS = -Wall -g

all: replace_lib.o replace_sys.o;

replace_lib.o:
	$(CC) $(CFLAGS) -c replace_lib.c
	$(CC) $(CFLAGS) -o replace_lib replace_lib.c

replace_sys.o:
	$(CC) $(CFLAGS) -c replace_sys.c
	$(CC) $(CFLAGS) -o replace_sys replace_sys.c

clean:
	rm -rf *.o *.a *.so *.txt replace_lib replace_sys
