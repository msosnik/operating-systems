CC = gcc
CFLAGS = -Wall -g
LIB = library

all: static shared

static:
	$(CC) $(CFLAGS) -c $(LIB).c
	ar rcs $(LIB).a $(LIB).o

shared:
	$(CC) $(CFLAGS) -fPIC -c $(LIB).c
	$(CC) $(CFLAGS) -shared -o $(LIB).so $(LIB).o

clean:
	rm -rf *.o *.a *.so main
