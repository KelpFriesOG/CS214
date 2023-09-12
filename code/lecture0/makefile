CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Wall
RM      = rm -f


default: all

all: Hello

Hello: Hello.c
	$(CC) $(CFLAGS) -o Hello hello.c
