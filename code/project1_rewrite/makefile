CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Werror
RM      = rm -f

default: all


all: Hello

Hello: memgrind.c
	$(CC) $(CFLAGS) -o main memgrind.c

memgrind.c : mymalloc.h mymalloc.c

clean:
	@echo "Cleaning up..."
	rm main
