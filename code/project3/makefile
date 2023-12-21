CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Werror
RM      = rm -f

default: all


all: Hello

Hello: mysh.c
	$(CC) $(CFLAGS) -o main mysh.c

clean:
	@echo "Cleaning up..."
	rm main
