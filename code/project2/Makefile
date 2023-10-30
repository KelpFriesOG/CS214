CC = gcc
CFLAGS = -Wall
TARGET = main

$(TARGET): *.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET) <directory_path>