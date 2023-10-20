CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Wall
RM      = rm -f

default: all

all: Hello

Hello: main.c
	$(CC) $(CFLAGS) -o main.o main.c

clean:
	@echo "Cleaning up..."
	rm main.o