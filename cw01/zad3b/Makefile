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

tests_O0: OPT_SUFFIX:=_O0
tests_O0: CFLAGS+=-O0
tests_O0: test_static test_shared test_dynamic

tests_O1: OPT_SUFFIX:=_O1
tests_O1: CFLAGS+=-O1
tests_O1: test_static test_shared test_dynamic

tests_O2: OPT_SUFFIX:=_O2
tests_O2: CFLAGS+=-O2
tests_O2: test_static test_shared test_dynamic

tests_O3: OPT_SUFFIX:=_O3
tests_O3: CFLAGS+=-O3
tests_O3: test_static test_shared test_dynamic

tests_Os: OPT_SUFFIX:=_Os
tests_Os: CFLAGS+=-Os
tests_Os: test_static test_shared test_dynamic

test_inputs.txt:
	exec >"$@" ;\
	printf "init 3\n" ;\
	printf "count library.c\n" ;\
	printf "count main.c\n" ;\
	printf "count library.h\n" ;\
	printf "show 0\n" ;\
	printf "show 1\n" ;\
	printf "show 2\n" ;\
	printf "delete 2\n" ;\
	printf "delete 1\n" ;\
	printf "delete 0\n" ;\
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
