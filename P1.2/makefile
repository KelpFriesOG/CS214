CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Werror
RM      = rm -f

default: all

all: Hello

Hello: basictesting.c
	$(CC) $(CFLAGS) -o main.o basictesting.c

basictesting.c : mymalloc.h mymalloc.c

clean:
	@echo "Cleaning up..."
	rm main.o