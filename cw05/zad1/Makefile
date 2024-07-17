CC = gcc
CFLAGS = -Wall
SRC = main.c
EXEC = main
READ = read_mail.c
WRITE = write_mail.c

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)
	
read: $(READ)
	$(CC) $(CFLAGS) $(READ) -o read 

write: $(WRITE)
	$(CC) $(CFLAGS) $(WRITE) -o write

clean:
	rm -f $(EXEC) main read write

