CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Wall
RM      = rm -f

default: all

all: Hello

Hello: hello.c
	$(CC) $(CFLAGS) -o hello.o hello.c

clean:
	@echo "Cleaning up..."
	rm -f Hello