CC=gcc
CFLAGS=-Wall -g -I./headers
SOURCES=src/main.c src/shell.c src/command.c src/scanner.c src/parser.c src/builtIns.c
EXECUTABLE=shell

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

