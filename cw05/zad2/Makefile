CC = gcc
CFLAGS = -Wall
SRC = main.c
EXEC = main
CALC = calculate.c


$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)
	
calculate: $(CALC)
	$(CC) $(CFLAGS) $(CALC) -o calc

clean:
	rm -f $(EXEC) main time calc

