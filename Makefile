CC=gcc
CFLAGS=-Wall -g
SOURCES=main.c shell.c command.c scanner.c parser.c builtIns.c
EXECUTABLE=shell

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
