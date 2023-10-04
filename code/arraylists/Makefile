CC = gcc
CFLAGS = -g -std=c99 -Wvla -Wall -fsanitize=address,undefined

# recipe - says how to create a file and its dependencies
# file_name: list_of_dependencies
#   <tab>   command

arrtest: arrtest.o arraylist.o
	$(CC) $(CFLAGS) arrtest.o arraylist.o -o arrtest

arraylist.o: arraylist.c arraylist.h
	$(CC) $(CFLAGS) -c arraylist.c

arrtest.o: arrtest.c arraylist.h
	$(CC) $(CFLAGS) -c arrtest.c

test: arrtest
	./arrtest
