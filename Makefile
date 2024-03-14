CC=gcc
CFLAGS=-Wall -g -I./headers
SOURCES=src/main.c src/shell.c src/parser/command.c src/parser/scanner.c src/parser/parser.c src/builtIns.c src/parser/operators.c src/parser/tokenList.c
EXECUTABLE=shell

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

