CC=gcc
CLIBS=-lncursesw
CFLAGS=-Wall -Wextra -pedantic -std=c11
IGNORE=-Wno-unused-function -Wno-unused-variable -Wno-unused-parameter


main: *.c *.h
	$(CC) main.c $(CFLAGS) $(CLIBS) $(IGNORE) -o main


debug: *.c *.h
	$(CC) main.c $(CFLAGS) $(CLIBS) $(IGNORE) -g -o main

run:
	./main
