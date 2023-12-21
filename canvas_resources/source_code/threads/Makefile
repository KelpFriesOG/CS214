CC = gcc
CFLAGS = -g -Wall -pthread

procon: procon.o queue.o
	$(CC) $(CFLAGS) $^ -o $@

procon.o queue.o: queue.h
