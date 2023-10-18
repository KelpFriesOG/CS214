CC      = gcc
CFLAGS  = -g -std=c99 -fsanitize=address,undefined -Wall
RM      = rm -f

all: memgrind memtest

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) -o memgrind memgrind.o mymalloc.o

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) -o memtest memtest.o mymalloc.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

memgrind.o memtest.o mymalloc.o: mymalloc.h

# default: all

# all: Hello

# Hello: basictesting.c
# 	$(CC) $(CFLAGS) -o main.o basictesting.c

# basictesting.c : mymalloc.h mymalloc.c

clean:
	rm -f *.o memgrind memtest

clean:
	@echo "Cleaning up..."
	rm main.o