CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
OPT_SUFFIX =_zad3

all: test_static test_shared

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

test_static:  results_static.txt
test_shared:  results_shared.txt
test_dynamic: results_dynamic.txt

test_inputs.txt:
	exec >"$@" ;\
	printf "init 3\n" ;\
	printf "count library.c\n" ;\
	printf "count main.c\n" ;\
	printf "count library.h\n" ;\
	printf "show 0\n" ;\
	printf "show 1\n" ;\
	printf "show 2\n" ;\
	printf "delete index 2\n" ;\
	printf "delete index 1\n" ;\
	printf "delete index 0\n" ;\
	printf "destroy\n" ;\
	printf "exit\n"

results_static.txt: test_inputs.txt main_static
	./main_static <"$<" >"$(basename $@)$(OPT_SUFFIX).txt"

results_shared.txt: test_inputs.txt main_shared
	./main_shared <"$<" >"$(basename $@)$(OPT_SUFFIX).txt"

results_dynamic.txt: test_inputs.txt main_dynamic
	./main_dynamic<"$<" >"$(basename $@)$(OPT_SUFFIX).txt"

clean:
	rm -f *.a *.so *.o main_static main_shared main_dynamic test_inputs.txt results_*.txt
