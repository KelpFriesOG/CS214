CC = gcc
CFLAGS = -g -std=c99 -Wall -fsanitize=address,undefined

all: test enum

test: test.o lines.o
	$(CC) $(CFLAGS) -o $@ $^

test.o lines.o: lines.h

enum: enum.c
	$(CC) $(CFLAGS) -o $@ $^
