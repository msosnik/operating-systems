CC = gcc
CFLAGS = -Wall
SRC = main.c
EXEC = main

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)
	
other:other_flags.c
	$(CC) $(FLAGS) other_flags.c -o other

clean:
	rm -f $(EXEC) other

