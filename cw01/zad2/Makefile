CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

all: static shared

lib_static: libblockarray.a
lib_shared: libblockarray.so

static:  main_static
shared:  main_shared
dynamic: main_dynamic

lib_static.o: library.c
	$(CC) $(CFLAGS) -c -o $@ $^

libblockarray.a: lib_static.o
	ar rcs $@ $^

lib_shared.o: library.c
	$(CC) $(CFLAGS) -c -fPIC -o $@ $^

libblockarray.so: lib_shared.o
	$(CC) -shared -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $^

main_dynamic.o: main.c
	$(CC) $(CFLAGS) -c -DDYNAMIC -o $@ $^

lib_dynamic.o: lib_dynamic.c
	$(CC) $(CFLAGS) -c -o $@ $^

main_static: main.o libblockarray.a
	$(CC) -o $@ main.o -L. -static -lblockarray

main_shared: main.o libblockarray.so
	$(CC) -o $@ main.o -L. -lblockarray -Wl,-rpath=`pwd`

main_dynamic: main_dynamic.o lib_dynamic.o libblockarray.so
	$(CC) -o $@ main_dynamic.o lib_dynamic.o -ldl

clean:
	rm -f *.a *.so *.o main_static main_shared main_dynamic

